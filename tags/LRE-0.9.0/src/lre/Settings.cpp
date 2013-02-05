/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "Settings.h"

#include "ArgumentParser.h"
#include "Component.h"
#include "FileUtils.h"

//-- STL --//
#include <iostream>
#include <fstream>

//-- OSG --//
#include "extern/FileUtils.h"

namespace lre {

	//=======================================================================================
	Settings::Settings(): recursive_(false), inputPath_(""),
		outputPath_(""), pattern_("*.in"), removeExtension_(true),
		keepStructure_(true), dataPath_(""), dataPattern_("*.lre"),
		appendix_(""), appendixAfterLastSet_(true)
	{
	}

	//=======================================================================================
	Settings::~Settings()
	{
		// Nothing to be done.
	}
	
	//=======================================================================================
	int Settings::init(int argc, const char** argv)
	{
		lre::ArgumentParser ap(argc, argv);
		ap.setApplicationName("Lightweight Replace Engine by Johannes Scholz");
		ap.setApplicationUsage("[options]");

		ap.addCommandLineOption("--recursive or --R", "Enables input path recursive mode. Only valid if --input is set to a path. Disabled by default.");
		ap.addCommandLineOption("--keepExtension or -K", "Define whether the extension should be kept in the target filenames. Usually the last extension in the input file name is removed. Remove extensions only if your files are 'myfile.txt.in' format.");
		ap.addCommandLineOption("--pattern <file_pattern>", "Sets the file pattern to match if --input is set to a path. Default is '*.in'. Allowed wildcards: * and ?");
		ap.addCommandLineOption("--input <path or filename>", "Sets the input path or filename. Required option.");
		ap.addCommandLineOption("--output <path>", "Sets the output path. Required option.");
		ap.addCommandLineOption("--forgetSubfolders or -F", "Remove recursive directory structure in output directory. Disabled by default.");
		ap.addCommandLineOption("--data <path or filename>", "Data file containg Components or path to multiple files matching --dataPattern <file_pattern>. Empty by default. Required option, if no data is defined in source code, only.");
		ap.addCommandLineOption("--dataPattern <file_pattern>", "Sets the file pattern to match inside --data directory. Default is '*.lre'. Required option, if no data is defined in source code, only.");
		ap.addCommandLineOption("--appendix <string>", "Define a string (e.g. line break) that shall be appended after each generated set");
		ap.addCommandLineOption("--noFinalAppendix", "Disable appendix string for the last set of a lre::Component");

		if (ap.getArgumentCount() <= 1 || ap.isSet("--help") || ap.isSet("-h") || ap.isSet("/?") || ap.isSet("-?")) {
			ap.reportOptions();
			return 1;
		}
		
		if (ap.isSet("--recursive") || ap.isSet("-R")) {
			recursive_ = true;
		}

		if (ap.isSet("--forgetSubfolders") || ap.isSet("-F")) {
			keepStructure_ = false;
		}

		if (ap.isSet("--keepExtension") || ap.isSet("-K")) {
			removeExtension_ = false;
		}

		if (ap.isSet("--noFinalAppendix")) {
			appendixAfterLastSet_ = false;
		}

		ap.read("--pattern", pattern_);
		ap.read("--input", inputPath_);
		ap.read("--output", outputPath_);
		ap.read("--data", dataPath_);
		ap.read("--dataPattern", dataPattern_);
		ap.read("--appendix", appendix_);

		return ap.reportUnusedArguments();
	}

	//=======================================================================================
	void Settings::setAppendixString(const std::string& appendix)
	{
		appendix_ = appendix;
	}

	//=======================================================================================
	void Settings::reportSetup()
	{
		std::cout<<"--- Lightweight Replace Engine: Setup ---"<<std::endl;
		std::cout<<"Recurse directory: "<<recursive_<<std::endl;
		std::cout<<"Remove extensions: "<<removeExtension_<<std::endl;
		std::cout<<"Input path: "<<inputPath_<<std::endl;
		std::cout<<"Input file pattern: "<<pattern_<<std::endl;
		std::cout<<"Output directory: "<<outputPath_<<std::endl;
		std::cout<<"Data path: "<<dataPath_<<std::endl;
		std::cout<<"Data file pattern: "<<dataPattern_<<std::endl;
		std::cout<<"Keep directory structure: "<<keepStructure_<<std::endl;
		std::cout<<std::endl;
	}

} // namespace lre

