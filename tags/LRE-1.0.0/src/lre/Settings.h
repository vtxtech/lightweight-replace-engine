/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef lre_Settings_H
#define lre_Settings_H 1

#include "Export.h"

#include "Notify.h"

//-- STL --//
#include <string>

namespace lre {

	class LRE_EXPORT Settings {
		public:
			// Default constructor
			Settings();
			// Virtual destructor
			virtual ~Settings();

			// Initialize from command line arguments (optionally). Settings uses default settings if not called.
			int init(int argc, const char** argv);

			// Sets the input path or filename
			// @param data Path to a directory including multiple files or one file for processing
			void setInput(const std::string& data) { inputPath_ = data; }

			// Getter for input path
			// @return Path to input file or input directory
			std::string getInput() const { return inputPath_; }

			// Sets the output path
			// @param path Path to a directory for writing the resulting files. 
			void setOutputDirectory(const std::string& path) { outputPath_ = path; }

			// Getter for output directory
			// @return Path to output directory
			std::string getOutputDirectory() const { return outputPath_; }

			// Sets the data path. Only required if no lre::Components are defined inside your source code
			// @param path Path to a directory where out-of-source data is defined
			void setDataDirectory(const std::string& path) { dataPath_ = path; }

			// Getter for data directory
			// @return Path to data file or data directory
			std::string getDataDirectory() const { return dataPath_; }

			// Sets the data file pattern. Only files matching the pattern are parsed to read replacement data
			// Wildcards (* and ?) are allowed. Only required if no lre::Components are defined inside your source code
			// @param pattern The file pattern (e.g. extension) to be processed. Default is '*.lre'
			void setDataPattern(const std::string& pattern) { dataPattern_ = pattern; }

			// Getter for data pattern
			// @return Data files' pattern
			std::string getDataPattern() const { return dataPattern_; }

			// Sets the input path recursive mode. Only valid if setInput is set to a path. Default is false.
			// @param recurse Enable recursive handling of any files matching the extension inside the input directory
			void setRecursive(bool recurse) { recursive_ = recurse; }

			// Getter for recursive input directory handling
			// @return true, if recursive mode is enabled, otherwise false
			bool getRecursive() const { return recursive_; }

			// Keep directory structure from input path. Default is true.
			// @param keepStructure Keep subfolder structure in output directory
			void setKeepSubFolders(bool keepStructure) { keepStructure_ = keepStructure; }

			// Getter for keep input path structure
			// @return true, if keep mode is enabled, otherwise false.
			bool getKeepSubFolders() const { return keepStructure_; }

			// Sets the file pattern to process if setInput is set to a path. Wildcards (* and ?) are allowed.
			// @param pattern The file pattern (e.g. extension) to be processed. Default is '*.in'
			void setFilePattern(const std::string& pattern) { pattern_ = pattern; }

			// Getter for input path file search pattern
			// @return Input path file pattern
			std::string getFilePattern() const { return pattern_; }

			// Define whether the extension should be removed from the target filenames
			// Default is true. Usually you will enable this in your files are in format 'myfile.cpp.in'
			// and you wish to generate filenames in format 'myfile.cpp'
			void setRemoveExtension(bool remove) { removeExtension_ = remove; }

			// Getter for remove extension behavior
			// @return true, if extension will be removed. False if extension is kept.
			bool getRemoveExtension() const { return removeExtension_; }

			// Define whether a specific string (e.g. line break) should be appended after each generated set
			// Empty by default.
			// @param The string that will be appended
			void setAppendixString(const std::string& appendix);

			// Getter for appendix string
			// @return The appendix string
			std::string getAppendixString() const { return appendix_; }

			// Define whether after the last set generated the appendix should be added.
			// @param state Set to true, if desired. True by default.
			void setAddAppendixAfterLastSet(bool state) { appendixAfterLastSet_ = state; }

			// Getter for appendix after last set
			// @return true, if appendix is added after each set. false, if appendix is skipped after last set.
			bool getAddAppendixAfterLastSet() const { return appendixAfterLastSet_; }

			// Enable verbose (debug) output mode
			void setVerbose(bool state) { verbose_ = state; silent_ = false; lre::setNotifyLevel(lre::DEBUG); }

			// Getter for verbose (debug) output mode
			bool getVerbose() const { return verbose_; }

			// Enable silent mode (no standard output, just errors)
			void setSilent(bool state) { silent_ = state; verbose_ = false; lre::setNotifyLevel(lre::ALWAYS); }

			// Getter for verbose (debug) output mode
			bool getSilent() const { return silent_; }

			// Print the settings to lre::notify
			void reportSetup();
	
	private:

			bool verbose_;
			bool silent_;
			bool recursive_;
			bool keepStructure_;
			bool removeExtension_;
			bool appendixAfterLastSet_;
			std::string inputPath_;
			std::string outputPath_;
			std::string pattern_;
			std::string dataPath_;
			std::string dataPattern_;
			std::string appendix_;
	};

} // namespace lre

#endif // lre_Settings_H
