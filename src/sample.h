/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            sample.h
 *
 *  Mon Jul 21 10:23:20 CEST 2008
 *  Copyright 2008 Bent Bisballe Nyeng
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

#include <map>
#include <string>

#include "channel.h"
#include "audiofile.h"

using AudioFiles = std::map<const InstrumentChannel*, AudioFile*>;

class Sample
{
public:
	Sample(const std::string& name, double power, double position,
	       double openness, bool normalized = false);
	~Sample();

	AudioFile* getAudioFile(const Channel& channel) const;

	double getPower() const;
	double getPosition() const;
	double getOpenness() const;
	bool getNormalized() const;

private:
	friend class DOMLoader;
	friend class DOMLoaderTest;
	friend class PowerList;

	void addAudioFile(InstrumentChannel* instrument_channel,
	                  AudioFile* audio_file);

	std::string name;
	double power;
	double position{0.0};
	double openness{0.0};
	bool normalized;
	AudioFiles audiofiles;
};
