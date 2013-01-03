/*
 * Lightweight Replace Engine - GoogleTest based unit tests
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief File Input/Output Tests checking integrity of lre::Component/lre::Set data sets
 */

// --- GTest --- //
#include <gtest/gtest.h>

// --- LRE --- //
#include <lre/ReplaceEngine.h>
#include <lre/FileUtil.h>

// --- STL --- //
//#include <string>
//#include <stdio.h>
//#include <math.h>

class FileIOTest : public ::testing::Test {
  public:

    virtual void SetUp()
	{
		filename_ = "./generated-by-test/output.lre";
		lre::FileUtil::makeDirectory(lre::FileUtil::extractDirectory(filename_));
    }
    
    virtual void TearDown()
	{

	}

	std::string filename_;

};

TEST_F(FileIOTest, WriteData)
{
	// Create a ReplaceEngine to do the job
	lre::ReplaceEngine re;
	
	// No setup needed, this test just writes a data file

	// Add the data for file generation here
	lre::Component* cmd = re.addComponent("Command");
	lre::Set* set1 = cmd->addSet();
	set1->addPair("name", "import");
	set1->addPair("description_en", "Imports a geometry file");
	set1->addPair("description_de", "Importiert eine Geometriedatei");
	lre::Set* set2 = cmd->addSet();
	set2->addPair("name", "exit");
	set2->addPair("description_en", "Exits application");
	set2->addPair("description_de", "Beendet die Anwendung");

	lre::Component* users = re.addComponent("Users");
	lre::Set* set11 = users->addSet();
	set11->addPair("name", "jos");
	set11->addPair("password", "abc123");
	lre::Set* set12 = users->addSet();
	set12->addPair("name", "mw");
	set12->addPair("password", "dummy");

	EXPECT_TRUE(re.saveData(filename_));

	std::string content_test = lre::FileUtil::getFile(filename_);
	EXPECT_TRUE(content_test == "<LRE:COMPONENT:Command>"+lre::FileUtil::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:description_de>Importiert eine Geometriedatei</LRE:KEY:description_de>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:description_en>Imports a geometry file</LRE:KEY:description_en>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:name>import</LRE:KEY:name>"+lre::FileUtil::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:description_de>Beendet die Anwendung</LRE:KEY:description_de>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:description_en>Exits application</LRE:KEY:description_en>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:name>exit</LRE:KEY:name>"+lre::FileUtil::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"</LRE:COMPONENT:Command>"+lre::FileUtil::getNativeEndline()+
								"<LRE:COMPONENT:Users>"+lre::FileUtil::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:name>jos</LRE:KEY:name>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:password>abc123</LRE:KEY:password>"+lre::FileUtil::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:name>mw</LRE:KEY:name>"+lre::FileUtil::getNativeEndline()+
								"		<LRE:KEY:password>dummy</LRE:KEY:password>"+lre::FileUtil::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtil::getNativeEndline()+
								"</LRE:COMPONENT:Users>"+lre::FileUtil::getNativeEndline()+
								"");
}


