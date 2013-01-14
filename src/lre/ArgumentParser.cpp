/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "ArgumentParser.h"

#include "FileUtil.h"

//-- STL --//
#include <iostream>

namespace lre {

	//=======================================================================================
	ArgumentParser::ArgumentParser(int argc, const char** argv): appUsage_(""), appName_("")
	{
		const char** tmp = argv;
		arguments_.clear();
		for (int i = 0; i < argc; ++i) {
			arguments_.push_back(std::string(*tmp));
			tmp++;
		}
	}

	//=======================================================================================
	ArgumentParser::ArgumentParser(int argc, char** argv): appUsage_(""), appName_("")
	{
		char** tmp = argv;
		arguments_.clear();
		for (int i = 0; i < argc; ++i) {
			arguments_.push_back(std::string(*tmp));
			tmp++;
		}
	}

	//=======================================================================================
	ArgumentParser::~ArgumentParser()
	{
		
	}
	
	//=======================================================================================
	bool ArgumentParser::read(const std::string& optionName, std::string& value)
	{
		for (StringDeque::iterator itr = arguments_.begin(); itr != arguments_.end(); ++itr) {
			if (*itr == optionName) {
				++itr;
				if (itr != arguments_.end() && !isOption(*itr)) {
					value = *itr;
					return true;
				}
			}
		}
		return false;
	}

	//=======================================================================================
	bool ArgumentParser::isSet(const std::string& optionName)
	{
		for (StringDeque::iterator itr = arguments_.begin(); itr != arguments_.end(); ++itr) {
			if (*itr == optionName) { return true; }
		}
		return false;
	}

	//=======================================================================================
	bool ArgumentParser::isOption(const std::string& optionName)
	{
		if (optionName.substr(0, 1) == "-") { return true; }
		return false;
	}

	//=======================================================================================
	void ArgumentParser::addCommandLineOption(const std::string& argument, const std::string& description)
	{
		options_.insert(StringStringPair(argument, description));
	}

	//=======================================================================================
	void ArgumentParser::reportOptions()
	{
		if (appName_ != "") { std::cout<<appName_<<std::endl; }
		if (appUsage_ != "") { std::cout<<"Usage: "<<lre::FileUtil::extractFilename(arguments_.at(0))<<" "<<appUsage_<<std::endl; }
		for (StringStringMap::iterator itr = options_.begin(); itr != options_.end(); ++itr) {
			std::cout<<itr->first<<":"<<std::endl;
			std::cout<<itr->second<<std::endl<<std::endl;
		}
	}

} // namespace lre
