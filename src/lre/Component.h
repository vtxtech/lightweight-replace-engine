/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef lre_Component_H
#define lre_Component_H 1

#include "Export.h"

#include "Set.h"

//-- STL --//
#include <string>
#include <deque>

namespace lre {

	class LRE_EXPORT Component {
		public:
			// Parameter constructor using a case sensitive name.
			Component(const std::string& name);
			// Virtual destructor
			virtual ~Component();

			// Returns the name of the component
			const std::string& getName() const { return name_; }

			// Creates a new empty Set and returns a reference to it for modification
			Set* addSet();

			// Get a Set
			Set* getSet(unsigned int index);

			// Get the number of sets
			unsigned int getNumSets() const { return setList_.size(); }

			// Convert Component to std::string
			std::string toString() const;

			// Set Component from std::string
			bool fromString(const std::string& str);

		private:
			std::string name_;
			SetList setList_;
	
	};

	typedef std::deque<Component> ComponentList;

} // namespace lre

#endif // lre_ReplaceEngine_H
