/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "Component.h"

#include "FileUtils.h"

namespace lre {

	//=======================================================================================
	Component::Component(const std::string& name): name_(name)
	{

	}

	//=======================================================================================
	Component::~Component()
	{
		setList_.clear();
	}

	//=======================================================================================
	Set* Component::addSet()
	{
		setList_.push_back(Set());
		return &(setList_.at(setList_.size()-1));
	}

	//=======================================================================================
	Set* Component::getSet(unsigned int index)
	{
		if (index < getNumSets()) {
			return &(setList_.at(index));
		}
		return NULL;
	}

	//=======================================================================================
	std::string Component::toString() const
	{
		std::string result = "<LRE:COMPONENT:"+getName()+">"+lre::FileUtils::getNativeEndline();
		for (SetList::const_iterator itr = setList_.begin(); itr != setList_.end(); ++itr) {
			result += itr->toString()+lre::FileUtils::getNativeEndline();
		}
		result += "</LRE:COMPONENT:"+getName()+">";
		return result;
	}

	//=======================================================================================
	bool Component::fromString(const std::string& str)
	{
		std::string compTagBegin1 = "<LRE:COMPONENT:"; std::string compTagBegin2 = ">";
		std::string compTagEnd1 = "</LRE:COMPONENT:"; std::string compTagEnd2 = ">";

		std::string setTagBegin = "<LRE:SET>";
		std::string setTagEnd = "</LRE:SET>";

		unsigned int f = 0; // the current string position
		unsigned int e = 0; // position of the end of the component
		unsigned int i = 0; // number of sets found inside the component

		f = str.find(compTagBegin1, f);
		// No component inside String
		if (f == std::string::npos) { return false; }
		f += compTagBegin1.size();

		unsigned int namePos = str.find(compTagBegin2, f);
		// Component ">" missing
		if (namePos == std::string::npos) { return false; }

		// Determine Component Name
		std::string compName = str.substr(f, namePos-f);
		f = namePos + compTagBegin2.size();
		name_ = compName;

		std::string endTag = compTagEnd1+compName+compTagEnd2;
		e = str.find(endTag, f);
		// Missing endTag "</LRE:COMPONENT:compName>"
		if (e == std::string::npos) { return false; }
		
		while (f != std::string::npos && f < e) {
			f = str.find(setTagBegin, f);
			if (f == std::string::npos || f >= e) {
				if (i == 0) {
					// No set found, yet. So this is an error.
					return false;
				} else {
					// We already have one or more sets found, so we should be sure everything is done.
					return true;
				}
			}
			unsigned int f2 = str.find(setTagEnd, f+setTagBegin.size());
			// Missing end tag "</LRE:SET>"
			if (f2 == std::string::npos || f2 >= e) { return false; }
			f2 += setTagEnd.size();
			
			// Everything seems to be fine, so evaluate data inside LRE:KEY-Tag
			lre::Set newSet;
			if (newSet.fromString(str.substr(f, f2-f))) {
				setList_.push_back(newSet);
				f = f2;
				i++;
			} else {
				// Invalid SET data
				return false;
			}
		}

		return (i>0);
	}

} // namespace lre

