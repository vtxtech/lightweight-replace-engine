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

#include "FileUtils.h"

//-- STL --//
#include <iostream>

namespace lre {

	//=======================================================================================
	ArgumentParser::ArgumentParser(int argc, const char** argv): appUsage_(""), appName_("")
	{
		const char** tmp = argv;
		arguments_.clear();
		argumentUsed_.clear();
		argumentMissingParameter_.clear();
		for (int i = 0; i < argc; ++i) {
			arguments_.push_back(std::string(*tmp));
			argumentUsed_.push_back(false);
			argumentMissingParameter_.push_back(false);
			tmp++;
		}
	}

	//=======================================================================================
	ArgumentParser::ArgumentParser(int argc, char** argv): appUsage_(""), appName_("")
	{
		char** tmp = argv;
		arguments_.clear();
		argumentUsed_.clear();
		argumentMissingParameter_.clear();
		for (int i = 0; i < argc; ++i) {
			arguments_.push_back(std::string(*tmp));
			argumentUsed_.push_back(false);
			argumentMissingParameter_.push_back(false);
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
		for (unsigned int i = 0; i < arguments_.size(); ++i) {
			if (arguments_.at(i) == optionName) {
				argumentUsed_.at(i) = true;
				++i;
				if (i < arguments_.size() && !isOption(arguments_.at(i))) {
					argumentUsed_.at(i) = true;
					value = arguments_.at(i);
					return true;
				} else {
					argumentMissingParameter_.at(i-1) = true;
				}
			}
		}
		return false;
	}

	//=======================================================================================
	bool ArgumentParser::isSet(const std::string& optionName)
	{
		BoolDeque::iterator used = argumentUsed_.begin();
		for (StringDeque::iterator itr = arguments_.begin(); itr != arguments_.end(); ++itr) {
			if (*itr == optionName) {
				(*used) = true;
				return true;
			}
			++used;
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
		if (appUsage_ != "") { std::cout<<"Usage: "<<lre::FileUtils::extractFilename(arguments_.at(0))<<" "<<appUsage_<<std::endl; }

		for (StringStringMap::iterator itr = options_.begin(); itr != options_.end(); ++itr) {
			std::cout<<itr->first<<":"<<std::endl;
			std::cout<<itr->second<<std::endl<<std::endl;
		}
	}

	//=======================================================================================
	int ArgumentParser::reportUnusedArguments()
	{
		int failedCount = 0;
		for (unsigned int i = 1; i < argumentUsed_.size(); ++i) {
			if (!argumentUsed_.at(i)) {
				std::cout<<"Invalid option '"<<arguments_.at(i)<<"'."<<std::endl;
				failedCount++;
			}
		}
		for (unsigned int i = 1; i < argumentMissingParameter_.size(); ++i) {
			if (argumentMissingParameter_.at(i)) {
				std::cout<<"Option '"<<arguments_.at(i)<<"' missing a parameter."<<std::endl;
				failedCount++;
			}
		}
		return failedCount;
	}

} // namespace lre
