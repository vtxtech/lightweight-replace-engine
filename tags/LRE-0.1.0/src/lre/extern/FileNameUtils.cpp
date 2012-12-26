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

//#include <stdlib.h>
#include <string.h>
//#include <limits.h>

#include "FileNameUtils.h"

//#ifdef WIN32
//    #define _WIN32_WINNT 0x0500
//    #include <windows.h>
//#endif

//#if defined(__sgi)
//    #include <ctype.h>
//#elif defined(__GNUC__) || !defined(WIN32) || defined(__MWERKS__)
//    #include <cctype>
//    using std::tolower;
//#endif

using namespace std;

static const char * const PATH_SEPARATORS = "/\\";
static unsigned int PATH_SEPARATORS_LEN = 2;

std::string osgDB::getFilePath(const std::string& fileName)
{
    std::string::size_type slash = fileName.find_last_of(PATH_SEPARATORS);
    if (slash==std::string::npos) return std::string();
    else return std::string(fileName, 0, slash);
}


std::string osgDB::getSimpleFileName(const std::string& fileName)
{
    std::string::size_type slash = fileName.find_last_of(PATH_SEPARATORS);
    if (slash==std::string::npos) return fileName;
    else return std::string(fileName.begin()+slash+1,fileName.end());
}

char osgDB::getNativePathSeparator()
{
#if defined(WIN32) && !defined(__CYGWIN__)
    return WINDOWS_PATH_SEPARATOR;
#else
    return UNIX_PATH_SEPARATOR;
#endif
}

// strip one level of extension from the filename.
std::string osgDB::getNameLessExtension(const std::string& fileName)
{
    std::string::size_type dot = fileName.find_last_of('.');
    std::string::size_type slash = fileName.find_last_of(PATH_SEPARATORS);        // Finds forward slash *or* back slash
    if (dot==std::string::npos || (slash!=std::string::npos && dot<slash)) return fileName;
    return std::string(fileName.begin(),fileName.begin()+dot);
}
