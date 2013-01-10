/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief LRE's ArgumentParser inspired by OpenSceneGraph by Robert Osfield
 */

#ifndef lre_ArgumentParser_H
#define lre_ArgumentParser_H 1

#include "Export.h"

//-- STL --//
#include <string>
#include <deque>
#include <map>

#ifdef WIN32
	// Disable DLL Export warning on StringDeque arguments_;
	#pragma warning (disable: 4251)
#endif

namespace lre {

	typedef std::deque<std::string> StringDeque;
	typedef std::map<std::string, std::string> StringStringMap;
	typedef std::pair<std::string, std::string> StringStringPair;

	// A lightweight command line argument parser
	class LRE_EXPORT ArgumentParser {
		public:
			// Parameter constructor
			ArgumentParser(int argc, char** argv);
			// Virtual destructor
			virtual ~ArgumentParser();

			// Set the application name to be reported by reportOptions()
			// @param name e.g. "My lovely application"
			void setApplicationName(const std::string& name) { appName_ = name; }

			// Set the application usage string to be reported by reportOptions()
			// @param usage e.g. "[options] filename"
			void setApplicationUsage(const std::string& usage) { appUsage_ = usage; }

			// Adds a command line option to be reported by reportOptions()
			// @param argument The argument the user may set (e.g. --override-on")
			// @param description The description of the parameter
			void addCommandLineOption(const std::string& argument, const std::string& description);

			// Report the command line options added using addCommandLineOption
			// on the command line (std::cout)
			void reportOptions();

			// Read the parameter to string
			// @return false if not set or no parameter following. True on success
			bool read(const std::string& optionName, std::string& value);

			// Checks if the parameter is set in the command line
			// @return true if set, otherwise false
			bool isSet(const std::string& optionName);

			// Check if the parameter string is an option.
			// @return true if the string begins with - or --
			bool isOption(const std::string& optionName);

			// Returns the number of arguments given, actually a copy of argc
			// @return Number of arguments given in commandline.
			int getArgumentCount() { return arguments_.size(); }

		private:
			StringDeque arguments_;
			StringStringMap options_;
			std::string appName_;
			std::string appUsage_;
	};

} // namespace lre

#endif // lre_ReplaceEngine_H
