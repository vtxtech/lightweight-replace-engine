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
	// lre::FileUtils::findFiles
	// lre::FileUtils::fileExists
	// lre::FileUtils::getFile
	// lre::FileUtils::putFile
}
