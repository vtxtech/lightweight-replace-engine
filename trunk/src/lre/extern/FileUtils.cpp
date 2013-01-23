/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

// This is a stripped down version of the original OSG file
// to be used in Lightweight Replace Engine

#include "FileUtils.h"

// currently this impl is for _all_ platforms, except as defined.
// the mac version will change soon to reflect the path scheme under osx, but
// for now, the above include is commented out, and the below code takes precedence.

#if defined(WIN32) && !defined(__CYGWIN__)
    #define WINBASE_DECLARE_GET_MODULE_HANDLE_EX
    #include <windows.h>
    #include <sys/stat.h>
    #include <io.h>
    #include <direct.h>

    #define mkdir(x,y) _mkdir((x))
    #define stat64 _stati64

#else // unix

#if defined(__CYGWIN__) || defined(__FreeBSD__) || (defined(__hpux) && !defined(_LARGEFILE64_SOURCE))
    #define stat64 stat
#endif

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#endif

// set up _S_ISDIR()
#if !defined(S_ISDIR)
#  if defined( _S_IFDIR) && !defined( __S_IFDIR)
#    define __S_IFDIR _S_IFDIR
#  endif
#  define S_ISDIR(mode)    (mode&__S_IFDIR)
#endif

#include <errno.h>
#include <stack>
#include <iostream>

namespace osgDB
{
#define OSGDB_STRING_TO_FILENAME(s) s
#define OSGDB_FILENAME_TO_STRING(s) s
#define OSGDB_FILENAME_TEXT(x) x
#define OSGDB_WINDOWS_FUNCT(x) x ## A
}

static const char * const PATH_SEPARATORS = "/\\";

std::string getFilePath(const std::string& fileName)
{
    std::string::size_type slash = fileName.find_last_of(PATH_SEPARATORS);
    if (slash==std::string::npos) return std::string();
    else return std::string(fileName, 0, slash);
}

#ifdef WIN32
	std::string strerror_ext()
	{
		char buff[256];
		strerror_s(buff, 256, errno);
		return std::string(buff);
	}
#else
	std::string strerror_ext()
	{
		return std::string(strerror(errno));
	}
#endif

bool osgDB::makeDirectory( const std::string &path )
{
    if (path.empty())
    {
		std::cout << "osgDB::makeDirectory(): cannot create an empty directory" << std::endl;
        return false;
    }

    struct stat64 stbuf;
    if( stat64( path.c_str(), &stbuf ) == 0 )
    {
        if( S_ISDIR(stbuf.st_mode))
            return true;
        else
        {
            std::cout << "osgDB::makeDirectory(): "  <<
                    path << " already exists and is not a directory!" << std::endl;
            return false;
        }
    }

    std::string dir = path;
    std::stack<std::string> paths;
    while( true )
    {
        if( dir.empty() )
            break;

        if( stat64( dir.c_str(), &stbuf ) < 0 )
        {
            switch( errno )
            {
                case ENOENT:
                case ENOTDIR:
                    paths.push( dir );
                    break;

                default:
                    std::cout << "osgDB::makeDirectory(): "  << strerror_ext() << std::endl;
                    return false;
            }
        }
        dir = getFilePath(std::string(dir));
    }

    while( !paths.empty() )
    {
        std::string dir = paths.top();

        #if defined(WIN32)
            //catch drive name
            if (dir.size() == 2 && dir.c_str()[1] == ':') {
                paths.pop();
                continue;
            }
        #endif

        if( mkdir( dir.c_str(), 0755 )< 0 )
        {
            std::cout << "osgDB::makeDirectory(): "  << strerror_ext() << std::endl;
            return false;
        }
        paths.pop();
    }
    return true;
}

osgDB::FileType osgDB::fileType(const std::string& filename)
{
    struct stat64 fileStat;
    if ( stat64(filename.c_str(), &fileStat) != 0 )
    {
        return FILE_NOT_FOUND;
    } // end if

    if ( fileStat.st_mode & S_IFDIR )
        return DIRECTORY;
    else if ( fileStat.st_mode & S_IFREG )
        return REGULAR_FILE;

    return FILE_NOT_FOUND;
}

#if defined(WIN32) && !defined(__CYGWIN__)
    osgDB::DirectoryContents osgDB::getDirectoryContents(const std::string& dirName)
    {
        osgDB::DirectoryContents contents;

        OSGDB_WINDOWS_FUNCT(WIN32_FIND_DATA) data;
        HANDLE handle = OSGDB_WINDOWS_FUNCT(FindFirstFile)((OSGDB_STRING_TO_FILENAME(dirName) + OSGDB_FILENAME_TEXT("\\*")).c_str(), &data);
        if (handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                contents.push_back(OSGDB_FILENAME_TO_STRING(data.cFileName));
            }
            while (OSGDB_WINDOWS_FUNCT(FindNextFile)(handle, &data) != 0);

            FindClose(handle);
        }
        return contents;
    }

#else

    #include <dirent.h>
    osgDB::DirectoryContents osgDB::getDirectoryContents(const std::string& dirName)
    {
        osgDB::DirectoryContents contents;

        DIR *handle = opendir(dirName.c_str());
        if (handle)
        {
            dirent *rc;
            while((rc = readdir(handle))!=NULL)
            {
                contents.push_back(rc->d_name);
            }
            closedir(handle);
        }

        return contents;
    }

#endif // unix getDirectoryContents
