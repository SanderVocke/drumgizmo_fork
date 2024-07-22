/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiocache.cc
 *
 *  Fri Apr 10 10:39:24 CEST 2015
 *  Copyright 2015 Jonas Suhr Christensen
 *  jsc@umbraculum.org
 ****************************************************************************/

/*
 *  This file is part of DrumGizmo.
 *
 *  DrumGizmo is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  DrumGizmo is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DrumGizmo; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#include "audiocache.h"

#include <mutex>

#include <cstdio>
#include <cassert>

#include <hugin.hpp>

#include "audiocachefile.h"

AudioCache::AudioCache(Settings& settings)
	: settings(settings)
{
}

AudioCache::~AudioCache()
{
	DEBUG(cache, "~AudioCache() pre\n");

	deinit();
	delete[] nodata;

	DEBUG(cache, "~AudioCache() post\n");
}

void AudioCache::init(std::size_t poolsize)
{
	setAsyncMode(true);

	id_manager.init(poolsize);
	event_handler.start();
}

void AudioCache::deinit()
{
	event_handler.stop();
}

// Invariant: initial_samples_needed < preloaded audio data
gsl::owner<sample_t*> AudioCache::open(const AudioFile& file,
                                       std::size_t initial_samples_needed,
                                       int channel, cacheid_t& new_cacheid)
{
	assert(chunk_size); // Assert updateChunkSize was called before processing.

	if(!file.isValid())
	{
		settings.number_of_underruns.fetch_add(1);
		// File preload not yet ready - skip this sample.
		new_cacheid = CACHE_DUMMYID;
		assert(nodata);
		return nodata;
	}

	// Register a new id for this cache session.
	new_cacheid = id_manager.registerID({});

	// If we are out of available ids we get CACHE_DUMMYID
	if(new_cacheid == CACHE_DUMMYID)
	{
		settings.number_of_underruns.fetch_add(1);
		// Use nodata buffer instead.
		assert(nodata);
		return nodata;
	}

	// Get the CacheBuffer connected with the registered id.
	CacheBuffer& cache = id_manager.getCache(new_cacheid);

	cache.afile = nullptr; // File is opened when needed.
	cache.channel = channel;

	// Next call to 'next()' will read from this point.
	cache.localpos = initial_samples_needed;

	cache.ready = false;
	cache.front = nullptr; // This is allocated when needed.
	cache.back = nullptr; // This is allocated when needed.

	std::size_t cropped_size{};

	if(file.preloadedsize == file.size)
	{
		// We have preloaded the entire file, so use it.
		cropped_size = file.preloadedsize;
	}
	else
	{
		// Make sure that the preload-data made available to the next() calls
		// fit on frame boundary:
		//
		// [                  all preloaded data                    ]
		// [ initial ][ biggest multiple of full frames ][ the rest ]
		// \                                            /
		//  \----------------------v-------------------/
		//                     cropped_size

		cropped_size = file.preloadedsize - cache.localpos;
		cropped_size -= cropped_size % framesize;
		cropped_size += initial_samples_needed;
	}

	cache.preloaded_samples = file.data;
	cache.preloaded_samples_size = cropped_size;

	// Next potential read from disk will read from this point.
	cache.pos = cropped_size;

	// Only load next buffer if there is more data in the file to be loaded...
	if(cache.pos < file.size)
	{
		cache.afile = &event_handler.openFile(file.filename);

		if(cache.back == nullptr)
		{
			cache.allocBack(chunk_size);
		}

		event_handler.pushLoadNextEvent(cache.afile, cache.channel, cache.pos,
		                                cache.back, &cache.ready);
	}

	return cache.preloaded_samples; // return preloaded data
}

gsl::owner<sample_t*> AudioCache::next(cacheid_t cacheid, std::size_t& size)
{
	if(cacheid == CACHE_DUMMYID)
	{
		settings.number_of_underruns.fetch_add(1);
		assert(nodata);
		return nodata;
	}

	CacheBuffer& cache = id_manager.getCache(cacheid);

	if(cache.preloaded_samples != nullptr)
	{
		// We are playing from memory:
		if(cache.localpos < cache.preloaded_samples_size)
		{
			sample_t* samples = cache.preloaded_samples + cache.localpos; // NOLINT: Fix with span?
			// If only a partial frame is returned. Reflect this in the size
			size = std::min(size, cache.preloaded_samples_size - cache.localpos);

			cache.localpos += size;
			return samples;
		}

		cache.preloaded_samples = nullptr; // Start using samples from disk.

	}
	else
	{
		// We are playing from cache:
		if(cache.localpos < chunk_size)
		{
			if(cache.front == nullptr)
			{
				// Just return silence.
				settings.number_of_underruns.fetch_add(1);
				cache.localpos += size; // Skip these samples so we don't loose sync.
				assert(nodata);
				return nodata;
			}

			sample_t* samples = cache.front + cache.localpos;// NOLINT: Fix with span?
			// If only a partial frame is returned. Reflect this in the size
			size = std::min(size, chunk_size - cache.localpos);
			cache.localpos += size;
			return samples;
		}
	}

	// Check for buffer underrun
	if(!cache.ready)
	{
		// Just return silence.
		settings.number_of_underruns.fetch_add(1);
		cache.localpos += size; // Skip these samples so we don't loose sync.
		assert(nodata);
		return nodata;
	}

	// Swap buffers
	cache.swap();

	// Next time we go here we have already read the first frame.
	cache.localpos = size;

	cache.pos += chunk_size;

	// Does the file have remaining unread samples?
	assert(cache.afile); // Assert that we have an audio file.
	if(cache.pos < cache.afile->getSize())
	{
		// Do we have a back buffer to read into?
		if(cache.back == nullptr)
		{
			cache.allocBack(chunk_size);
		}

		event_handler.pushLoadNextEvent(cache.afile, cache.channel, cache.pos,
		                                cache.back, &cache.ready);
	}

	// We should always have a front buffer at this point.
	assert(cache.front);
	return cache.front;
}

bool AudioCache::isReady(cacheid_t cacheid)
{
	if(cacheid == CACHE_DUMMYID)
	{
		return true;
	}

	const CacheBuffer& cache = id_manager.getCache(cacheid);
	return cache.ready;
}

void AudioCache::close(cacheid_t cacheid)
{
	if(cacheid == CACHE_DUMMYID)
	{
		return;
	}

	event_handler.pushCloseEvent(cacheid);
}

void AudioCache::setFrameSize(std::size_t framesize)
{
	// Make sure the event handler thread is stalled while we set the framesize
	// state.
	const std::lock_guard<AudioCacheEventHandler> event_handler_lock(event_handler);

	// NOTE: Not threaded...
	//std::lock_guard<AudioCacheIDManager> id_manager_lock(id_manager);

	if(framesize > nodata_framesize)
	{
		if(nodata != nullptr)
		{
			nodata_dirty.emplace_back(nodata); // Store for later deletion.
		}
		nodata = new sample_t[framesize];
		nodata_framesize = framesize;

		for(std::size_t i = 0; i < framesize; ++i)
		{
			nodata[i] = 0.0f;// NOLINT: Fix with span?
		}
	}

	this->framesize = framesize;
}

std::size_t AudioCache::getFrameSize() const
{
	return framesize;
}

void AudioCache::updateChunkSize(std::size_t output_channels)
{
	// Make sure we won't get out-of-range chunk sizes.
	constexpr std::size_t max_cache_chunk_size{512ul * 1024ul};
	const auto disk_cache_chunk_size =
		std::max(settings.disk_cache_chunk_size.load(), max_cache_chunk_size);
	constexpr std::size_t min_output_channels{1};
	output_channels = std::max(output_channels, min_output_channels);

	// 1MB pr. chunk divided over 16 channels, 4 bytes pr. sample.
	const auto ideal_chunk_size =
		disk_cache_chunk_size / output_channels / sizeof(sample_t);

	// Chunk size must match a whole number of frames.
	chunk_size = (ideal_chunk_size / framesize) * framesize;

	event_handler.setChunkSize(chunk_size);
}

void AudioCache::setAsyncMode(bool async)
{
	event_handler.setThreaded(async);
}

bool AudioCache::isAsyncMode() const
{
	return event_handler.isThreaded();
}
