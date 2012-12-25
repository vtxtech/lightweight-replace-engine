/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

// This is a stripped down version of the original OSG file
// to be used in Lightweight Replace Engine

#ifndef OSGDB_FILENAMEUTILS
#define OSGDB_FILENAMEUTILS 1

#include <string>
#include <vector>

namespace osgDB {

	/** Gets the parent path from full name (Ex: /a/b/c.Ext => /a/b). */
	extern std::string getFilePath(const std::string& filename);
	/** Gets file name with extension (Ex: /a/b/c.Ext => c.Ext). */
	extern std::string getSimpleFileName(const std::string& fileName);
	/** Gets file path without last extension (Ex: /a/b/c.Ext => /a/b/c ; file.ext1.ext2 => file.ext1). */
	extern std::string getNameLessExtension(const std::string& fileName);

	const char UNIX_PATH_SEPARATOR = '/';
	const char WINDOWS_PATH_SEPARATOR = '\\';

	/** Get the path separator for the current platform. */
	extern char getNativePathSeparator();

}

#endif
