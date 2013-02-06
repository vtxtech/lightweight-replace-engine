/*
 * Lightweight Replace Engine - GoogleTest based unit tests
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief Tests for ReplaceEngine::run()
 */

// --- GTest --- //
#include <gtest/gtest.h>

// --- LRE --- //
#include <lre/ReplaceEngine.h>
#include <lre/FileUtils.h>

class RunTest : public ::testing::Test {
  public:

    virtual void SetUp()
	{
		filename_ = "./generated-by-runtest/output.lre";
		directory_ = lre::FileUtils::extractDirectory(filename_);
		lre::FileUtils::makeDirectory(directory_);
    }
    
    virtual void TearDown()
	{

	}

	std::string filename_;
	std::string directory_;

};

TEST_F(RunTest, InvalidData)
{
	// Create a ReplaceEngine to do the job
	lre::ReplaceEngine re;

	re.settings().setVerbose(true);
	
	// Ignore any command line arguments
	// we setup everything manually here.
	re.settings().setInput("../data");
	re.settings().setOutputDirectory(directory_);
	re.settings().setRecursive(true);
	re.settings().setRemoveExtension(true);
	re.settings().setFilePattern("*.in");
	re.settings().setAppendixString("\n");
	re.settings().setAddAppendixAfterLastSet(false);
	
	 // not required because the data used for replacing is following below
	re.settings().setDataDirectory("");

	// Add the data for file generation here
	lre::Component* cmd = re.addComponent("Command");
	lre::Set* set1 = cmd->addSet();
	set1->addPair("name", "import");
	set1->addPair("description_en", "Imports a geometry file");
	set1->addPair("description_de", "Importiert eine Geometriedatei");
	lre::Set* set2 = cmd->addSet();
	set2->addPair("name", "exit");
	set2->addPair("description_en", "Exits application");
	//set2->addPair("description_de", "Beendet die Anwendung");

	lre::Component* users = re.addComponent("Users");
	lre::Set* set11 = users->addSet();
	//set11->addPair("name", "jos");
	set11->addPair("password", "abc123");
	lre::Set* set12 = users->addSet();
	set12->addPair("name", "mw");
	set12->addPair("password", "dummy");

	// Finally to the work
	EXPECT_EQ(3, re.run());
}



