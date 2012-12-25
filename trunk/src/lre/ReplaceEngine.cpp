/*
 * Lightweight Replace Engine
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
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

namespace lre {

	//=======================================================================================
	ReplaceEngine::ReplaceEngine(): recursive_(false), inputPath_(""), outputPath_(""), pattern_("*.in"), removeExtension_(true), keepStructure_(true)
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
		lre::ArgumentParser ap(argc, argv);
		ap.setApplicationName("Lightweight Replace Engine by Johannes Scholz");
		ap.setApplicationUsage("[options]");

		ap.addCommandLineOption("--recursive or --R", "Enables input path recursive mode. Only valid if --input is set to a path. Disabled by default.");
		ap.addCommandLineOption("--keepExtension or -K", "Define whether the extension should be kept in the target filenames. Usually the last extension in the input file name is removed. Remove extensions only if your files are 'myfile.txt.in' format.");
		ap.addCommandLineOption("--pattern <pat>", "Sets the file pattern to match if --input is set to a path. Default is '*.in'. Allowed wildcards: * and ?");
		ap.addCommandLineOption("--input <path or filename>", "Sets the input path or filename.");
		ap.addCommandLineOption("--output <path>", "Sets the output path.");
		ap.addCommandLineOption("--forgetSubfolders or -F", "Remove recursive directory structure in output directory. Disabled by default.");

		if (ap.getArgumentCount() <= 1 || ap.isSet("--help") || ap.isSet("-h") || ap.isSet("/?") || ap.isSet("-?")) {
			ap.reportOptions();
			return 1;
		}
		
		if (ap.isSet("--recursive") || ap.isSet("-R")) {
			recursive_ = true;
		}

		if (ap.isSet("--forgetSubfolders") || ap.isSet("-F")) {
			keepStructure_ = true;
		}

		if (ap.isSet("--keepExtension") || ap.isSet("-K")) {
			removeExtension_ = true;
		}

		ap.read("--pattern", pattern_);
		ap.read("--input", inputPath_);
		ap.read("--output", outputPath_);

		return 0;
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
		if (inputPath_ == "") { std::cout<<"FATAL: Input path is undefined"<<std::endl; return 1; }
		if (outputPath_ == "") { std::cout<<"FATAL: Output path is undefined"<<std::endl; return 1; }
		if (removeExtension_ && pattern_ == "") { std::cout<<"FATAL: Cannot remove extension without extension defined."<<std::endl; return 1; }
		if (inputPath_ == outputPath_ && !removeExtension_) { std::cout<<"FATAL: Input path and output path must not be equal if extensions are kept."<<std::endl; return 1; }
		
		outputPath_ = FileUtil::excludeTrailingSeparator(outputPath_);
		inputPath_ = FileUtil::excludeTrailingSeparator(inputPath_);

		// Report input data for std::cout
		reportSetup();

		// This is the list of files to be processed
		std::vector<std::string> files;
		files.clear();

		std::cout<<"File lookup..."<<std::endl;
		files = FileUtil::findFiles(inputPath_, pattern_, recursive_);

		std::cout<<"--- Files matching pattern ---"<<std::endl;
		for (unsigned int i = 0; i < files.size(); ++i) {
			std::cout<<"FILE("<<i<<"): "<<files.at(i)<<"... "<<std::flush;
			if (processFile(files.at(i), makeTargetFilename(files.at(i)))) {
				std::cout<<"Success."<<std::endl;
			} else {
				std::cout<<"Failed."<<std::endl;
			}
		}

		return 0;
	}

	//=======================================================================================
	std::string ReplaceEngine::makeTargetFilename(const std::string& source_filename)
	{
		std::string result = source_filename;
		// Settings for consideration
		// * bool removeExtension_;
		// * bool keepStructure_;
		// * std::string inputPath_;
		// * std::string outputPath_;
		if (!keepStructure_) {
			// Output straight into the target folder
			result = outputPath_ + FileUtil::separator() + FileUtil::extractFilename(result);
		} else {
			std::string relative = source_filename.substr(inputPath_.size()+1, source_filename.size()-(inputPath_.size()+1));
			result = outputPath_ + FileUtil::separator() + relative;
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

			for (unsigned int setNumber = 0; setNumber < compData->getSetCount(); ++setNumber) {
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
			}

			std::cout<<"Compl. COMP: '"<<source.substr(compStartIndex, compEndIndex-compStartIndex)<<"' "; std::cout.flush();
			source.replace(compStartIndex, compEndIndex-compStartIndex, resultData);

			comp = compStart;
			f = source.find(comp/*, pos*/);
		}

		return FileUtil::putFile(target_filename, source);
	}

	//=======================================================================================
	void ReplaceEngine::reportSetup()
	{
		std::cout<<"--- Lightweight Replace Engine: Setup ---"<<std::endl;
		std::cout<<"Recurse directory: "<<recursive_<<std::endl;
		std::cout<<"Remove extensions: "<<removeExtension_<<std::endl;
		std::cout<<"Input path: "<<inputPath_<<std::endl;
		std::cout<<"Output directory: "<<outputPath_<<std::endl;
		std::cout<<"Keep directory structure: "<<keepStructure_<<std::endl;
		std::cout<<"Extension match: "<<pattern_<<std::endl;
		std::cout<<"--- Data ---"<<std::endl;
		std::cout<<"No. of Components: "<<componentList_.size()<<std::endl;
		for (ComponentList::const_iterator itr = componentList_.begin(); itr != componentList_.end(); ++itr) {
			std::cout<<"Component: "<<itr->getName()<<std::endl;
		}
		std::cout<<std::endl;
	}

} // namespace lre

