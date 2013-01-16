/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "Set.h"

#include "FileUtil.h"

//-- STL --//
#include <iostream>

namespace lre {

	//=======================================================================================
	Set::Set()
	{

	}

	//=======================================================================================
	Set::~Set()
	{
		
	}

	//=======================================================================================
	bool Set::addPair(const std::string& key, const std::string& value)
	{
		// Return false, if key is already in map
		StringStringMap::iterator itr = map_.find(key);
		if (itr != map_.end()) {
			return false;
		}

		// If key is not found, add the pair to the map
		map_.insert(StringStringPair(key, value));
		return true;
	}

	//=======================================================================================
	const StringStringMap& Set::getMap() const
	{
		return map_;
	}

	//=======================================================================================
	std::string Set::toString() const
	{
		std::string result = "\t<LRE:SET>"+lre::FileUtil::getNativeEndline();
		for (StringStringMap::const_iterator itr = map_.begin(); itr != map_.end(); ++itr) {
			result += "\t\t<LRE:KEY:"+itr->first+">"+itr->second+"</LRE:KEY:"+itr->first+">"+lre::FileUtil::getNativeEndline();
		}
		result += "\t</LRE:SET>";
		return result;
	}

	//=======================================================================================
	bool Set::fromString(const std::string& str)
	{
		std::string setTagBegin = "<LRE:SET>";
		std::string setTagEnd = "</LRE:SET>";

		std::string keyTagBegin1 = "<LRE:KEY:";	std::string keyTagBegin2 = ">";
		std::string keyTagEnd1 = "</LRE:KEY:"; std::string keyTagEnd2 = ">";

		unsigned int f = 0; // the current string position
		unsigned int e = 0; // position of the end of the set
		unsigned int i = 0; // number of keys found inside the set

		f = str.find(setTagBegin, f);

		// No set inside String
		if (f == std::string::npos) { return false; }
		f += setTagBegin.size();

		e = str.find(setTagEnd, f);
		// Set end tag missing
		if (e == std::string::npos) { return false; }

		// No key data
		if (f == e) { return false; }

		while (f != std::string::npos && f < e) {
			f = str.find(keyTagBegin1, f);
			// No key element or elements outside of set
			if (f == std::string::npos || f >= e) {
				if (i == 0) {
					// No key found, yet. So this is an error.
					return false;
				} else {
					// We already have one or more keys found, so we should be sure everything is done.
					return true;
				}
			}
			f += keyTagBegin1.size();

			// Determine the KEY
			unsigned int keyPos = str.find(keyTagBegin2, f);
			// Missing ">"
			if (keyPos == std::string::npos || keyPos >= e) { return false; }

			std::string key = str.substr(f, keyPos-f);
			std::string endTag = keyTagEnd1+key+keyTagEnd2;

			f = keyPos + keyTagBegin2.size();

			keyPos = str.find(endTag, f);
			// Missing endTag "</LRE:KEY:key>"
			if (keyPos == std::string::npos || keyPos >= e) { return false; }

			std::string value = str.substr(f, keyPos-f);
			f = keyPos+endTag.size();

			// Increment number of found LRE:KEY
			i++;

			addPair(key, value);
		}

		return (i>0);
	}

} // namespace lre

