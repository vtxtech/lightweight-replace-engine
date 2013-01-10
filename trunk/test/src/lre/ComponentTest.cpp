/*
 * Lightweight Replace Engine - GoogleTest based unit tests
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// --- GTest --- //
#include <gtest/gtest.h>

// --- LRE --- //
#include <lre/Component.h>
#include <lre/FileUtil.h>

class ComponentTest : public ::testing::Test {
  public:

    virtual void SetUp()
	{
		content_ =	"<LRE:COMPONENT:Command>"+lre::FileUtil::getNativeEndline()+
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
					"</LRE:COMPONENT:Command>";
    }
    
    virtual void TearDown()
	{

	}

	std::string content_;

};

TEST_F(ComponentTest, ComponentToString)
{
	lre::Component comp1("Command");
	lre::Set* set1 = comp1.addSet();
	set1->addPair("name", "import");
	set1->addPair("description_en", "Imports a geometry file");
	set1->addPair("description_de", "Importiert eine Geometriedatei");
	lre::Set* set2 = comp1.addSet();
	set2->addPair("name", "exit");
	set2->addPair("description_en", "Exits application");
	set2->addPair("description_de", "Beendet die Anwendung");

	EXPECT_TRUE(comp1.toString() == content_);
}

TEST_F(ComponentTest, ComponentFromString)
{
	lre::Component comp("");
	EXPECT_TRUE(comp.fromString(content_));

	EXPECT_EQ("Command", comp.getName());

	EXPECT_EQ(2, comp.getSetCount());

	// *** Check contents of Set #1 *** 
	lre::Set* set1 = comp.getSet(0);

	// We should have three key/value pairs
	ASSERT_EQ(3, set1->getMap().size());

	lre::StringStringMap::const_iterator itr = set1->getMap().begin();
	EXPECT_EQ("description_de", itr->first);
	EXPECT_EQ("Importiert eine Geometriedatei", itr->second);

	++itr;
	EXPECT_EQ("description_en", itr->first);
	EXPECT_EQ("Imports a geometry file", itr->second);

	++itr;
	EXPECT_EQ("name", itr->first);
	EXPECT_EQ("import", itr->second);

	// *** Check contents of Set #2 ***
	lre::Set* set2 = comp.getSet(1);

	// We should have three key/value pairs
	ASSERT_EQ(3, set2->getMap().size());

	lre::StringStringMap::const_iterator itr2 = set2->getMap().begin();
	EXPECT_EQ("description_de", itr2->first);
	EXPECT_EQ("Beendet die Anwendung", itr2->second);

	++itr2;
	EXPECT_EQ("description_en", itr2->first);
	EXPECT_EQ("Exits application", itr2->second);

	++itr2;
	EXPECT_EQ("name", itr2->first);
	EXPECT_EQ("exit", itr2->second);
}


