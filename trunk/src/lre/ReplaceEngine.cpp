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
		if (settings().getInput() == "") { std::cout<<"FATAL: Input path is undefined"<<std::endl; return 1; }
		if (settings().getOutputDirectory() == "") { std::cout<<"FATAL: Output path is undefined"<<std::endl; return 1; }
		if (settings().getRemoveExtension() && settings().getFilePattern() == "") { std::cout<<"FATAL: Cannot remove extension without extension defined."<<std::endl; return 1; }
		if (settings().getInput() == settings().getOutputDirectory() && !settings().getRemoveExtension()) { std::cout<<"FATAL: Input path and output path must not be equal if extensions are kept."<<std::endl; return 1; }
		if (componentList_.size() == 0 && settings().getDataDirectory() == "") { std::cout<<"FATAL: No data defined. Please set either data path or add components in source."<<std::endl; return 1; }

		settings().setOutputDirectory(FileUtils::excludeTrailingSeparator(settings().getOutputDirectory()));
		settings().setInput(FileUtils::excludeTrailingSeparator(settings().getInput()));
		settings().setDataDirectory(FileUtils::excludeTrailingSeparator(settings().getDataDirectory()));

		// Report input data to std::cout
		settings().reportSetup();

		// Backup current lre::Component data which is restored at the end of run()
		ComponentList tempCompList = componentList_;

		if (settings().getDataDirectory() != "") {
			// Build the list of lre::Components from settings().getDataDirectory()
			std::vector<std::string> dataFiles;
			dataFiles.clear();

			//std::cout<<"Data file lookup..."<<std::endl;
			dataFiles = FileUtils::findFiles(settings().getDataDirectory(), settings().getDataPattern(), false);

			std::cout<<"--- Data files matching pattern ---"<<std::endl;
			for (unsigned int i = 0; i < dataFiles.size(); ++i) {
				std::cout<<"FILE("<<i<<"): "<<dataFiles.at(i)<<"... "<<std::flush;
				if (loadData(dataFiles.at(i))) {
					std::cout<<"Success."<<std::endl;
				} else {
					std::cout<<"Failed."<<std::endl;
				}
			}
			std::cout<<std::endl;
		}

		// Report data to std::cout
		reportData();

		// This is the list of files to be processed
		std::vector<std::string> files;
		files.clear();

		//std::cout<<"Source file lookup..."<<std::endl;
		files = FileUtils::findFiles(settings().getInput(), settings().getFilePattern(), settings().getRecursive());

#ifdef _DEBUG
		std::cout<<std::endl;
#endif
		std::cout<<"--- Source files matching pattern ---"<<std::endl;
		for (unsigned int i = 0; i < files.size(); ++i) {
			processFile(files.at(i), makeTargetFilename(files.at(i)));
		}

		// Restore componentList_ from backup before file loading
		componentList_ = tempCompList;

		return 0;
	}

	//=======================================================================================
	std::string ReplaceEngine::makeTargetFilename(const std::string& source_filename) const
	{
		//std::cout<<"target file..."<<std::endl;
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
		//std::cout<<"outputPath="<<outputPath<<std::endl;
		//std::cout<<"inputPath="<<inputPath<<std::endl;
		//std::cout<<"source_filename="<<source_filename<<std::endl;
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
		//std::cout<<"Processing..."<<std::endl;
		std::cout<<"In: "<<source_filename<<std::endl;
		std::cout<<"Out: "<<target_filename<<std::endl;
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
			std::cout<<"Nothing to be done."<<std::endl<<std::endl;
			return true;
		}
		// Process 'COMPONENT's
		while (f != std::string::npos) {
			std::string::size_type compStartIndex = f;
			std::string::size_type f2 = source.find(keywordEndTag, f);
			if (f2 == std::string::npos) {
				std::cout<<"Unclosed "<<comp<<"-tag. "; std::cout.flush();
				return false;
			}
			std::string componentName = source.substr(f+comp.size(), f2-f-comp.size());
			pos = f2 + keywordEndTag.size();
			std::string::size_type dataStart = pos;
			std::cout<<"Component: \""<<componentName<<"\""<<std::flush;
			comp = keywordEnd+keywordSeparator+keywordComponent+keywordSeparator+componentName+keywordEndTag;
			f = source.find(comp, pos);
			if (f == std::string::npos) {
				std::cout<<"Missing "<<comp<<" "; std::cout.flush();
				return false;
			}
			pos = f+comp.size();
			std::string::size_type dataEnd = f;
			std::cout<<" from "<<dataStart<<" to "<<dataEnd<<std::endl;
			std::string::size_type compEndIndex = dataEnd+comp.size();
			Component* compData = getComponent(componentName);
			if (compData == NULL) {
				std::cout<<"undefined! "<<std::endl;
				comp = compStart;
				f = source.find(comp, compEndIndex);
				continue;
			}

			std::string data = source.substr(dataStart, dataEnd-dataStart);
			std::cout<<" --- Data --- "<<std::endl;
			std::cout<<"\""<<data<<"\""<<std::endl;
			std::cout<<" --- End of data --- "<<std::endl;

			std::string resultData = "";

			for (unsigned int setNumber = 0; setNumber < compData->getNumSets(); ++setNumber) {
				Set* dataSet = compData->getSet(setNumber);
				if (dataSet == NULL) {
					std::cout<<" --- Failed to get Set no. "<<setNumber<<" --- "<<std::endl;
					continue;
				}
				std::cout<<" --- Set #"<<setNumber+1<<" --- "<<std::endl;

				data = source.substr(dataStart, dataEnd-dataStart);
				std::string::size_type keyPos = 0;
				std::string key = keywordBegin+keywordSeparator+keywordKey+keywordSeparator;
				keyPos = data.find(key, keyPos);

				// Process 'KEY's
				while (keyPos != std::string::npos) {
					std::string::size_type keyPos2 = data.find(keywordEndTag, keyPos);
					if (keyPos2 == std::string::npos) {
						std::cout<<"Unclosed "<<key<<"-tag. "; std::cout.flush();
						return false;
					}
					std::string::size_type keyPosEnd = keyPos2+keywordEndTag.size();
					std::string keyName = data.substr(keyPos+key.size(), keyPos2-keyPos-key.size());
					//std::cout<<"Key: '"<<keyName<<"' "; std::cout.flush();
					//std::cout<<"Compl. entry: '"<<data.substr(keyPos, keyPosEnd-keyPos)<<"' "; std::cout.flush();
					StringStringMap::const_iterator valueItr = dataSet->getMap().find(keyName);
					if (valueItr == dataSet->getMap().end()) {
						std::cout<<"Key not found. ";
						keyPos = data.find(key, keyPos2+keywordEndTag.size());
						continue;
					}
					data.replace(keyPos, keyPosEnd-keyPos, valueItr->second);
					//std::cout<<"newData:"<<data;
					keyPos = data.find(key/*, keyPos2+keywordEndTag.size()*/);
				}
				resultData += data;
				std::cout<<data<<std::flush;
				if (settings().getAddAppendixAfterLastSet() || (!settings().getAddAppendixAfterLastSet() && setNumber != compData->getNumSets()-1)) {
					resultData += settings().getAppendixString();
					std::cout<<settings().getAppendixString();
				}

				std::cout<<std::endl<<" --- End of Set #"<<setNumber+1<<" --- "<<std::endl;
			}

			//std::cout<<"Compl. COMP: '"<<source.substr(compStartIndex, compEndIndex-compStartIndex)<<"' "; std::cout.flush();
			source.replace(compStartIndex, compEndIndex-compStartIndex, resultData);

			comp = compStart;
			f = source.find(comp/*, pos*/);
		}

		bool success = FileUtils::putFile(target_filename, source);
		if (success) {
			std::cout<<"File completed successfully."<<std::endl;
		} else {
			std::cout<<"Write file failed."<<std::endl;
		}
		std::cout<<std::endl;
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
		std::cout<<"--- Lightweight Replace Engine: Data ---"<<std::endl;
		std::cout<<"No. of Components: "<<componentList_.size()<<std::endl;
		for (ComponentList::const_iterator itr = componentList_.begin(); itr != componentList_.end(); ++itr) {
			std::cout<<"Component: "<<itr->getName()<<std::endl;
		}
		std::cout<<std::endl;
	}

} // namespace lre

