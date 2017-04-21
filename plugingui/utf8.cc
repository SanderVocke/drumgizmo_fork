/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            utf8.cc
 *
 *  Tue Feb 27 19:18:23 CET 2007
 *  Copyright  2006 Bent Bisballe Nyeng
 *  deva@aasimon.org
 ****************************************************************************/

/*
 *  This file is part of DrumGizmo.
 *
 *  DrumGizmo is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  DrumGizmo is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DrumGizmo; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#include "utf8.h"

UTF8::UTF8()
{
	// Encode Map
	map_encode["\x80"] = "�\x80";
	map_encode["\x81"] = "�\x81";
	map_encode["\x82"] = "�\x82";
	map_encode["\x83"] = "�\x83";
	map_encode["\x84"] = "�\x84";
	map_encode["\x85"] = "�\x85";
	map_encode["\x86"] = "�\x86";
	map_encode["\x87"] = "�\x87";
	map_encode["\x88"] = "�\x88";
	map_encode["\x89"] = "�\x89";
	map_encode["\x8a"] = "�\x8a";
	map_encode["\x8b"] = "�\x8b";
	map_encode["\x8c"] = "�\x8c";
	map_encode["\x8d"] = "�\x8d";
	map_encode["\x8e"] = "�\x8e";
	map_encode["\x8f"] = "�\x8f";
	map_encode["\x90"] = "�\x90";
	map_encode["\x91"] = "�\x91";
	map_encode["\x92"] = "�\x92";
	map_encode["\x93"] = "�\x93";
	map_encode["\x94"] = "�\x94";
	map_encode["\x95"] = "�\x95";
	map_encode["\x96"] = "�\x96";
	map_encode["\x97"] = "�\x97";
	map_encode["\x98"] = "�\x98";
	map_encode["\x99"] = "�\x99";
	map_encode["\x9a"] = "�\x9a";
	map_encode["\x9b"] = "�\x9b";
	map_encode["\x9c"] = "�\x9c";
	map_encode["\x9d"] = "�\x9d";
	map_encode["\x9e"] = "�\x9e";
	map_encode["\x9f"] = "�\x9f";
	map_encode["�"] = " ";
	map_encode["�"] = "¡";
	map_encode["�"] = "¢";
	map_encode["�"] = "£";
	map_encode["�"] = "¤";
	map_encode["�"] = "¥";
	map_encode["�"] = "¦";
	map_encode["�"] = "§";
	map_encode["�"] = "¨";
	map_encode["�"] = "©";
	map_encode["�"] = "ª";
	map_encode["�"] = "«";
	map_encode["�"] = "¬";
	map_encode["�"] = "­";
	map_encode["�"] = "®";
	map_encode["�"] = "¯";
	map_encode["�"] = "°";
	map_encode["�"] = "±";
	map_encode["�"] = "²";
	map_encode["�"] = "³";
	map_encode["�"] = "´";
	map_encode["�"] = "µ";
	map_encode["�"] = "¶";
	map_encode["�"] = "·";
	map_encode["�"] = "¸";
	map_encode["�"] = "¹";
	map_encode["�"] = "º";
	map_encode["�"] = "»";
	map_encode["�"] = "¼";
	map_encode["�"] = "½";
	map_encode["�"] = "¾";
	map_encode["�"] = "¿";
	map_encode["�"] = "�\x80";
	map_encode["�"] = "�\x81";
	map_encode["�"] = "�\x82";
	map_encode["�"] = "�\x83";
	map_encode["�"] = "�\x84";
	map_encode["�"] = "�\x85";
	map_encode["�"] = "�\x86";
	map_encode["�"] = "�\x87";
	map_encode["�"] = "�\x88";
	map_encode["�"] = "�\x89";
	map_encode["�"] = "�\x8a";
	map_encode["�"] = "�\x8b";
	map_encode["�"] = "�\x8c";
	map_encode["�"] = "�\x8d";
	map_encode["�"] = "�\x8e";
	map_encode["�"] = "�\x8f";
	map_encode["�"] = "�\x90";
	map_encode["�"] = "�\x91";
	map_encode["�"] = "�\x92";
	map_encode["�"] = "�\x93";
	map_encode["�"] = "�\x94";
	map_encode["�"] = "�\x95";
	map_encode["�"] = "�\x96";
	map_encode["�"] = "�\x97";
	map_encode["�"] = "�\x98";
	map_encode["�"] = "�\x99";
	map_encode["�"] = "�\x9a";
	map_encode["�"] = "�\x9b";
	map_encode["�"] = "�\x9c";
	map_encode["�"] = "�\x9d";
	map_encode["�"] = "�\x9e";
	map_encode["�"] = "�\x9f";
	map_encode["�"] = "à";
	map_encode["�"] = "á";
	map_encode["�"] = "â";
	map_encode["�"] = "ã";
	map_encode["�"] = "ä";
	map_encode["�"] = "å";
	map_encode["�"] = "æ";
	map_encode["�"] = "ç";
	map_encode["�"] = "è";
	map_encode["�"] = "é";
	map_encode["�"] = "ê";
	map_encode["�"] = "ë";
	map_encode["�"] = "ì";
	map_encode["�"] = "í";
	map_encode["�"] = "î";
	map_encode["�"] = "ï";
	map_encode["�"] = "ð";
	map_encode["�"] = "ñ";
	map_encode["�"] = "ò";
	map_encode["�"] = "ó";
	map_encode["�"] = "ô";
	map_encode["�"] = "õ";
	map_encode["�"] = "ö";
	map_encode["�"] = "÷";
	map_encode["�"] = "ø";
	map_encode["�"] = "ù";
	map_encode["�"] = "ú";
	map_encode["�"] = "û";
	map_encode["�"] = "ü";
	map_encode["�"] = "ý";
	map_encode["�"] = "þ";
	map_encode["�"] = "ÿ";

	// Decode Map
	map_decode["�\x80"] = "\x80";
	map_decode["�\x81"] = "\x81";
	map_decode["�\x82"] = "\x82";
	map_decode["�\x83"] = "\x83";
	map_decode["�\x84"] = "\x84";
	map_decode["�\x85"] = "\x85";
	map_decode["�\x86"] = "\x86";
	map_decode["�\x87"] = "\x87";
	map_decode["�\x88"] = "\x88";
	map_decode["�\x89"] = "\x89";
	map_decode["�\x8a"] = "\x8a";
	map_decode["�\x8b"] = "\x8b";
	map_decode["�\x8c"] = "\x8c";
	map_decode["�\x8d"] = "\x8d";
	map_decode["�\x8e"] = "\x8e";
	map_decode["�\x8f"] = "\x8f";
	map_decode["�\x90"] = "\x90";
	map_decode["�\x91"] = "\x91";
	map_decode["�\x92"] = "\x92";
	map_decode["�\x93"] = "\x93";
	map_decode["�\x94"] = "\x94";
	map_decode["�\x95"] = "\x95";
	map_decode["�\x96"] = "\x96";
	map_decode["�\x97"] = "\x97";
	map_decode["�\x98"] = "\x98";
	map_decode["�\x99"] = "\x99";
	map_decode["�\x9a"] = "\x9a";
	map_decode["�\x9b"] = "\x9b";
	map_decode["�\x9c"] = "\x9c";
	map_decode["�\x9d"] = "\x9d";
	map_decode["�\x9e"] = "\x9e";
	map_decode["�\x9f"] = "\x9f";
	map_decode[" "] = "�";
	map_decode["¡"] = "�";
	map_decode["¢"] = "�";
	map_decode["£"] = "�";
	map_decode["¤"] = "�";
	map_decode["¥"] = "�";
	map_decode["¦"] = "�";
	map_decode["§"] = "�";
	map_decode["¨"] = "�";
	map_decode["©"] = "�";
	map_decode["ª"] = "�";
	map_decode["«"] = "�";
	map_decode["¬"] = "�";
	map_decode["­"] = "�";
	map_decode["®"] = "�";
	map_decode["¯"] = "�";
	map_decode["°"] = "�";
	map_decode["±"] = "�";
	map_decode["²"] = "�";
	map_decode["³"] = "�";
	map_decode["´"] = "�";
	map_decode["µ"] = "�";
	map_decode["¶"] = "�";
	map_decode["·"] = "�";
	map_decode["¸"] = "�";
	map_decode["¹"] = "�";
	map_decode["º"] = "�";
	map_decode["»"] = "�";
	map_decode["¼"] = "�";
	map_decode["½"] = "�";
	map_decode["¾"] = "�";
	map_decode["¿"] = "�";
	map_decode["�\x80"] = "�";
	map_decode["�\x81"] = "�";
	map_decode["�\x82"] = "�";
	map_decode["�\x83"] = "�";
	map_decode["�\x84"] = "�";
	map_decode["�\x85"] = "�";
	map_decode["�\x86"] = "�";
	map_decode["�\x87"] = "�";
	map_decode["�\x88"] = "�";
	map_decode["�\x89"] = "�";
	map_decode["�\x8a"] = "�";
	map_decode["�\x8b"] = "�";
	map_decode["�\x8c"] = "�";
	map_decode["�\x8d"] = "�";
	map_decode["�\x8e"] = "�";
	map_decode["�\x8f"] = "�";
	map_decode["�\x90"] = "�";
	map_decode["�\x91"] = "�";
	map_decode["�\x92"] = "�";
	map_decode["�\x93"] = "�";
	map_decode["�\x94"] = "�";
	map_decode["�\x95"] = "�";
	map_decode["�\x96"] = "�";
	map_decode["�\x97"] = "�";
	map_decode["�\x98"] = "�";
	map_decode["�\x99"] = "�";
	map_decode["�\x9a"] = "�";
	map_decode["�\x9b"] = "�";
	map_decode["�\x9c"] = "�";
	map_decode["�\x9d"] = "�";
	map_decode["�\x9e"] = "�";
	map_decode["�\x9f"] = "�";
	map_decode["à"] = "�";
	map_decode["á"] = "�";
	map_decode["â"] = "�";
	map_decode["ã"] = "�";
	map_decode["ä"] = "�";
	map_decode["å"] = "�";
	map_decode["æ"] = "�";
	map_decode["ç"] = "�";
	map_decode["è"] = "�";
	map_decode["é"] = "�";
	map_decode["ê"] = "�";
	map_decode["ë"] = "�";
	map_decode["ì"] = "�";
	map_decode["í"] = "�";
	map_decode["î"] = "�";
	map_decode["ï"] = "�";
	map_decode["ð"] = "�";
	map_decode["ñ"] = "�";
	map_decode["ò"] = "�";
	map_decode["ó"] = "�";
	map_decode["ô"] = "�";
	map_decode["õ"] = "�";
	map_decode["ö"] = "�";
	map_decode["÷"] = "�";
	map_decode["ø"] = "�";
	map_decode["ù"] = "�";
	map_decode["ú"] = "�";
	map_decode["û"] = "�";
	map_decode["ü"] = "�";
	map_decode["ý"] = "�";
	map_decode["þ"] = "�";
	map_decode["ÿ"] = "�";
	// FIXME: This is just a hack to make Goran Mekic's name work.
	map_decode["�\x87"] = "c";
}

std::string UTF8::fromLatin1(std::string const& s)
{
	std::string ret;

	for(int i = 0; i < (int)s.length(); i++)
	{
		std::string c;

		if((unsigned char)s[i] <= 0x7F)
		{
			c = s.substr(i, 1);
		}
		else
		{
			c = map_encode[s.substr(i, 1)];
		}

		// If c == "", the character wasn't found in the map.
		// Ignore this case for now and just push an empty string in this case.

		ret.append(c);
	}

	return ret;
}

std::string UTF8::toLatin1(std::string const& s)
{
	std::string ret;

	int width = 1;
	for(int i = 0; i < (int)s.length(); i += width)
	{
		if(/*(unsigned char)s[i]>=0x00&&*/ (unsigned char)s[i] <= 0x7F)
		{
			width = 1; // 00-7F -> 1 byte
		}
		if((unsigned char)s[i] >= 0xC2 && (unsigned char)s[i] <= 0xDF)
		{
			width = 2; // C2-DF -> 2 bytes
		}
		if((unsigned char)s[i] >= 0xE0 && (unsigned char)s[i] <= 0xEF)
		{
			width = 3; // E0-EF -> 3 bytes
		}
		if((unsigned char)s[i] >= 0xF0 && (unsigned char)s[i] <= 0xF4)
		{
			width = 4; // F0-F4 -> 4 bytes
		}

		std::string c;
		if(width == 1)
		{
			c = s.substr(i, 1);
		}
		else
		{
			c = map_decode[s.substr(i, width)];
		}

		// If c == "", the character wasn't found in the map.
		// Ignore this case for now and just push an empty string in this case.

		ret.append(c);
	}

	return ret;
}
