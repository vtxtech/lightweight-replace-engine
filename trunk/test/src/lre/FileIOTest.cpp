/*
 * Lightweight Replace Engine - GoogleTest based unit tests
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @brief File Input/Output Tests checking integrity of lre::Component/lre::Set data sets
 */

// --- GTest --- //
#include <gtest/gtest.h>

// --- LRE --- //
#include <lre/ReplaceEngine.h>
#include <lre/FileUtils.h>

class FileIOTest : public ::testing::Test {
  public:

    virtual void SetUp()
	{
		filename_ = "./generated-by-test/output.lre";
		lre::FileUtils::makeDirectory(lre::FileUtils::extractDirectory(filename_));
    }
    
    virtual void TearDown()
	{

	}

	std::string filename_;

};

TEST_F(FileIOTest, WriteReadData)
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

	std::string content_test = lre::FileUtils::getFile(filename_);
	EXPECT_TRUE(content_test == "<LRE:COMPONENT:Command>"+lre::FileUtils::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:description_de>Importiert eine Geometriedatei</LRE:KEY:description_de>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:description_en>Imports a geometry file</LRE:KEY:description_en>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:name>import</LRE:KEY:name>"+lre::FileUtils::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:description_de>Beendet die Anwendung</LRE:KEY:description_de>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:description_en>Exits application</LRE:KEY:description_en>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:name>exit</LRE:KEY:name>"+lre::FileUtils::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"</LRE:COMPONENT:Command>"+lre::FileUtils::getNativeEndline()+
								"<LRE:COMPONENT:Users>"+lre::FileUtils::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:name>jos</LRE:KEY:name>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:password>abc123</LRE:KEY:password>"+lre::FileUtils::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"	<LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:name>mw</LRE:KEY:name>"+lre::FileUtils::getNativeEndline()+
								"		<LRE:KEY:password>dummy</LRE:KEY:password>"+lre::FileUtils::getNativeEndline()+
								"	</LRE:SET>"+lre::FileUtils::getNativeEndline()+
								"</LRE:COMPONENT:Users>"+lre::FileUtils::getNativeEndline()+
								"");

	// Create another ReplaceEngine to read the file
	lre::ReplaceEngine re2;
	EXPECT_TRUE(re2.loadData(filename_));

	EXPECT_EQ(2, re2.getNumComponents());

	lre::Component* test_commands = re2.getComponent("Command");
	EXPECT_TRUE(test_commands != NULL);
	EXPECT_EQ(2, test_commands->getNumSets());

	{
		lre::Set* tmp_set = test_commands->getSet(0);
		// We should have three key/value pairs
		EXPECT_EQ(3, tmp_set->getMap().size());

		lre::StringStringMap::const_iterator itr = tmp_set->getMap().begin();
		EXPECT_EQ("description_de", itr->first);
		EXPECT_EQ("Importiert eine Geometriedatei", itr->second);

		++itr;
		EXPECT_EQ("description_en", itr->first);
		EXPECT_EQ("Imports a geometry file", itr->second);

		++itr;
		EXPECT_EQ("name", itr->first);
		EXPECT_EQ("import", itr->second);
	}
	
	{
		lre::Set* tmp_set = test_commands->getSet(1);
		// We should have three key/value pairs
		EXPECT_EQ(3, tmp_set->getMap().size());

		lre::StringStringMap::const_iterator itr = tmp_set->getMap().begin();
		EXPECT_EQ("description_de", itr->first);
		EXPECT_EQ("Beendet die Anwendung", itr->second);

		++itr;
		EXPECT_EQ("description_en", itr->first);
		EXPECT_EQ("Exits application", itr->second);

		++itr;
		EXPECT_EQ("name", itr->first);
		EXPECT_EQ("exit", itr->second);
	}

	lre::Component* test_users = re2.getComponent("Users");
	EXPECT_TRUE(test_users != NULL);
	EXPECT_EQ(2, test_users->getNumSets());

	{
		lre::Set* tmp_set = test_users->getSet(0);
		// We should have three key/value pairs
		EXPECT_EQ(2, tmp_set->getMap().size());

		lre::StringStringMap::const_iterator itr = tmp_set->getMap().begin();
		EXPECT_EQ("name", itr->first);
		EXPECT_EQ("jos", itr->second);

		++itr;
		EXPECT_EQ("password", itr->first);
		EXPECT_EQ("abc123", itr->second);
	}
	
	{
		lre::Set* tmp_set = test_users->getSet(1);
		// We should have three key/value pairs
		EXPECT_EQ(2, tmp_set->getMap().size());

		lre::StringStringMap::const_iterator itr = tmp_set->getMap().begin();
		EXPECT_EQ("name", itr->first);
		EXPECT_EQ("mw", itr->second);

		++itr;
		EXPECT_EQ("password", itr->first);
		EXPECT_EQ("dummy", itr->second);
	}
}


