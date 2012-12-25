/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
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

			// Initialize from command line arguments (optionally). ReplaceEngine uses default if not called.
			int init(int argc, char** argv);

			// Invoke a ReplaceEngine run
			// @return 0 if succeeded. Error codes are >=1
			int run();

			// Sets the input path or filename
			// @param data Path to a directory including multiple files or one file for processing
			void setInput(const std::string& data) { inputPath_ = data; }

			// Sets the output path
			// @param path Path to a directory for writing the resulting files. 
			void setOutputDirectory(const std::string& path) { outputPath_ = path; }

			// Sets the input path recursive mode. Only valid if setInput is set to a path. Default is false.
			// @param recurse Enable recursive handling of any files matching the extension inside the input directory
			void setRecursive(bool recurse) { recursive_ = recurse; }

			// Keep directory structure from input path. Default is true.
			// @param keepStructure Keep subfolder structure in output directory
			void setKeepSubFolders(bool keepStructure) { keepStructure_ = keepStructure; }

			// Sets the file pattern to process if setInput is set to a path. Default is '*.in'. Wildcards (* and ?) are allowed.
			// @param pattern The file pattern (e.g. extension) to be processed
			void setFilePattern(const std::string& pattern) { pattern_ = pattern; }

			// Define whether the extension should be removed from the target filenames
			// Default is true. Usually you will enable this in your files are in format 'myfile.cpp.in'
			// and you wish to generate filenames in format 'myfile.cpp'
			void setRemoveExtension(bool remove) { removeExtension_ = remove; }

			// Creates a new empty Component and returns a reference to it for modification
			// Note that the name is case sensitive.
			// @return The newly created component
			Component* addComponent(const std::string& name);

			// Generates one file per call
			// @return true on success, otherwise false
			bool processFile(const std::string& source_filename, const std::string& target_filename);

			// Generates output filename based on current settings
			// @return the target filename string including path
			std::string makeTargetFilename(const std::string& source_filename);

		private:
			void reportSetup();

			bool recursive_;
			bool keepStructure_;
			bool removeExtension_;
			std::string inputPath_;
			std::string outputPath_;
			std::string pattern_;

			// Get a component by its name.
			// @return Component* if found, otherwise NULL
			Component* getComponent(const std::string& name);
			
			// The list of defined component by using addComponent(name)
			ComponentList componentList_;
	};

} // namespace lre

#endif // lre_ReplaceEngine_H
