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
#include <lre/Set.h>
#include <lre/FileUtils.h>

class SetTest : public ::testing::Test {
  public:

    virtual void SetUp()
	{
		content_ =	"	<LRE:SET>"+lre::FileUtils::getNativeEndline()+
					"		<LRE:KEY:description_de>Importiert eine Geometriedatei</LRE:KEY:description_de>"+lre::FileUtils::getNativeEndline()+
					"		<LRE:KEY:description_en>Imports a geometry file</LRE:KEY:description_en>"+lre::FileUtils::getNativeEndline()+
					"		<LRE:KEY:name>import</LRE:KEY:name>"+lre::FileUtils::getNativeEndline()+
					"	</LRE:SET>";
    }
    
    virtual void TearDown()
	{

	}

	std::string content_;

};

TEST_F(SetTest, SetToString)
{
	lre::Set set1;
	set1.addPair("name", "import");
	set1.addPair("description_en", "Imports a geometry file");
	set1.addPair("description_de", "Importiert eine Geometriedatei");

	EXPECT_TRUE(set1.toString() == content_);
}

TEST_F(SetTest, SetFromString)
{
	lre::Set set1;
	EXPECT_TRUE(set1.fromString(content_));

	// We should have three key/value pairs
	EXPECT_EQ(3, set1.getMap().size());

	lre::StringStringMap::const_iterator itr = set1.getMap().begin();
	EXPECT_EQ("description_de", itr->first);
	EXPECT_EQ("Importiert eine Geometriedatei", itr->second);

	++itr;
	EXPECT_EQ("description_en", itr->first);
	EXPECT_EQ("Imports a geometry file", itr->second);

	++itr;
	EXPECT_EQ("name", itr->first);
	EXPECT_EQ("import", itr->second);
}


