/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef lre_Set_H
#define lre_Set_H 1

#include "Export.h"

//-- STL --//
#include <string>
#include <map>
#include <deque>

#ifdef WIN32
	// Disable DLL Export warning on StringStringMap map_
	#pragma warning (disable: 4251)
#endif

namespace lre {

	typedef std::map<std::string, std::string> StringStringMap;
	typedef std::pair<std::string, std::string> StringStringPair;

	class LRE_EXPORT Set {
		public:
			// Default constructor
			Set();
			// Virtual destructor
			virtual ~Set();

			// Add a key/value pair to the map, if key does not exist in map.
			// @return False, if key exists
			bool addPair(const std::string& key, const std::string& value);

			// Get the map of key/value pairs
			const StringStringMap& getMap() const;

			// Convert Set to std::string
			std::string toString() const;

			// Set StringStringMap from std::string
			bool fromString(const std::string& str);

		private:
			StringStringMap map_;
	};

	typedef std::deque<Set> SetList;

} // namespace lre

#endif // lre_ReplaceEngine_H
