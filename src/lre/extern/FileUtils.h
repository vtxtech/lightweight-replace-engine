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

#ifndef OSGDB_FILEUTILS
#define OSGDB_FILEUTILS 1

#include <vector>
#include <deque>
#include <string>
#include <stdio.h>

namespace osgDB {

/** Make a new directory.  Returns true if directory exists or was created. */
extern bool makeDirectory( const std::string &directoryPath );

/** return true if a file exists. */
extern bool fileExists(const std::string& filename);

enum FileType
{
    FILE_NOT_FOUND,
    REGULAR_FILE,
    DIRECTORY
};

/** return type of file. */
extern FileType fileType(const std::string& filename);

/** simple list of names to represent a directory's contents. */
typedef std::vector<std::string> DirectoryContents;

/** Return the contents of a directory.
  * Return value will contain filenames only, not absolute paths.
  * Returns an empty array on any error.*/
extern DirectoryContents getDirectoryContents(const std::string& dirName);

}        // namespace osgDB

#endif
