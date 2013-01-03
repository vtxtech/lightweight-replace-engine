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

// handle TCHAR type on various platforms
// #ifndef is inspired by https://svn.apache.org/repos/asf/logging/log4cxx/tags/v0_9_4/include/log4cxx/helpers/tchar.h
// defining type as plain char is from unzip.h, line 64

#ifndef TCHAR
typedef char TCHAR;
#endif


// currently this impl is for _all_ platforms, except as defined.
// the mac version will change soon to reflect the path scheme under osx, but
// for now, the above include is commented out, and the below code takes precedence.

#if defined(WIN32) && !defined(__CYGWIN__)
    #include <io.h>
    #define WINBASE_DECLARE_GET_MODULE_HANDLE_EX
    #include <windows.h>
    #include <winbase.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <direct.h> // for _mkdir

    #define mkdir(x,y) _mkdir((x))
    #define stat64 _stati64

    // set up for windows so acts just like unix access().
#ifndef F_OK
    #define F_OK 4
#endif

#else // unix

#if defined( __APPLE__ )
    // I'm not sure how we would handle this in raw Darwin
    // without the AvailablilityMacros.
    #include <AvailabilityMacros.h>

    //>OSG_IOS
    //IOS includes
    #include "TargetConditionals.h"

    #if (TARGET_OS_IPHONE)
        #include <Availability.h>
        // workaround a bug which appears when compiling for SDK < 4.0 and for the simulator
        #ifdef __IPHONE_4_0 && (__IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_4_0)
            #define stat64 stat
        #else
            #if !TARGET_IPHONE_SIMULATOR
                #define stat64 stat
            #endif
        #endif
    #endif
    //<OSG_IPHONE

    // 10.5 defines stat64 so we can't use this #define
    // By default, MAC_OS_X_VERSION_MAX_ALLOWED is set to the latest
    // system the headers know about. So I will use this as the control
    // variable. (MIN_ALLOWED is set low by default so it is
    // unhelpful in this case.)
    // Unfortunately, we can't use the label MAC_OS_X_VERSION_10_4
    // for older OS's like Jaguar, Panther since they are not defined,
    // so I am going to hardcode the number.
    #if (MAC_OS_X_VERSION_MAX_ALLOWED <= 1040)
        #define stat64 stat
    #endif
#elif defined(__CYGWIN__) || defined(__FreeBSD__) || (defined(__hpux) && !defined(_LARGEFILE64_SOURCE))
    #define stat64 stat
#endif

    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

    // set up _S_ISDIR()
#if !defined(S_ISDIR)
#  if defined( _S_IFDIR) && !defined( __S_IFDIR)
#    define __S_IFDIR _S_IFDIR
#  endif
#  define S_ISDIR(mode)    (mode&__S_IFDIR)
#endif

#include "FileUtils.h"
#include "FileNameUtils.h"

#include <errno.h>
#include <string.h>

#include <stack>
#include <iostream>

namespace osgDB
{
#ifdef OSG_USE_UTF8_FILENAME
#define OSGDB_STRING_TO_FILENAME(s) osgDB::convertUTF8toUTF16(s)
#define OSGDB_FILENAME_TO_STRING(s) osgDB::convertUTF16toUTF8(s)
#define OSGDB_FILENAME_TEXT(x) L ## x
#define OSGDB_WINDOWS_FUNCT(x) x ## W
#define OSGDB_WINDOWS_FUNCT_STRING(x) #x "W"
typedef wchar_t filenamechar;
typedef std::wstring filenamestring;
#else
#define OSGDB_STRING_TO_FILENAME(s) s
#define OSGDB_FILENAME_TO_STRING(s) s
#define OSGDB_FILENAME_TEXT(x) x
#define OSGDB_WINDOWS_FUNCT(x) x ## A
#define OSGDB_WINDOWS_FUNCT_STRING(x) #x "A"
typedef char filenamechar;
typedef std::string filenamestring;
#endif
}

bool osgDB::makeDirectory( const std::string &path )
{
    if (path.empty())
    {
		std::cout << "osgDB::makeDirectory(): cannot create an empty directory" << std::endl;
        return false;
    }

    struct stat64 stbuf;
#ifdef OSG_USE_UTF8_FILENAME
    if( _wstat64( OSGDB_STRING_TO_FILENAME(path).c_str(), &stbuf ) == 0 )
#else
    if( stat64( path.c_str(), &stbuf ) == 0 )
#endif
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

#ifdef OSG_USE_UTF8_FILENAME
        if( _wstat64( OSGDB_STRING_TO_FILENAME(dir).c_str(), &stbuf ) < 0 )
#else
        if( stat64( dir.c_str(), &stbuf ) < 0 )
#endif
        {
            switch( errno )
            {
                case ENOENT:
                case ENOTDIR:
                    paths.push( dir );
                    break;

                default:
                    std::cout << "osgDB::makeDirectory(): "  << strerror(errno) << std::endl;
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

#ifdef OSG_USE_UTF8_FILENAME
        if ( _wmkdir(OSGDB_STRING_TO_FILENAME(dir).c_str())< 0 )
#else
        if( mkdir( dir.c_str(), 0755 )< 0 )
#endif
        {
            std::cout << "osgDB::makeDirectory(): "  << strerror(errno) << std::endl;
            return false;
        }
        paths.pop();
    }
    return true;
}

bool osgDB::fileExists(const std::string& filename)
{
#ifdef OSG_USE_UTF8_FILENAME
    return _waccess( OSGDB_STRING_TO_FILENAME(filename).c_str(), F_OK ) == 0;
#else
#ifdef WIN32
    return _access( filename.c_str(), F_OK ) == 0;
#else
	return access( filename.c_str(), F_OK ) == 0;
#endif
#endif
}

osgDB::FileType osgDB::fileType(const std::string& filename)
{
    struct stat64 fileStat;
#ifdef OSG_USE_UTF8_FILENAME
    if ( _wstat64(OSGDB_STRING_TO_FILENAME(filename).c_str(), &fileStat) != 0 )
#else
    if ( stat64(filename.c_str(), &fileStat) != 0 )
#endif
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
    #include <io.h>
    #include <direct.h>

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

#endif // unix getDirectoryContexts
