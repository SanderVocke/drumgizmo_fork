/* -*- Mode: c++ -*- */
/***************************************************************************
 *            statusframecontent.h
 *
 *  Fri Mar 24 21:49:50 CET 2017
 *  Copyright 2017 André Nusser
 *  andre.nusser@googlemail.com
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

#include "settings.h"
#include "textedit.h"
#include "widget.h"

class SettingsNotifier;

namespace GUI
{

class StatusframeContent : public Widget
{
public:
	StatusframeContent(Widget* parent, SettingsNotifier& settings_notifier);

	// From Widget
	virtual void resize(std::size_t width, std::size_t height) override;

	void updateContent();

	void updateDrumkitLoadStatus(LoadStatus load_status);
	void updateDrumkitName(std::string const& drumkit_name);
	void updateDrumkitDescription(std::string const& drumkit_description);
	void updateDrumkitVersion(std::string const& drumkit_version);
	void updateDrumkitSamplerate(std::size_t drumkit_samplerate);
	void updateMidimapLoadStatus(LoadStatus load_status);
	void updateSamplerate(double samplerate);
	void updateBufferSize(std::size_t buffer_size);
	void updateResamplingEnabled(bool enable_resampling);
	void updateResamplingActive(bool resampling_active);
	void updateNumberOfUnderruns(std::size_t number_of_underruns);

private:
	TextEdit text_field{this};

	SettingsNotifier& settings_notifier;

	std::string drumkit_load_status;
	std::string drumkit_name;
	std::string drumkit_description;
	std::string drumkit_version;
	std::string drumkit_samplerate;
	std::string midimap_load_status;
	std::string samplerate;
	std::string buffer_size;
	std::string resampling_enabled;
	std::string resampling_active;
	std::string number_of_underruns;
};

} // GUI::
