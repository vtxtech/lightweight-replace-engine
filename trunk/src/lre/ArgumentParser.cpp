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
		for (int i = 0; i < argc; ++i) {
			arguments_.push_back(Argument(std::string(*tmp)));
			tmp++;
		}
	}

	//=======================================================================================
	ArgumentParser::ArgumentParser(int argc, char** argv): appUsage_(""), appName_("")
	{
		char** tmp = argv;
		arguments_.clear();
		for (int i = 0; i < argc; ++i) {
			arguments_.push_back(Argument(std::string(*tmp)));
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
			if (arguments_.at(i).arg == optionName) {
				arguments_.at(i).used = true;
				++i;
				if (i < arguments_.size() && !isOption(arguments_.at(i).arg)) {
					arguments_.at(i).used = true;
					value = arguments_.at(i).arg;
					return true;
				} else {
					arguments_.at(i-1).missingParameter = true;
				}
			}
		}
		return false;
	}

	//=======================================================================================
	bool ArgumentParser::isSet(const std::string& optionName)
	{
		for (ArgumentDeque::iterator itr = arguments_.begin(); itr != arguments_.end(); ++itr) {
			if (itr->arg == optionName) {
				itr->used = true;
				return true;
			}
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

	enum FillSide {
		Left,
		Right
	};

	std::string fill(const std::string& input, int targetWidth, FillSide side)
	{
		std::string output = input;
		while (output.size() < targetWidth) {
			if (side == Left) {
				output = " " + output;
			} else {
				output += " ";
			}
		}
		return output;
	}

	//=======================================================================================
	void ArgumentParser::reportOptions()
	{
		if (appName_ != "") { std::cout<<appName_<<std::endl; }
		if (appUsage_ != "") { std::cout<<"Usage: "<<lre::FileUtils::extractFilename(arguments_.at(0).arg)<<" "<<appUsage_<<std::endl<<std::endl; }

		int terminalWidth = 80;
		int leftColumnWidth = 5;
		int rightColumnWidth = terminalWidth - leftColumnWidth;

		for (StringStringMap::iterator itr = options_.begin(); itr != options_.end(); ++itr) {
			// print option, e.g. --input <foo>
			std::string param = fill(itr->first + ": ", leftColumnWidth, Right);
			std::cout<<param;
			// fill the left column of the next line after the option
			// with spaces, so the description does not have to take
			// care of whether it's in the same line with the option
			// or in the next line
			if (param.size() > leftColumnWidth) {
				std::cout<<std::endl<<std::endl<<fill("", leftColumnWidth, Left);
			}
			// now print the description
			std::string descr = itr->second;
			if (descr.size() > rightColumnWidth) {
				// find out what fits into the right column
				std::string out = descr.substr(0, rightColumnWidth);
				// well, now find the last space so we do not break within a word
				std::string::size_type p = out.find_last_of(" ");
				std::cout<<fill(out.substr(0, p), terminalWidth-leftColumnWidth, Right);
				// skip the space so we do not start our next line with it
				p++;
				// descr now holds only the rest of the text that isn't
				// printed, yet.
				descr = descr.substr(p, descr.size() - p);
				// look until we have only the final line left
				while (descr.size() > rightColumnWidth) {
					// again, find out what fits into the right column
					out = descr.substr(0, rightColumnWidth);
					// find the last space ...
					p = out.find_last_of(" ");
					out = out.substr(0, p);
					// ... and print it!
					std::cout<<fill(fill(out, leftColumnWidth+out.size(), Left), terminalWidth, Right);
					// skip the space
					p++;
					descr = descr.substr(p, descr.size() - p);
				}
				// ok, finally print out the last line
				int lastLineSize = leftColumnWidth+descr.size();
				std::cout<<fill(descr.substr(0, rightColumnWidth), lastLineSize, Left);
				if (lastLineSize != terminalWidth) { std::cout<<std::endl<<std::endl; }
			} else {
				std::cout<<descr<<std::endl<<std::endl;
			}
		}
	}

	//=======================================================================================
	int ArgumentParser::reportUnusedArguments()
	{
		int failedCount = 0;
		for (unsigned int i = 1; i < arguments_.size(); ++i) {
			if (!arguments_.at(i).used) {
				std::cout<<"Invalid option '"<<arguments_.at(i).arg<<"'."<<std::endl;
				failedCount++;
			}
			if (arguments_.at(i).missingParameter) {
				std::cout<<"Option '"<<arguments_.at(i).arg<<"' missing a parameter."<<std::endl;
				failedCount++;
			}
		}
		return failedCount;
	}

} // namespace lre
