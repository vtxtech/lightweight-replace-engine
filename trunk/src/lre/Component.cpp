/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "Component.h"

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
		if (index < getSetCount()) {
			return &(setList_.at(index));
		}
		return NULL;
	}

} // namespace lre

