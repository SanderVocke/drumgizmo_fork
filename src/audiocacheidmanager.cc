/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiocacheidmanager.cc
 *
 *  Tue Jan  5 10:59:37 CET 2016
 *  Copyright 2016 Bent Bisballe Nyeng
 *  deva@aasimon.org
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
#include "audiocacheidmanager.h"

#include <limits>
#include <cassert>

void CacheBuffer::allocBack(std::size_t chunk_size)
{
	back = new sample_t[chunk_size];
}

void CacheBuffer::deleteChunks() const
{
	delete[] front;
	delete[] back;
}

void CacheBuffer::swap() noexcept
{
	std::swap(front, back);
}

AudioCacheIDManager::~AudioCacheIDManager()
{
	assert(available_ids.size() == id2cache.size()); // All ids should be released.
}

void AudioCacheIDManager::init(unsigned int capacity)
{
	const std::lock_guard<std::mutex> guard(mutex);

	id2cache.resize(capacity);
	available_ids.resize(capacity);
	for(int i = 0; i < static_cast<int>(capacity); ++i)
	{
		available_ids[i] = i;
	}
}

CacheBuffer& AudioCacheIDManager::getCache(cacheid_t cacheid)
{
	const std::lock_guard<std::mutex> guard(mutex);

	assert(cacheid != CACHE_NOID);
	assert(cacheid != CACHE_DUMMYID);
	assert(cacheid >= 0);
	assert(cacheid < (int)id2cache.size());
	assert(id2cache[cacheid].id == cacheid);

	return id2cache[cacheid];
}

cacheid_t AudioCacheIDManager::registerID(const CacheBuffer& cache)
{
	const std::lock_guard<std::mutex> guard(mutex);

	cacheid_t cacheid = CACHE_NOID;

	if(available_ids.empty())
	{
		return CACHE_DUMMYID;
	}
	else
	{
		cacheid = available_ids.back();
		available_ids.pop_back();
	}

	assert(id2cache[cacheid].id == CACHE_NOID); // Make sure it is not already in use

	id2cache[cacheid] = cache;
	id2cache[cacheid].id = cacheid;

	return cacheid;
}

void AudioCacheIDManager::releaseID(cacheid_t cacheid)
{
	const std::lock_guard<std::mutex> guard(mutex);

	assert(id2cache[cacheid].id != CACHE_NOID); // Test if it wasn't already released.

	id2cache[cacheid].id = CACHE_NOID;

	available_ids.push_back(cacheid);
}

void AudioCacheIDManager::disableActive()
{
	// Run through all active CacheBuffers and disable them.
	for(auto& cache : id2cache)
	{
		if(cache.id != CACHE_NOID)
		{
			// Force use of nodata in all of the rest of the next() calls:
			cache.localpos = std::numeric_limits<size_t>::max();
			cache.ready = false;
		}
	}
}

std::vector<cacheid_t> AudioCacheIDManager::getActiveIDs()
{
	std::vector<cacheid_t> active_ids;

	for(auto& cache : id2cache)
	{
		if(cache.id != CACHE_NOID)
		{
			active_ids.push_back(cache.id);
		}
	}

	return active_ids;
}
