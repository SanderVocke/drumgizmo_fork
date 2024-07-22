/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            audiocacheidmanager.h
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
#pragma once

#include <cstdlib>

#include <vector>
#include <mutex>

#include "audiotypes.h"
#include "owner.h"

class AudioCacheFile;

using cacheid_t = int;

constexpr cacheid_t CACHE_DUMMYID{-2};
constexpr cacheid_t CACHE_NOID{-1};

class CacheBuffer
{
public:
	void allocBack(std::size_t chunk_size);
	void deleteChunks() const;
	void swap() noexcept;

	cacheid_t id{CACHE_NOID}; //< Current id of this CacheBuffer. CACHE_NOID means not in use.

	AudioCacheFile* afile{};
	size_t channel{};
	size_t pos{}; //< File position
	volatile bool ready{false};
	gsl::owner<sample_t*> front{};
	gsl::owner<sample_t*> back{};
	size_t localpos{}; //< Intra buffer (front) position.

	sample_t* preloaded_samples{}; // nullptr means preload buffer not active.
	size_t preloaded_samples_size{};
};

class AudioCacheIDManager
{
	friend class AudioCacheEventHandler;
public:
	AudioCacheIDManager() = default;
	~AudioCacheIDManager();

	//! Initialise id lists with specified capacity.
	//! Exceeding this capacity will result in CACHE_DUMMYID on calls to
	//! registerID.
	void init(unsigned int capacity);

	//! Get the cache object connected with the specified cacheid.
	//! Note: The cacheid MUST be active.
	CacheBuffer& getCache(cacheid_t cacheid);

	//! Reserve a new cache object and return its cacheid.
	//! The contents of the supplied cache object will be copied to the new
	//! cache object.
	cacheid_t registerID(const CacheBuffer& cache);

	//! Release a cache object and its correseponding cacheid.
	//! After this call the cacheid can no longer be used.
	void releaseID(cacheid_t cacheid);

protected:
	// For AudioCacheEventHandler
	void disableActive();
	std::vector<cacheid_t> getActiveIDs();

	std::mutex mutex;

	std::vector<CacheBuffer> id2cache;
	std::vector<cacheid_t> available_ids;
};
