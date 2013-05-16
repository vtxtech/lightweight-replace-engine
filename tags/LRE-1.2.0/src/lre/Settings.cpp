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
#include "Notify.h"

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
		appendix_(""), appendixAfterLastSet_(true), verbose_(false),
		silent_(false), copyPattern_(""), copyFilesExcludingInputPattern_(true)
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
		ap.addCommandLineOption("--copyPattern <file_pattern>", "Sets the copy file pattern. Default is empty (disabled). If set, matching files from --input directory are copied to the appropriate --output location.");
		ap.addCommandLineOption("--copyAll or -C", "Tells copy procedure to copy all files matching --dataPattern. Otherwise, by default, files matching --pattern are excluded.");
		ap.addCommandLineOption("--appendix <string>", "Define a string (e.g. line break) that shall be appended after each generated set");
		ap.addCommandLineOption("--noFinalAppendix", "Disable appendix string for the last set of a lre::Component");
		ap.addCommandLineOption("--verbose or -V", "Enable verbose debug output");
		ap.addCommandLineOption("--silent or -S", "Enable silent mode (no standard output, just errors). Overrides and disables verbose mode.");

		if (ap.isSet("--help") || ap.isSet("-h") || ap.isSet("/?") || ap.isSet("-?")) {
			ap.reportOptions();
			return 1;
		}

		if (ap.isSet("--recursive") || ap.isSet("-R")) {
			recursive_ = true;
		}
		
		if (ap.isSet("--verbose") || ap.isSet("-V")) {
			setVerbose(true);
		}
		
		// Silent mode must be handled after verbose mode because silent mode
		// overrides and disables verbose mode
		if (ap.isSet("--silent") || ap.isSet("-S")) {
			setSilent(true);
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

		if (ap.isSet("--forgetSubfolders") || ap.isSet("-F")) {
			keepStructure_ = false;
		}

		if (ap.isSet("--copyAll") || ap.isSet("-C")) {
			copyFilesExcludingInputPattern_ = false;
		}

		ap.read("--pattern", pattern_);
		ap.read("--input", inputPath_);
		ap.read("--output", outputPath_);
		ap.read("--data", dataPath_);
		ap.read("--dataPattern", dataPattern_);
		ap.read("--copyPattern", copyPattern_);
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
		lre::notify(lre::NOTICE)<<"--- Lightweight Replace Engine: Setup ---"<<std::endl;
		lre::notify(lre::NOTICE)<<"Recurse directory: "<<recursive_<<std::endl;
		lre::notify(lre::NOTICE)<<"Remove extensions: "<<removeExtension_<<std::endl;
		lre::notify(lre::NOTICE)<<"Input path: "<<inputPath_<<std::endl;
		lre::notify(lre::NOTICE)<<"Input file pattern: "<<pattern_<<std::endl;
		lre::notify(lre::NOTICE)<<"Output directory: "<<outputPath_<<std::endl;
		lre::notify(lre::NOTICE)<<"Data path: "<<dataPath_<<std::endl;
		lre::notify(lre::NOTICE)<<"Data file pattern: "<<dataPattern_<<std::endl;
		lre::notify(lre::NOTICE)<<"Copy file pattern: "<<copyPattern_<<std::endl;
		lre::notify(lre::NOTICE)<<"Copy files excluding input pattern: "<<copyFilesExcludingInputPattern_<<std::endl;
		lre::notify(lre::NOTICE)<<"Keep directory structure: "<<keepStructure_<<std::endl;
		lre::notify(lre::NOTICE)<<std::endl;
	}

} // namespace lre

