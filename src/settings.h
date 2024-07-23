/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            settings.h
 *
 *  Tue Mar 22 10:59:46 CET 2016
 *  Copyright 2016 Christian Gl�ckner
 *  cgloeckner@freenet.de
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

#include <atomic>
#include <string>
#include <limits>

#include <cassert>

#include "atomic.h"
#include "notifier.h"

enum class LoadStatus : unsigned int
{
	Idle,
	Parsing,
	Loading,
	Done,
	Error
};

//! Engine settings
struct Settings
{
	Atomic<std::string> drumkit_file{""};
	Atomic<LoadStatus> drumkit_load_status{LoadStatus::Idle};
	Atomic<std::string> drumkit_name{""};
	Atomic<std::string> drumkit_description{""};
	Atomic<std::string> drumkit_version{""};
	Atomic<std::size_t> drumkit_samplerate{44100};

	//! The maximum amount of memory in bytes that the AudioCache
	//! is allowed to use for preloading. Default is 1GB.
	Atomic<std::size_t> disk_cache_upper_limit{1024 * 1024 * 1024};

	//! The optimal read chunk size from the disk.
	Atomic<std::size_t> disk_cache_chunk_size{1024 * 1024};

	Atomic<bool> disk_cache_enable{true};

	Atomic<std::size_t> number_of_underruns{0};

	//! Increment this in order to invoke a reload of the current drumkit.
	Atomic<std::size_t> reload_counter{0};

	Atomic<std::string> midimap_file{""};
	Atomic<LoadStatus> midimap_load_status{LoadStatus::Idle};

	Atomic<bool> enable_velocity_modifier{true};

	static float constexpr velocity_modifier_falloff_default = 0.5f;
	static float constexpr velocity_modifier_weight_default = 0.25f;
	static float constexpr velocity_stddev_default = .45f;
	static float constexpr position_stddev_default = 0.f; // FIXME: set to something sensible
	static float constexpr openness_stddev_default = 0.f; // FIXME: set to something sensible
	static float constexpr sample_selection_f_close_default = .85f;
	static float constexpr sample_selection_f_position_default = 1.f;
	static float constexpr sample_selection_f_openness_default = 1.f;
	static float constexpr sample_selection_f_diverse_default = .16f;
	static float constexpr sample_selection_f_random_default = .07f;
	Atomic<float> velocity_modifier_falloff{velocity_modifier_falloff_default};
	Atomic<float> velocity_modifier_weight{velocity_modifier_weight_default};
	Atomic<float> velocity_stddev{velocity_stddev_default};
	Atomic<float> position_stddev{position_stddev_default};
	Atomic<float> openness_stddev{openness_stddev_default};
	Atomic<float> sample_selection_f_close{sample_selection_f_close_default};
	Atomic<float> sample_selection_f_position{sample_selection_f_position_default};
	Atomic<float> sample_selection_f_openness{sample_selection_f_openness_default};
	Atomic<float> sample_selection_f_diverse{sample_selection_f_diverse_default};
	Atomic<float> sample_selection_f_random{sample_selection_f_random_default};

	//! Control number of times to retry sample selection as long as the sample
	//! is the same one as the last one.
	//! 0: will do no retries, ie. just use the first sample found.
	// FIXME: remove when new sample algorithm is introduced and also remove other occurences
	static std::size_t constexpr sample_selection_retry_count_default = 3;
	Atomic<std::size_t> sample_selection_retry_count{sample_selection_retry_count_default};

	// Current velocity offset - for UI
	Atomic<float> velocity_modifier_current{1.0f};

	Atomic<bool> enable_velocity_randomiser{false};
	Atomic<float> velocity_randomiser_weight{0.1f};

	Atomic<float> samplerate{44100.0};
	Atomic<std::size_t> buffer_size{1024}; // Only used to show in the UI.

	Atomic<bool> enable_resampling{true};
	Atomic<bool> resampling_recommended{false};
	Atomic<float> resampling_quality{0.7f}; // [0.0f; 1.0f]

	Atomic<std::size_t> number_of_files{0};
	Atomic<std::size_t> number_of_files_loaded{0};
	Atomic<std::string> current_file{""};

	Atomic<bool> enable_bleed_control{false};
	Atomic<float> master_bleed{1.0f};
	Atomic<bool> has_bleed_control{false};

	Atomic<bool> normalized_samples{true};

	Atomic<bool> enable_latency_modifier{false};

	//! Maximum "early hits" introduces latency in milliseconds.
	Atomic<float> latency_max_ms{150.0f};

	//! 0 := on-beat
	//! positive := laid back
	//! negative := up-beat
	//! Same range is [-100; 100] ms
	static float constexpr latency_laid_back_ms_default = 0.0f;
	Atomic<float> latency_laid_back_ms{latency_laid_back_ms_default};

	//! 0.0 := Robot
	//! 2.0 := Good drummer
	//! 4.0 := Decent drummer
	//! 6.0 := Decent drummer on a bad day
	//! 8.0 := Bad drummer
	//! 10.0 := Bad and drunk drummer
	static float constexpr latency_stddev_default = 2.0f;
	Atomic<float> latency_stddev{latency_stddev_default};

	//! Regain on-beat position.
	//! 0: instantaniously
	//! 1: never
	static float constexpr latency_regain_default = 0.9f;
	Atomic<float> latency_regain{latency_regain_default};

	// Current latency offset in ms - for UI
	Atomic<float> latency_current{0};

	// Powermap parameters
	Atomic<bool> enable_powermap;
	Atomic<float> powermap_fixed0_x{0.};
	Atomic<float> powermap_fixed0_y{0.};
	Atomic<float> powermap_fixed1_x{.5};
	Atomic<float> powermap_fixed1_y{.5};
	Atomic<float> powermap_fixed2_x{1.};
	Atomic<float> powermap_fixed2_y{1.};
	Atomic<bool> powermap_shelf{true};

	// Powermap visualizer; -1 is "none"
	Atomic<float> powermap_input{-1.};
	Atomic<float> powermap_output{-1.};

	Atomic<std::size_t> audition_counter{0};
	Atomic<std::string> audition_instrument;
	Atomic<float> audition_velocity;

	// Notify UI about load errors
	Atomic<std::string> load_status_text;

	// Enables the ramping down of old samples once X groups of the same instrument are playing.
	Atomic<bool> enable_voice_limit{false};
	// Max number of voices before old samples are ramped down.
	static std::size_t constexpr voice_limit_max_default = 15;
	Atomic<std::size_t> voice_limit_max{voice_limit_max_default};
	// Time it takes for an old sample to completely fall silent.
	static float constexpr voice_limit_rampdown_default = 0.5f;
	Atomic<float> voice_limit_rampdown{voice_limit_rampdown_default};
};

//! Settings getter class.
struct SettingsGetter
{
	SettingRef<std::string> drumkit_file;
	SettingRef<LoadStatus> drumkit_load_status;
	SettingRef<std::string> drumkit_name;
	SettingRef<std::string> drumkit_description;
	SettingRef<std::string> drumkit_version;
	SettingRef<std::size_t> drumkit_samplerate;

	SettingRef<std::size_t> disk_cache_upper_limit;
	SettingRef<std::size_t> disk_cache_chunk_size;
	SettingRef<bool> disk_cache_enable;
	SettingRef<std::size_t> number_of_underruns;
	SettingRef<std::size_t> reload_counter;

	SettingRef<std::string> midimap_file;
	SettingRef<LoadStatus> midimap_load_status;

	SettingRef<bool> enable_velocity_modifier;
	SettingRef<float> velocity_modifier_falloff;
	SettingRef<float> velocity_modifier_weight;
	SettingRef<float> velocity_stddev;
	SettingRef<float> position_stddev;
	SettingRef<float> openness_stddev;
	SettingRef<float> sample_selection_f_close;
	SettingRef<float> sample_selection_f_position;
	SettingRef<float> sample_selection_f_openness;
	SettingRef<float> sample_selection_f_diverse;
	SettingRef<float> sample_selection_f_random;

	SettingRef<std::size_t> sample_selection_retry_count;

	SettingRef<float> velocity_modifier_current;

	SettingRef<bool> enable_velocity_randomiser;
	SettingRef<float> velocity_randomiser_weight;

	SettingRef<float> samplerate;
	SettingRef<std::size_t> buffer_size;

	SettingRef<bool> enable_resampling;
	SettingRef<bool> resampling_recommended;
	SettingRef<float> resampling_quality;

	SettingRef<std::size_t> number_of_files;
	SettingRef<std::size_t> number_of_files_loaded;
	SettingRef<std::string> current_file;

	SettingRef<bool> enable_bleed_control;
	SettingRef<float> master_bleed;
	SettingRef<bool> has_bleed_control;

	SettingRef<bool> normalized_samples;

	SettingRef<bool> enable_latency_modifier;
	SettingRef<float> latency_max_ms;
	SettingRef<float> latency_laid_back_ms;
	SettingRef<float> latency_stddev;
	SettingRef<float> latency_regain;
	SettingRef<float> latency_current;

	SettingRef<bool> enable_powermap;
	SettingRef<float> powermap_fixed0_x;
	SettingRef<float> powermap_fixed0_y;
	SettingRef<float> powermap_fixed1_x;
	SettingRef<float> powermap_fixed1_y;
	SettingRef<float> powermap_fixed2_x;
	SettingRef<float> powermap_fixed2_y;
	SettingRef<bool> powermap_shelf;

	SettingRef<float> powermap_input;
	SettingRef<float> powermap_output;

	SettingRef<std::size_t> audition_counter;
	SettingRef<std::string> audition_instrument;
	SettingRef<float> audition_velocity;

	SettingRef<std::string> load_status_text;

	SettingRef<bool> enable_voice_limit;
	SettingRef<std::size_t> voice_limit_max;
	SettingRef<float> voice_limit_rampdown;

	SettingsGetter(Settings& settings)
		: drumkit_file(settings.drumkit_file)
		, drumkit_load_status(settings.drumkit_load_status)
		, drumkit_name(settings.drumkit_name)
		, drumkit_description(settings.drumkit_description)
		, drumkit_version(settings.drumkit_version)
		, drumkit_samplerate(settings.drumkit_samplerate)
		, disk_cache_upper_limit(settings.disk_cache_upper_limit)
		, disk_cache_chunk_size(settings.disk_cache_chunk_size)
		, disk_cache_enable(settings.disk_cache_enable)
		, number_of_underruns(settings.number_of_underruns)
		, reload_counter(settings.reload_counter)
		, midimap_file(settings.midimap_file)
		, midimap_load_status(settings.midimap_load_status)
		, enable_velocity_modifier{settings.enable_velocity_modifier}
		, velocity_modifier_falloff{settings.velocity_modifier_falloff}
		, velocity_modifier_weight{settings.velocity_modifier_weight}
		, velocity_stddev{settings.velocity_stddev}
		, position_stddev{settings.position_stddev}
		, openness_stddev{settings.openness_stddev}
		, sample_selection_f_close{settings.sample_selection_f_close}
		, sample_selection_f_position{settings.sample_selection_f_position}
		, sample_selection_f_openness{settings.sample_selection_f_openness}
		, sample_selection_f_diverse{settings.sample_selection_f_diverse}
		, sample_selection_f_random{settings.sample_selection_f_random}
		, sample_selection_retry_count(settings.sample_selection_retry_count)
		, velocity_modifier_current{settings.velocity_modifier_current}
		, enable_velocity_randomiser{settings.enable_velocity_randomiser}
		, velocity_randomiser_weight{settings.velocity_randomiser_weight}
		, samplerate{settings.samplerate}
		, buffer_size(settings.buffer_size)
		, enable_resampling{settings.enable_resampling}
		, resampling_recommended{settings.resampling_recommended}
		, resampling_quality{settings.resampling_quality}
		, number_of_files{settings.number_of_files}
		, number_of_files_loaded{settings.number_of_files_loaded}
		, current_file{settings.current_file}
		, enable_bleed_control{settings.enable_bleed_control}
		, master_bleed{settings.master_bleed}
		, has_bleed_control{settings.has_bleed_control}
		, normalized_samples{settings.normalized_samples}
		, enable_latency_modifier{settings.enable_latency_modifier}
		, latency_max_ms{settings.latency_max_ms}
		, latency_laid_back_ms{settings.latency_laid_back_ms}
		, latency_stddev{settings.latency_stddev}
		, latency_regain{settings.latency_regain}
		, latency_current{settings.latency_current}
		, enable_powermap{settings.enable_powermap}
		, powermap_fixed0_x{settings.powermap_fixed0_x}
		, powermap_fixed0_y{settings.powermap_fixed0_y}
		, powermap_fixed1_x{settings.powermap_fixed1_x}
		, powermap_fixed1_y{settings.powermap_fixed1_y}
		, powermap_fixed2_x{settings.powermap_fixed2_x}
		, powermap_fixed2_y{settings.powermap_fixed2_y}
		, powermap_shelf{settings.powermap_shelf}
		, powermap_input{settings.powermap_input}
		, powermap_output{settings.powermap_output}
		, audition_counter{settings.audition_counter}
		, audition_instrument{settings.audition_instrument}
		, audition_velocity{settings.audition_velocity}
		, load_status_text{settings.load_status_text}
		, enable_voice_limit{settings.enable_voice_limit}
		, voice_limit_max{settings.voice_limit_max}
		, voice_limit_rampdown{settings.voice_limit_rampdown}
	{
	}
};

//! Settings change notifier class.
class SettingsNotifier
{
public:
	Notifier<std::string> drumkit_file;
	Notifier<LoadStatus> drumkit_load_status;
	Notifier<std::string> drumkit_name;
	Notifier<std::string> drumkit_description;
	Notifier<std::string> drumkit_version;
	Notifier<std::size_t> drumkit_samplerate;

	Notifier<std::size_t> disk_cache_upper_limit;
	Notifier<std::size_t> disk_cache_chunk_size;
	Notifier<bool> disk_cache_enable;
	Notifier<std::size_t> number_of_underruns;
	Notifier<std::size_t> reload_counter;

	Notifier<std::string> midimap_file;
	Notifier<LoadStatus> midimap_load_status;

	Notifier<bool> enable_velocity_modifier;
	Notifier<float> velocity_modifier_falloff;
	Notifier<float> velocity_modifier_weight;
	Notifier<float> velocity_stddev;
	Notifier<float> position_stddev;
	Notifier<float> openness_stddev;
	Notifier<float> sample_selection_f_close;
	Notifier<float> sample_selection_f_position;
	Notifier<float> sample_selection_f_openness;
	Notifier<float> sample_selection_f_diverse;
	Notifier<float> sample_selection_f_random;
	Notifier<std::size_t> sample_selection_retry_count;
	Notifier<float> velocity_modifier_current;

	Notifier<bool> enable_velocity_randomiser;
	Notifier<float> velocity_randomiser_weight;

	Notifier<float> samplerate;
	Notifier<std::size_t> buffer_size;

	Notifier<bool> enable_resampling;
	Notifier<bool> resampling_recommended;
	Notifier<float> resampling_quality;

	Notifier<std::size_t> number_of_files;
	Notifier<std::size_t> number_of_files_loaded;
	Notifier<std::string> current_file;

	Notifier<bool> enable_bleed_control;
	Notifier<float> master_bleed;
	Notifier<bool> has_bleed_control;

	Notifier<bool> normalized_samples;

	Notifier<bool> enable_latency_modifier;
	Notifier<float> latency_max_ms;
	Notifier<float> latency_laid_back_ms;
	Notifier<float> latency_stddev;
	Notifier<float> latency_regain;
	Notifier<float> latency_current;

	Notifier<bool> enable_powermap;
	Notifier<float> powermap_fixed0_x;
	Notifier<float> powermap_fixed0_y;
	Notifier<float> powermap_fixed1_x;
	Notifier<float> powermap_fixed1_y;
	Notifier<float> powermap_fixed2_x;
	Notifier<float> powermap_fixed2_y;
	Notifier<bool> powermap_shelf;

	Notifier<float> powermap_input;
	Notifier<float> powermap_output;

	Notifier<std::size_t> audition_counter;
	Notifier<std::string> audition_instrument;
	Notifier<int> audition_velocity;

	Notifier<std::string> load_status_text;

	Notifier<bool> enable_voice_limit;
	Notifier<std::size_t> voice_limit_max;
	Notifier<float> voice_limit_rampdown;

	void evaluate()
	{
#define EVAL(x) if(settings.x.hasChanged()) { x(settings.x.getValue()); }

		EVAL(drumkit_file);
		EVAL(drumkit_load_status);
		EVAL(drumkit_name);
		EVAL(drumkit_description);
		EVAL(drumkit_version);
		EVAL(drumkit_samplerate);

		EVAL(disk_cache_upper_limit);
		EVAL(disk_cache_chunk_size);
		EVAL(disk_cache_enable);
		EVAL(number_of_underruns);
		EVAL(reload_counter);

		EVAL(midimap_file);
		EVAL(midimap_load_status);

		EVAL(enable_velocity_modifier);
		EVAL(velocity_modifier_falloff);
		EVAL(velocity_modifier_weight);
		EVAL(velocity_stddev);
		EVAL(position_stddev);
		EVAL(openness_stddev)
		EVAL(sample_selection_f_close);
		EVAL(sample_selection_f_position);
		EVAL(sample_selection_f_openness);
		EVAL(sample_selection_f_diverse);
		EVAL(sample_selection_f_random);
		EVAL(sample_selection_retry_count);
		EVAL(velocity_modifier_current);

		EVAL(enable_velocity_randomiser);
		EVAL(velocity_randomiser_weight);

		EVAL(samplerate);
		EVAL(buffer_size);

		EVAL(enable_resampling);
		EVAL(resampling_recommended);
		EVAL(resampling_quality);

		EVAL(number_of_files);
		EVAL(number_of_files_loaded);
		EVAL(current_file);

		EVAL(enable_bleed_control);
		EVAL(master_bleed);
		EVAL(has_bleed_control);

		EVAL(normalized_samples);

		EVAL(enable_latency_modifier);
		EVAL(latency_max_ms);
		EVAL(latency_laid_back_ms);
		EVAL(latency_stddev);
		EVAL(latency_regain);
		EVAL(latency_current);

		EVAL(enable_powermap);
		EVAL(powermap_fixed0_x);
		EVAL(powermap_fixed0_y);
		EVAL(powermap_fixed1_x);
		EVAL(powermap_fixed1_y);
		EVAL(powermap_fixed2_x);
		EVAL(powermap_fixed2_y);
		EVAL(powermap_shelf);

		EVAL(powermap_input);
		EVAL(powermap_output);

		EVAL(audition_counter);
		EVAL(audition_instrument);
		EVAL(audition_velocity);

		EVAL(load_status_text);

		EVAL(enable_voice_limit);
		EVAL(voice_limit_max);
		EVAL(voice_limit_rampdown);
	}

	SettingsNotifier(Settings& settings)
		: settings(settings)
	{
	}

private:
	SettingsGetter settings;
};

// lovely reminder: NO, GLOCKE. NOOOO!!
/*
enum class IntParams {
	Foo = 0
};

struct Settings
{
	std::array<std::atomic<int>, 5> ints;

	Settings()
		: ints{}
	{
		//get(IntParams::Foo).store(3);
	}

	std::atomic<int>& get(IntParams param)
	{
		return ints[(size_t)param];
	}
};

struct SettingsGetter
{
	std::vector<SettingRef<int>> ints;

	SettingsGetter(Settings& parent)
	{
		for(auto& atomic: parent.ints)
		{
			ints.emplace_back(atomic);
		}
	}
};
*/
