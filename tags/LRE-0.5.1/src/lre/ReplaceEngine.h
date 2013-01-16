/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef lre_ReplaceEngine_H
#define lre_ReplaceEngine_H 1

#include "Export.h"

#include "Component.h"

//-- STL --//
#include <string>
#include <vector>

#ifdef WIN32
	// Disable DLL Export warning on ComponentList componentList_
	#pragma warning (disable: 4251)
#endif

namespace lre {

	class LRE_EXPORT ReplaceEngine {
		public:
			// Default constructor
			ReplaceEngine();
			// Virtual destructor
			virtual ~ReplaceEngine();

			// Initialize from command line arguments (optionally). ReplaceEngine uses default settings if not called.
			int init(int argc, char** argv);
			int init(int argc, const char** argv);

			// Invoke a ReplaceEngine run
			// @return 0 if succeeded. Error codes are >=1
			int run();

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

			// Creates a new empty Component and returns a reference to it for modification
			// Note that the name is case sensitive.
			// @return The newly created component
			Component* addComponent(const std::string& name);

			// Public accessor for number of components
			// @return Number of components defined during runtime by calling addComponent(name)
			unsigned int getNumComponents() const { return componentList_.size(); }

			// Get a component by its name.
			// @return Component* if found, otherwise NULL
			Component* getComponent(const std::string& name);

			// Generates one file per call
			// @return true on success, otherwise false
			bool processFile(const std::string& source_filename, const std::string& target_filename);

			// Generates output filename based on current settings
			// @return the target filename string including path
			std::string makeTargetFilename(const std::string& source_filename) const;

			// Store ComponentList in a file
			// @return true on sucess, otherwise false
			bool saveData(const std::string& filename) const;

			// Read ComponentList from file
			// @return true on sucess, otherwise false
			bool loadData(const std::string& filename);

		private:
			void reportSetup();
			void reportData();

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
			
			// The list of defined component by using addComponent(name)
			ComponentList componentList_;
	};

} // namespace lre

#endif // lre_ReplaceEngine_H
