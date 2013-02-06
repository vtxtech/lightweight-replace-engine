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
#include "Notify.h"
#include "Settings.h"

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
			// Initialize from const command line arguments (optionally). ReplaceEngine uses default settings if not called.
			int init(int argc, const char** argv);

			enum RunReturnCodes {
				FINISHED_SUCCESSFULLY = 0,
				INVALID_SETTINGS = 1,
				FAILED_TO_LOAD_DATAFILE = 2,
				FAILED_TO_PROCESS_FILE = 3
			};

			// Invoke a ReplaceEngine run
			// @return 0 if succeeded. Error codes are >=1
			int run();

			// Creates a new empty Component and returns a reference to it for modification
			// Note that the name is case sensitive.
			// @return The newly created component
			Component* addComponent(const std::string& name);

			// Public accessor for number of components
			// @return Number of components defined during runtime by calling addComponent(name)
			lre::ComponentList::size_type getNumComponents() const { return componentList_.size(); }

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

			// Getter for lre::Settings object
			// @return non-const Settings& object reference
			Settings& settings() { return settings_; }

			// Getter for lre::Settings object
			// @return non-const Settings& object reference
			const Settings& settings() const { return settings_; }

		private:
			void reportData();
			
			// The list of defined component by using addComponent(name)
			ComponentList componentList_;

			// LRE settings defined from command arguments our from source
			Settings settings_;
	};

} // namespace lre

#endif // lre_ReplaceEngine_H
