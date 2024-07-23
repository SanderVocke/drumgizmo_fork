/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            versionstr.cc
 *
 *  Wed Jul 22 11:41:32 CEST 2009
 *  Copyright 2009 Bent Bisballe Nyeng
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
#include "versionstr.h"

#include <cstdlib>
#include <cstdio>

#include <hugin.hpp>

// Workaround - major, minor and patch are defined as macros when using
// _GNU_SOURCES
#ifdef major
#undef major
#endif
#ifdef minor
#undef minor
#endif
#ifdef patch
#undef patch
#endif

VersionStr::VersionStr(const std::string& v)
{
	version = {{0, 0, 0}};
	set(v);
}

VersionStr::VersionStr(size_t major, size_t minor, size_t patch)
{
	version[0] = major;
	version[1] = minor;
	version[2] = patch;
}

void VersionStr::set(const std::string& v)
{
	std::string num;
	size_t idx = 0;
	for(size_t i = 0; i < v.length(); i++)
	{
		if(v[i] == '.')
		{
			if(idx > 2)
			{
				version = {{0, 0, 0}};
				ERR(version, "Version string is too long.");
				return;
			}
			version[idx] = atoi(num.c_str());
			idx++;
			num = "";
		}
		else if(v[i] >= '0' && v[i] <= '9')
		{
			num.append(1, v[i]);
		}
		else
		{
			version = {{0, 0, 0}};
			ERR(version, "Version string contains illegal character.");
			return;
		}
	}
	if(idx > 2)
	{
		version = {{0, 0, 0}};
		ERR(version, "Version string is too long.");
		return;
	}
	version[idx] = atoi(num.c_str());
}

VersionStr::operator std::string() const
{
	std::string v;
	char buf[64];
	if(patch())
	{
		sprintf(buf, "%d.%d.%d", (int)major(), (int)minor(), (int)patch());
	}
	else
	{
		sprintf(buf, "%d.%d", (int)major(), (int)minor());
	}
	v = buf;
	return v;
}

void VersionStr::operator=(const std::string& v)
{
	set(v);
}

// return a - b simplified as -1, 0 or 1
static int vdiff(const VersionStr& a, const VersionStr& b)
{
	if(a.major() < b.major())
	{
		return -1;
	}
	if(a.major() > b.major())
	{
		return 1;
	}
	if(a.minor() < b.minor())
	{
		return -1;
	}
	if(a.minor() > b.minor())
	{
		return 1;
	}
	if(a.patch() < b.patch())
	{
		return -1;
	}
	if(a.patch() > b.patch())
	{
		return 1;
	}
	return 0;
}

bool VersionStr::operator<(const VersionStr& other) const
{
	return vdiff(*this, other) == -1;
}

bool VersionStr::operator>(const VersionStr& other) const
{
	return vdiff(*this, other) == 1;
}

bool VersionStr::operator==(const VersionStr& other) const
{
	return vdiff(*this, other) == 0;
}

bool VersionStr::operator<=(const VersionStr& other) const
{
	return vdiff(*this, other) != 1;
}

bool VersionStr::operator>=(const VersionStr& other) const
{
	return vdiff(*this, other) != -1;
}

size_t VersionStr::major() const
{
	return version[0];
}

size_t VersionStr::minor() const
{
	return version[1];
}

size_t VersionStr::patch() const
{
	return version[2];
}
