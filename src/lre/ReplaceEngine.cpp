/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "ReplaceEngine.h"

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
	ReplaceEngine::ReplaceEngine()
	{
		// Nothing to be done.
	}

	//=======================================================================================
	ReplaceEngine::~ReplaceEngine()
	{
		componentList_.clear();
	}
	
	//=======================================================================================
	int ReplaceEngine::init(int argc, char** argv)
	{
		// We do not need "char**", since nothing will be changed
		// so we cast to "const char**", so both may
		// be passed to our ReplaceEngine.
		return init(argc, const_cast<const char**>(argv));
	}

	//=======================================================================================
	int ReplaceEngine::init(int argc, const char** argv)
	{
		return settings_.init(argc, argv);
	}

	//=======================================================================================
	Component* ReplaceEngine::addComponent(const std::string& name)
	{
		componentList_.push_back(Component(name));
		return &(componentList_.at(componentList_.size()-1));
	}

	//=======================================================================================
	int ReplaceEngine::run()
	{
		// Check input data roughly
		if (settings().getInput() == "") { lre::notify(lre::ERROR)<<"FATAL: Input path is undefined"<<std::endl; return INVALID_SETTINGS; }
		if (settings().getOutputDirectory() == "") { lre::notify(lre::ERROR)<<"FATAL: Output path is undefined"<<std::endl; return INVALID_SETTINGS; }
		if (settings().getRemoveExtension() && settings().getFilePattern() == "") { lre::notify(lre::ERROR)<<"FATAL: Cannot remove extension without extension defined."<<std::endl; return INVALID_SETTINGS; }
		if (settings().getInput() == settings().getOutputDirectory() && !settings().getRemoveExtension()) { lre::notify(lre::ERROR)<<"FATAL: Input path and output path must not be equal if extensions are kept."<<std::endl; return INVALID_SETTINGS; }
		if (componentList_.size() == 0 && settings().getDataDirectory() == "") { lre::notify(lre::ERROR)<<"FATAL: No data defined. Please set either data path or add components in source."<<std::endl; return INVALID_SETTINGS; }

		settings().setOutputDirectory(FileUtils::excludeTrailingSeparator(settings().getOutputDirectory()));
		settings().setInput(FileUtils::excludeTrailingSeparator(settings().getInput()));
		settings().setDataDirectory(FileUtils::excludeTrailingSeparator(settings().getDataDirectory()));

		// Report input data to lre::notify()
		settings().reportSetup();

		// Backup current lre::Component data which is restored at the end of run()
		ComponentList tempCompList = componentList_;

		if (settings().getDataDirectory() != "") {
			// Build the list of lre::Components from settings().getDataDirectory()
			std::vector<std::string> dataFiles;
			dataFiles.clear();

			lre::notify(lre::DEBUG)<<"Data file lookup..."<<std::endl;
			dataFiles = FileUtils::findFiles(settings().getDataDirectory(), settings().getDataPattern(), false);

			lre::notify(lre::NOTICE)<<"--- Data files matching pattern ---"<<std::endl;
			for (unsigned int i = 0; i < dataFiles.size(); ++i) {
				lre::notify(lre::NOTICE)<<"FILE("<<i<<"): "<<dataFiles.at(i)<<"... "<<std::flush;
				if (loadData(dataFiles.at(i))) {
					lre::notify(lre::NOTICE)<<"Success."<<std::endl;
				} else {
					lre::notify(lre::ERROR)<<red<<"Failed."<<white<<std::endl;
					return FAILED_TO_LOAD_DATAFILE;
				}
			}
			lre::notify(lre::NOTICE)<<std::endl;
		}

		// Report data to lre::notify()
		reportData();

		// This is the list of files to be processed
		std::vector<std::string> files;
		files.clear();

		lre::notify(lre::DEBUG)<<"Source file lookup..."<<std::endl;
		files = FileUtils::findFiles(settings().getInput(), settings().getFilePattern(), settings().getRecursive());

		lre::notify(lre::DEBUG)<<std::endl;
		lre::notify(lre::DEBUG)<<"--- Processing files matching pattern ---"<<std::endl;
		for (unsigned int i = 0; i < files.size(); ++i) {
			std::string targetFile = makeTargetFilename(files.at(i));
			if (!processFile(files.at(i), targetFile)) {
				lre::notify(lre::ERROR)<<red<<"Failed processing '"<<files.at(i)<<"' to '"<<targetFile<<"'."<<std::endl<<"Aborting."<<white<<std::endl;
				return FAILED_TO_PROCESS_FILE;
			}
		}

		// Restore componentList_ from backup before file loading
		componentList_ = tempCompList;

		lre::notify(lre::NOTICE)<<green<<"Finished successfully."<<white<<std::endl;
		return FINISHED_SUCCESSFULLY;
	}

	//=======================================================================================
	std::string ReplaceEngine::makeTargetFilename(const std::string& source_filename) const
	{
		std::string result = source_filename;
		// Settings for consideration
		// * bool settings().getRemoveExtension();
		// * bool settings().getKeepSubFolders();
		// * std::string settings().getInput();
		// * std::string settings().getOutputDirectory();
		std::string outputPath = settings().getOutputDirectory();
		std::string inputPath = settings().getInput();
		if (osgDB::fileType(inputPath) == osgDB::REGULAR_FILE) {
			inputPath = FileUtils::extractDirectory(inputPath);
		}
		if (osgDB::fileType(outputPath) == osgDB::REGULAR_FILE) {
			outputPath = FileUtils::extractDirectory(outputPath);
		}
		lre::notify(lre::DEBUG)<<"outputPath="<<outputPath<<std::endl;
		lre::notify(lre::DEBUG)<<"inputPath="<<inputPath<<std::endl;
		lre::notify(lre::DEBUG)<<"source_filename="<<source_filename<<std::endl;
		if (!settings().getKeepSubFolders()) {
			// Output straight into the target folder
			result = outputPath + FileUtils::separator() + FileUtils::extractFilename(result);
		} else {
			std::string relative = source_filename.substr(inputPath.size()+1, source_filename.size()-(inputPath.size()+1));
			result = outputPath + FileUtils::separator() + relative;
		}
		if (settings().getRemoveExtension()) { result = FileUtils::removeExtension(result); }
		return result;
	}

	//=======================================================================================
	Component* ReplaceEngine::getComponent(const std::string& name)
	{
		for (ComponentList::iterator itr = componentList_.begin(); itr != componentList_.end(); ++itr) {
			if (itr->getName() == name) { return &(*itr); }
		}
		return NULL;
	}

	//=======================================================================================
	bool ReplaceEngine::processFile(const std::string& source_filename, const std::string& target_filename)
	{
		lre::notify(lre::DEBUG)<<"Processing..."<<std::endl;
		lre::notify(lre::DEBUG)<<"In: "<<source_filename<<std::endl;
		lre::notify(lre::DEBUG)<<"Out: "<<target_filename<<std::endl;
		// Make a directory for the file, if it does not exist. makeDirectory
		// returns true if successfully created or already existing.
		if (!FileUtils::makeDirectory(FileUtils::extractDirectory(target_filename))) {
			return false;
		}
		
		std::string source = FileUtils::getFile(source_filename);

		std::string keywordBegin = "<LRE";
		std::string keywordEnd = "</LRE";
		std::string keywordEndTag = ">";
		std::string keywordSeparator = ":";
		std::string keywordComponent = "COMPONENT";
		std::string keywordKey = "KEY";

		std::string::size_type pos = 0;
		std::string compStart = keywordBegin+keywordSeparator+keywordComponent+keywordSeparator;
		std::string comp = compStart;
		std::string::size_type f = source.find(comp, pos);
		if (f == std::string::npos) {
			lre::notify(lre::DEBUG)<<"Nothing to be done."<<std::endl<<std::endl;
			return true;
		}
		// Process 'COMPONENT's
		while (f != std::string::npos) {
			std::string::size_type compStartIndex = f;
			std::string::size_type f2 = source.find(keywordEndTag, f);
			if (f2 == std::string::npos) {
				lre::notify(lre::ERROR)<<red<<"Unclosed "<<comp<<"-tag after position "<<f<<white;
				lre::notify(lre::DEBUG)<<red<<" in "<<source_filename<<" "<<white;
				lre::notify(lre::ALWAYS).flush();
				return false;
			}
			std::string componentName = source.substr(f+comp.size(), f2-f-comp.size());
			pos = f2 + keywordEndTag.size();
			std::string::size_type dataStart = pos;
			lre::notify(lre::DEBUG)<<"Component: \""<<componentName<<"\""<<std::flush;
			comp = keywordEnd+keywordSeparator+keywordComponent+keywordSeparator+componentName+keywordEndTag;
			f = source.find(comp, pos);
			if (f == std::string::npos) {
				lre::notify(lre::ERROR)<<red<<"Missing "<<comp<<" "<<white;
				lre::notify(lre::DEBUG)<<red<<" for "<<componentName<<" at position "<<pos<<white;
				lre::notify(lre::ALWAYS).flush();
				return false;
			}
			pos = f+comp.size();
			std::string::size_type dataEnd = f;
			lre::notify(lre::DEBUG)<<" from "<<dataStart<<" to "<<dataEnd<<std::endl;
			std::string::size_type compEndIndex = dataEnd+comp.size();
			Component* compData = getComponent(componentName);
			if (compData == NULL) {
				lre::notify(lre::DEBUG)<<red<<"Component '"<<componentName<<"' is "<<white;
				lre::notify(lre::ERROR)<<red<<"undefined! "<<white<<std::endl;
				comp = compStart;
				f = source.find(comp, compEndIndex);
				continue;
			}

			std::string data = source.substr(dataStart, dataEnd-dataStart);
			lre::notify(lre::DEBUG)<<" --- Data --- "<<std::endl;
			lre::notify(lre::DEBUG)<<"\""<<data<<"\""<<std::endl;
			lre::notify(lre::DEBUG)<<" --- End of data --- "<<std::endl;

			std::string resultData = "";

			if (compData->getNumSets() > 0) {
				lre::notify(lre::DEBUG)<<"Processing "<<compData->getNumSets()<<" sets (indexed from 0 to "<<compData->getNumSets()-1<<")"<<std::endl;
			}
			for (unsigned int setNumber = 0; setNumber < compData->getNumSets(); ++setNumber) {
				Set* dataSet = compData->getSet(setNumber);
				if (dataSet == NULL) {
					lre::notify(lre::ERROR)<<red<<" --- Failed to get Set no. "<<setNumber<<" for Component '"+componentName+"'--- "<<white<<std::endl;
					continue;
				}
				lre::notify(lre::DEBUG)<<" --- Set #"<<setNumber<<" --- "<<std::endl;

				data = source.substr(dataStart, dataEnd-dataStart);
				std::string::size_type keyPos = 0;
				std::string key = keywordBegin+keywordSeparator+keywordKey+keywordSeparator;
				keyPos = data.find(key, keyPos);

				// Process 'KEY's
				while (keyPos != std::string::npos) {
					std::string::size_type keyPos2 = data.find(keywordEndTag, keyPos);
					if (keyPos2 == std::string::npos) {
						lre::notify(lre::ERROR)<<red<<"Unclosed "<<key<<"-tag after position "<<keyPos<<white; lre::notify(lre::ALWAYS).flush();
						return false;
					}
					std::string::size_type keyPosEnd = keyPos2+keywordEndTag.size();
					std::string keyName = data.substr(keyPos+key.size(), keyPos2-keyPos-key.size());

					StringStringMap::const_iterator valueItr = dataSet->getMap().find(keyName);
					if (valueItr == dataSet->getMap().end()) {
						lre::notify(lre::ERROR)<<red<<"Error: Key '"<<keyName<<"' for Set #"<<setNumber+1<<" of Component '"<<componentName<<"' not found."<<white<<std::endl;
						keyPos = data.find(key, keyPos2+keywordEndTag.size());
						return false;
					}
					data.replace(keyPos, keyPosEnd-keyPos, valueItr->second);
					//lre::notify(lre::ALWAYS)<<"newData:"<<data;
					keyPos = data.find(key/*, keyPos2+keywordEndTag.size()*/);
				}
				resultData += data;
				lre::notify(lre::DEBUG)<<data<<std::flush;
				if (settings().getAddAppendixAfterLastSet() || (!settings().getAddAppendixAfterLastSet() && setNumber != compData->getNumSets()-1)) {
					resultData += settings().getAppendixString();
					lre::notify(lre::DEBUG)<<settings().getAppendixString();
				}

				lre::notify(lre::DEBUG)<<std::endl<<" --- End of Set #"<<setNumber<<" --- "<<std::endl;
			}

			//lre::notify(lre::ALWAYS)<<"Compl. COMP: '"<<source.substr(compStartIndex, compEndIndex-compStartIndex)<<"' "; lre::notify().flush();
			source.replace(compStartIndex, compEndIndex-compStartIndex, resultData);

			comp = compStart;
			f = source.find(comp/*, pos*/);
		}

		bool success = FileUtils::putFile(target_filename, source);
		if (success) {
			lre::notify(lre::DEBUG)<<"File completed successfully."<<std::endl;
		} else {
			lre::notify(lre::DEBUG)<<"Write file failed."<<std::endl;
		}
		lre::notify(lre::DEBUG)<<std::endl;

		return true;
	}

	//=======================================================================================
	bool ReplaceEngine::saveData(const std::string& filename) const
	{
		std::string result = "";
		for (ComponentList::const_iterator itr = componentList_.begin(); itr != componentList_.end(); ++itr) {
			result += itr->toString()+lre::FileUtils::getNativeEndline();
		}
		return FileUtils::putFile(filename, result);
	}

	//=======================================================================================
	bool ReplaceEngine::loadData(const std::string& filename)
	{
		std::string str = FileUtils::getFile(filename);
		
		// Failed to read or empty file
		if (str == "") { return false; }

		std::string compTagBegin1 = "<LRE:COMPONENT:"; std::string compTagBegin2 = ">";
		std::string compTagEnd1 = "</LRE:COMPONENT:"; std::string compTagEnd2 = ">";

		std::string::size_type f = 0; // the current string position
		std::string::size_type s = 0; // position of the start of the component
		std::string::size_type e = 0; // position of the end of the component
		unsigned int i = 0; // number of components found inside the file

		while (f != std::string::npos) {
			f = str.find(compTagBegin1, f);
			s = f;
			if (f == std::string::npos) {
				if (i == 0) {
					// No component found, yet. So this is an error.
					return false;
				} else {
					// We already have one or more components found, so we should be sure everything is done.
					return true;
				}
			}
			f += compTagBegin1.size();

			std::string::size_type namePos = str.find(compTagBegin2, f);
			// Component ">" missing
			if (namePos == std::string::npos) { return false; }

			// Determine Component Name
			std::string compName = str.substr(f, namePos-f);
			f = namePos + compTagBegin2.size();

			std::string endTag = compTagEnd1+compName+compTagEnd2;
			e = str.find(endTag, f);
			// Missing endTag "</LRE:COMPONENT:compName>"
			if (e == std::string::npos) { return false; }
			e += endTag.size();

			lre::Component newComponent("");
			if (newComponent.fromString(str.substr(s, e-s))) {
				componentList_.push_back(newComponent);
				f = e;
				i++;
			} else {
				// Invalid component data
				return false;
			}
		}

		return (i>0);
	}

	//=======================================================================================
	void ReplaceEngine::reportData()
	{
		lre::notify(lre::NOTICE)<<"--- Lightweight Replace Engine: Data ---"<<std::endl;
		lre::notify(lre::NOTICE)<<"No. of Components: "<<componentList_.size()<<std::endl;
		for (ComponentList::const_iterator itr = componentList_.begin(); itr != componentList_.end(); ++itr) {
			lre::notify(lre::NOTICE)<<"Component: "<<itr->getName()<<std::endl;
		}
		lre::notify(lre::NOTICE)<<std::endl;
	}

} // namespace lre

