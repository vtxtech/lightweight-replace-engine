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
#include "FileUtil.h"

//-- STL --//
#include <iostream>
#include <fstream>

//-- OSG --//
#include "extern/FileUtils.h"

namespace lre {

	//=======================================================================================
	ReplaceEngine::ReplaceEngine(): recursive_(false), inputPath_(""),
		outputPath_(""), pattern_("*.in"), removeExtension_(true),
		keepStructure_(true), dataPath_(""), dataPattern_("*.lre"),
		appendix_(""), appendixAfterLastSet_(true)
	{
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
		lre::ArgumentParser ap(argc, argv);
		ap.setApplicationName("Lightweight Replace Engine by Johannes Scholz");
		ap.setApplicationUsage("[options]");

		ap.addCommandLineOption("--recursive or --R", "Enables input path recursive mode. Only valid if --input is set to a path. Disabled by default.");
		ap.addCommandLineOption("--keepExtension or -K", "Define whether the extension should be kept in the target filenames. Usually the last extension in the input file name is removed. Remove extensions only if your files are 'myfile.txt.in' format.");
		ap.addCommandLineOption("--pattern <file_pattern>", "Sets the file pattern to match if --input is set to a path. Default is '*.in'. Allowed wildcards: * and ?");
		ap.addCommandLineOption("--input <path or filename>", "Sets the input path or filename. Required option.");
		ap.addCommandLineOption("--output <path>", "Sets the output path. Required option.");
		ap.addCommandLineOption("--forgetSubfolders or -F", "Remove recursive directory structure in output directory. Disabled by default.");
		ap.addCommandLineOption("--data <path or filename>", "Data file containg Components or path to multiple files matching --dataPattern. Empty by default. Required option, if no data is defined in source code, only.");
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

		return 0;
	}

	//=======================================================================================
	Component* ReplaceEngine::addComponent(const std::string& name)
	{
		componentList_.push_back(Component(name));
		return &(componentList_.at(componentList_.size()-1));
	}

	//=======================================================================================
	void ReplaceEngine::setAppendixString(const std::string& appendix)
	{
		appendix_ = appendix;
	}

	//=======================================================================================
	int ReplaceEngine::run()
	{
		// Check input data roughly
		if (inputPath_ == "") { std::cout<<"FATAL: Input path is undefined"<<std::endl; return 1; }
		if (outputPath_ == "") { std::cout<<"FATAL: Output path is undefined"<<std::endl; return 1; }
		if (removeExtension_ && pattern_ == "") { std::cout<<"FATAL: Cannot remove extension without extension defined."<<std::endl; return 1; }
		if (inputPath_ == outputPath_ && !removeExtension_) { std::cout<<"FATAL: Input path and output path must not be equal if extensions are kept."<<std::endl; return 1; }
		if (componentList_.size() == 0 && dataPath_ == "") { std::cout<<"FATAL: No data defined. Please set either data path or add components in source."<<std::endl; return 1; }

		outputPath_ = FileUtil::excludeTrailingSeparator(outputPath_);
		inputPath_ = FileUtil::excludeTrailingSeparator(inputPath_);
		dataPath_ = FileUtil::excludeTrailingSeparator(dataPath_);

		// Report input data to std::cout
		reportSetup();

		// Backup current lre::Component data which is restored at the end of run()
		ComponentList tempCompList = componentList_;

		if (dataPath_ != "") {
			// Build the list of lre::Components from dataPath_
			std::vector<std::string> dataFiles;
			dataFiles.clear();

			//std::cout<<"Data file lookup..."<<std::endl;
			dataFiles = FileUtil::findFiles(dataPath_, dataPattern_, false);

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
		files = FileUtil::findFiles(inputPath_, pattern_, recursive_);

#ifdef _DEBUG
		std::cout<<std::endl;
#endif
		std::cout<<"--- Source files matching pattern ---"<<std::endl;
		for (unsigned int i = 0; i < files.size(); ++i) {
			std::cout<<"FILE("<<i<<"): "<<files.at(i)<<"... "<<std::flush;
			if (processFile(files.at(i), makeTargetFilename(files.at(i)))) {
				std::cout<<"Success."<<std::endl;
			} else {
				std::cout<<"Failed."<<std::endl;
			}
		}

		// Restore componentList_ from backup before file loading
		componentList_ = tempCompList;

		return 0;
	}

	//=======================================================================================
	std::string ReplaceEngine::makeTargetFilename(const std::string& source_filename) const
	{
		std::cout<<"target file..."<<std::endl;
		std::string result = source_filename;
		// Settings for consideration
		// * bool removeExtension_;
		// * bool keepStructure_;
		// * std::string inputPath_;
		// * std::string outputPath_;
		std::string outputPath = outputPath_;
		std::string inputPath = inputPath_;
		if (osgDB::fileType(inputPath) == osgDB::REGULAR_FILE) {
			inputPath = FileUtil::extractDirectory(inputPath);
		}
		if (osgDB::fileType(outputPath) == osgDB::REGULAR_FILE) {
			outputPath = FileUtil::extractDirectory(outputPath);
		}
		std::cout<<"outputPath="<<outputPath<<std::endl;
		std::cout<<"inputPath="<<inputPath<<std::endl;
		std::cout<<"source_filename="<<source_filename<<std::endl;
		if (!keepStructure_) {
			// Output straight into the target folder
			result = outputPath + FileUtil::separator() + FileUtil::extractFilename(result);
		} else {
			std::string relative = source_filename.substr(inputPath.size()+1, source_filename.size()-(inputPath.size()+1));
			result = outputPath + FileUtil::separator() + relative;
		}
		if (removeExtension_) { result = FileUtil::removeExtension(result); }
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
		std::cout<<"Processing..."<<std::endl;
		// Make a directory for the file, if it does not exist. makeDirectory
		// returns true if successfully created or already existing.
		if (!FileUtil::makeDirectory(FileUtil::extractDirectory(target_filename))) {
			return false;
		}
		
		std::string source = FileUtil::getFile(source_filename);

		std::string keywordBegin = "<LRE";
		std::string keywordEnd = "</LRE";
		std::string keywordEndTag = ">";
		std::string keywordSeparator = ":";
		std::string keywordComponent = "COMPONENT";
		std::string keywordKey = "KEY";

		unsigned int pos = 0;
		std::string compStart = keywordBegin+keywordSeparator+keywordComponent+keywordSeparator;
		std::string comp = compStart;
		unsigned int f = source.find(comp, pos);
		if (f == std::string::npos) {
			std::cout<<"Nothing to be done. "; std::cout.flush();
		}
		// Process 'COMPONENT's
		while (f != std::string::npos) {
			unsigned int compStartIndex = f;
			unsigned int f2 = source.find(keywordEndTag, f);
			if (f2 == std::string::npos) {
				std::cout<<"Unclosed "<<comp<<"-tag. "; std::cout.flush();
				return false;
			}
			std::string componentName = source.substr(f+comp.size(), f2-f-comp.size());
			pos = f2 + keywordEndTag.size();
			unsigned int dataStart = pos;
			std::cout<<"Component: "<<componentName<<" "; std::cout.flush();
			comp = keywordEnd+keywordSeparator+keywordComponent+keywordSeparator+componentName+keywordEndTag;
			f = source.find(comp, pos);
			if (f == std::string::npos) {
				std::cout<<"Missing "<<comp<<" "; std::cout.flush();
				return false;
			}
			pos = f+comp.size();
			unsigned int dataEnd = f;
			unsigned int compEndIndex = dataEnd+comp.size();
			Component* compData = getComponent(componentName);
			if (compData == NULL) {
				std::cout<<"undefined! "<<std::endl;
				comp = compStart;
				f = source.find(comp, compEndIndex);
				continue;
			}

			std::string resultData = "";

			for (unsigned int setNumber = 0; setNumber < compData->getNumSets(); ++setNumber) {
				Set* dataSet = compData->getSet(setNumber);
				if (dataSet == NULL) {
					std::cout<<" --- Failed to get Set no. "<<setNumber<<" --- "<<std::endl;
					continue;
				}

				std::string data = source.substr(dataStart, dataEnd-dataStart);
				std::cout<<std::endl<<" --- Data --- "<<std::endl;
				std::cout<<"\""<<data<<"\""<<std::endl;
				std::cout<<" --- End of data --- "<<std::endl;

				unsigned int keyPos = 0;
				std::string key = keywordBegin+keywordSeparator+keywordKey+keywordSeparator;
				keyPos = data.find(key, keyPos);

				// Process 'KEY's
				while (keyPos != std::string::npos) {
					unsigned int keyPos2 = data.find(keywordEndTag, keyPos);
					if (keyPos2 == std::string::npos) {
						std::cout<<"Unclosed "<<key<<"-tag. "; std::cout.flush();
						return false;
					}
					unsigned int keyPosEnd = keyPos2+keywordEndTag.size();
					std::string keyName = data.substr(keyPos+key.size(), keyPos2-keyPos-key.size());
					std::cout<<"Key: '"<<keyName<<"' "; std::cout.flush();
					std::cout<<"Compl. entry: '"<<data.substr(keyPos, keyPosEnd-keyPos)<<"' "; std::cout.flush();
					StringStringMap::const_iterator valueItr = dataSet->getMap().find(keyName);
					if (valueItr == dataSet->getMap().end()) {
						std::cout<<"Key not found. ";
						keyPos = data.find(key, keyPos2+keywordEndTag.size());
						continue;
					}
					data.replace(keyPos, keyPosEnd-keyPos, valueItr->second);
					std::cout<<"newData:"<<data;
					keyPos = data.find(key/*, keyPos2+keywordEndTag.size()*/);
				}
				resultData += data;
				if (appendixAfterLastSet_ || (!appendixAfterLastSet_ && setNumber != compData->getNumSets()-1)) {
					resultData += appendix_;
				}
			}

			std::cout<<"Compl. COMP: '"<<source.substr(compStartIndex, compEndIndex-compStartIndex)<<"' "; std::cout.flush();
			source.replace(compStartIndex, compEndIndex-compStartIndex, resultData);

			comp = compStart;
			f = source.find(comp/*, pos*/);
		}

		return FileUtil::putFile(target_filename, source);
	}

	//=======================================================================================
	bool ReplaceEngine::saveData(const std::string& filename) const
	{
		std::string result = "";
		for (ComponentList::const_iterator itr = componentList_.begin(); itr != componentList_.end(); ++itr) {
			result += itr->toString()+lre::FileUtil::getNativeEndline();
		}
		return FileUtil::putFile(filename, result);
	}

	//=======================================================================================
	bool ReplaceEngine::loadData(const std::string& filename)
	{
		std::string str = FileUtil::getFile(filename);
		
		// Failed to read or empty file
		if (str == "") { return false; }

		std::string compTagBegin1 = "<LRE:COMPONENT:"; std::string compTagBegin2 = ">";
		std::string compTagEnd1 = "</LRE:COMPONENT:"; std::string compTagEnd2 = ">";

		unsigned int f = 0; // the current string position
		unsigned int s = 0; // position of the start of the component
		unsigned int e = 0; // position of the end of the component
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

			unsigned int namePos = str.find(compTagBegin2, f);
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
	void ReplaceEngine::reportSetup()
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

