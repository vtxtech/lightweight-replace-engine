/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "FileUtils.h"

//-- OSG --//
#include "extern/FileUtils.h"

//-- STL --//
#include <fstream>
#include <iostream>
#include <algorithm>

#if defined(WIN32) && !defined(__CYGWIN__)
#include <io.h>
#endif

namespace lre {

	static const char * const SEPARATORS = "\\/";

	//=======================================================================================
	// from http://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
	// if (wildcmp("bl?h.*", "blah.jpg")) { MATCH } else { NO_MATCH };
	int wildcmp(const char *wild, const char *string) {
	  // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
	  const char *cp = NULL, *mp = NULL;

	  while ((*string) && (*wild != '*')) {
		if ((*wild != *string) && (*wild != '?')) {
		  return 0;
		}
		wild++;
		string++;
	  }

	  while (*string) {
		if (*wild == '*') {
		  if (!*++wild) {
			return 1;
		  }
		  mp = wild;
		  cp = string+1;
		} else if ((*wild == *string) || (*wild == '?')) {
		  wild++;
		  string++;
		} else {
		  wild = mp;
		  string = cp++;
		}
	  }

	  while (*wild == '*') {
		wild++;
	  }
	  return !*wild;
	}

	//=======================================================================================
	bool FileUtils::makeDirectory(const std::string& path)
	{
		return osgDB::makeDirectory(path);
	}

	//=======================================================================================
	std::string FileUtils::getFile(const std::string& filename)
	{
		std::ifstream in(filename.c_str());
		in.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
		return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
	}

	//=======================================================================================
	bool FileUtils::putFile(const std::string& filename, const std::string& text)
	{
		std::ofstream of;
		of.open(filename.c_str());
		if (!of.is_open()) {
			return false;
		}
		of<<text;
		of.close();
		return true;
	}

	//=======================================================================================
	std::string FileUtils::separator()
	{
#if defined(WIN32) && !defined(__CYGWIN__)
    return "\\";
#else
    return "/";
#endif
	}

	//=======================================================================================
	std::string FileUtils::getNativeEndline()
	{
#if defined( __APPLE__ )
		return "\r";
#else // this is understood on Windows, UNIX'es and LINUX'es 
		return "\n";
#endif
	}

	bool FileUtils::fileExists(const std::string& filename)
	{
#ifdef WIN32
		return _access( filename.c_str(), 4 ) == 0;
#else
		return access( filename.c_str(), F_OK ) == 0;
#endif
	}

	//=======================================================================================
	std::vector<std::string> FileUtils::findFiles(const std::string& path, const std::string& extension, bool recursive)
	{
		std::vector<std::string> files; files.clear();

		// Check if we are in input-file or input-directory mode
		if (osgDB::fileType(path) == osgDB::REGULAR_FILE) {
			files.push_back(path);
			return files;
		} else if (osgDB::fileType(path) == osgDB::FILE_NOT_FOUND) {
			// Return empty list
			return files;
		}

		std::vector<std::string> filesAndFolders = getDirectoryContent(path);
		for (unsigned int i = 0; i < filesAndFolders.size(); ++i) {
			std::string name = path+separator()+filesAndFolders.at(i);
			if (fileExists(name)) {
				if (osgDB::fileType(name) == osgDB::DIRECTORY) {
					if (recursive) {
#ifdef _DEBUG
						std::cout<<"Recursing DIR: "<<name<<std::endl;
#endif
						std::vector<std::string> subfolderFiles = findFiles(name, extension, recursive);
						for (unsigned int k = 0; k < subfolderFiles.size(); ++k) {
							files.push_back(subfolderFiles.at(k));
						}
					} else {
#ifdef _DEBUG
						std::cout<<"Skipping DIR: "<<name<<std::endl;
#endif
					}
				} else {
					files.push_back(name);
				}
			}
		}

		std::vector<std::string> filesMatchingPattern; filesMatchingPattern.clear();

		// Remove files not matching extension
		for (unsigned int i = 0; i < files.size(); ++i) {
			if (wildcmp(extension.c_str(), files.at(i).c_str())) {
				filesMatchingPattern.push_back(files.at(i));
			}
		}

		return filesMatchingPattern;
	}

	//=======================================================================================
	std::string FileUtils::removeExtension( const std::string& filename )
	{
		std::string::size_type dot_pos = filename.find_last_of(".");
		if (dot_pos == std::string::npos) {
			return filename;
		} else {
			return std::string(filename, 0, dot_pos); 
		}
	}

	//=======================================================================================
	std::string FileUtils::extractFilename( const std::string& filename )
		{
		std::string::size_type pos = filename.find_last_of(SEPARATORS);
		if (pos == std::string::npos) {
			return filename;
		} else {
			return std::string(filename.begin()+pos+1, filename.end());
		}
	}

	//=======================================================================================
	std::string FileUtils::extractDirectory( const std::string& filename )
	{
		std::string::size_type pos = filename.find_last_of(SEPARATORS);
		if (pos == std::string::npos) {
			return std::string();
		} else {
			return std::string(filename, 0, pos);
		}
	}

	//=======================================================================================
	std::string FileUtils::excludeTrailingSeparator( const std::string& filename )
	{
		if (filename == "") { return filename; }

		if (filename.at(filename.size()-1) == '\\' || filename.at(filename.size()-1) == '/') {
			return filename.substr(0, filename.size()-1);
		}
		return filename;
	}
	
	//=======================================================================================
	std::string FileUtils::includeTrailingSeparator( const std::string& filename )
	{
		return excludeTrailingSeparator(filename)+separator();
	}

	//=======================================================================================
	std::vector<std::string> FileUtils::getDirectoryContent(const std::string& path)
	{
		std::vector<std::string> content = osgDB::getDirectoryContents(path);

		// Remove . and ..
		std::vector<std::string>::iterator itr = std::find(content.begin(), content.end(), ".");
		if (itr != content.end()) content.erase(itr);
		itr = std::find(content.begin(), content.end(), "..");
		if (itr != content.end()) content.erase(itr);

		return content;
	}

} // namespace lre

