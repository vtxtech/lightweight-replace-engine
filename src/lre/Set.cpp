/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "Set.h"

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

} // namespace lre

