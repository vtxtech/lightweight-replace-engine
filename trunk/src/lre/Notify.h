/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief Inspired by OpenSceneGraph's Notify class
 *
 */

#ifndef lre_Notify_H
#define lre_Notify_H 1

#include "Export.h"

//-- STL --//
#include <iostream>

namespace lre {

	LRE_EXPORT enum NotifySeverity {
		DEBUG = 0, // printed in verbose mode only
		NOTICE = 1, // printed unless silent mode
		ALWAYS = 2, // always printed, e.g. for --help output
		ERROR = 3 // always printed, to be used for errors only
	};

	// Set the notify level
	void setNotifyLevel(NotifySeverity severity);

	// Returns the current notify level
	NotifySeverity getNotifyLevel();

	// Make some output to std::cout based on current notify level settings
	LRE_EXPORT std::ostream& notify(const NotifySeverity severity = lre::NOTICE);

} // namespace lre

#endif // lre_Notify_H

