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
#include <lre/FileUtils.h>

class FileUtilsTest : public ::testing::Test {
  public:

    virtual void SetUp()
	{
		stdInput_ = "./data\\test/some.thing.in";
    }
    
    virtual void TearDown()
	{

	}

	std::string stdInput_;

};

TEST_F(FileUtilsTest, FileNames)
{
	EXPECT_EQ("./data\\test/some.thing", lre::FileUtils::removeExtension(stdInput_));
	EXPECT_EQ("some.thing.in", lre::FileUtils::extractFilename(stdInput_));
	EXPECT_EQ("./data\\test", lre::FileUtils::extractDirectory(stdInput_));
	EXPECT_EQ("./data\\test", lre::FileUtils::excludeTrailingSeparator("./data\\test/"));
	// Note that includeTrailingSeparator removes the last separator if existing and always adds the native separator
	EXPECT_EQ("./data\\test"+lre::FileUtils::separator(), lre::FileUtils::includeTrailingSeparator("./data\\test/"));
#if defined( __APPLE__ )
	EXPECT_EQ("\r", lre::FileUtils::getNativeEndline());
#else
	EXPECT_EQ("\n", lre::FileUtils::getNativeEndline());
#endif

#if defined(WIN32) && !defined(__CYGWIN__)
	EXPECT_EQ("\\", lre::FileUtils::separator());
#else
	EXPECT_EQ("/", lre::FileUtils::separator());
#endif

}

TEST_F(FileUtilsTest, Files)
{
	EXPECT_TRUE(lre::FileUtils::makeDirectory("./created-by-test/subfolder/another-folder"));
	EXPECT_TRUE(lre::FileUtils::makeDirectory("./created-by-test/subfolder"));
	EXPECT_FALSE(lre::FileUtils::makeDirectory("./created-by-test/subfolder/another-folder/"));
	EXPECT_FALSE(lre::FileUtils::makeDirectory("./created-by-test/subfolder/"));

	EXPECT_TRUE(lre::FileUtils::fileExists("./created-by-test/subfolder/"));
	EXPECT_FALSE(lre::FileUtils::fileExists("./created-by-test/subfolder/xx/"));

	EXPECT_FALSE(lre::FileUtils::putFile("./created-by-test/subfolder/xx/some.file", "some text"));
	EXPECT_TRUE(lre::FileUtils::putFile("./created-by-test/my.file", "some text"));
	EXPECT_TRUE(lre::FileUtils::putFile("./created-by-test/subfolder/another.file", "some text"));
	EXPECT_TRUE(lre::FileUtils::putFile("./created-by-test/subfolder/file.other", "some text"));
	EXPECT_EQ("some text", lre::FileUtils::getFile("./created-by-test/my.file"));

	std::vector<std::string> files = lre::FileUtils::findFiles("./created-by-test", "*.file", true);
	ASSERT_EQ(2, files.size());
	EXPECT_EQ("my.file", lre::FileUtils::extractFilename(files.at(0)));
	EXPECT_EQ("another.file", lre::FileUtils::extractFilename(files.at(1)));
	files = lre::FileUtils::findFiles("./created-by-test", "*.file", false);
	ASSERT_EQ(1, files.size());
	EXPECT_EQ("my.file", lre::FileUtils::extractFilename(files.at(0)));
	files = lre::FileUtils::findFiles("./created-by-test", "*.*", true);
	ASSERT_EQ(3, files.size());
	EXPECT_EQ("my.file", lre::FileUtils::extractFilename(files.at(0)));
	EXPECT_EQ("another.file", lre::FileUtils::extractFilename(files.at(1)));
	EXPECT_EQ("file.other", lre::FileUtils::extractFilename(files.at(2)));

}
