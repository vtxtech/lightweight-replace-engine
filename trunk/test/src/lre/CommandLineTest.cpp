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
#include <lre/ReplaceEngine.h>

TEST(CommandLineTest, ArgumentsCheck)
{
	const char* argv[] = { "lre", "--input", "./input-path/", "--recursive",
		"--keepExtension", "--forgetSubfolders", "--noFinalAppendix",
		"--pattern", "*.test", "--output", "./output-path/", "--data",
		"./data-path/", "--dataPattern", "*.data", "--appendix", "foo"};
	int   argc   = sizeof(argv) / sizeof(argv[0]);
	// -R -K -F

	lre::ReplaceEngine re;
	// First check the defaults here:
	EXPECT_FALSE(re.settings().getRecursive());
	EXPECT_EQ("", re.settings().getInput());
	EXPECT_EQ("", re.settings().getOutputDirectory());
	EXPECT_EQ("*.in", re.settings().getFilePattern());
	EXPECT_TRUE(re.settings().getRemoveExtension());
	EXPECT_TRUE(re.settings().getKeepSubFolders());
	EXPECT_EQ("", re.settings().getDataDirectory());
	EXPECT_EQ("*.lre", re.settings().getDataPattern());
	EXPECT_EQ("", re.settings().getAppendixString());
	EXPECT_TRUE(re.settings().getAddAppendixAfterLastSet());

	// Update settings from fake command arguments
	re.init(argc, argv);

	// Now check everything changed:
	EXPECT_TRUE(re.settings().getRecursive());
	EXPECT_EQ("./input-path/", re.settings().getInput());
	EXPECT_EQ("./output-path/", re.settings().getOutputDirectory());
	EXPECT_EQ("*.test", re.settings().getFilePattern());
	EXPECT_FALSE(re.settings().getRemoveExtension());
	EXPECT_FALSE(re.settings().getKeepSubFolders());
	EXPECT_EQ("./data-path/", re.settings().getDataDirectory());
	EXPECT_EQ("*.data", re.settings().getDataPattern());
	EXPECT_EQ("foo", re.settings().getAppendixString());
	EXPECT_FALSE(re.settings().getAddAppendixAfterLastSet());

	// Short arguments test
	const char* argv2[] = { "lre", "-R", "-F", "-K"};
	int   argc2   = sizeof(argv2) / sizeof(argv2[0]);
	
	lre::ReplaceEngine re2;
	// Again, first check the defaults:
	EXPECT_FALSE(re2.settings().getRecursive());
	EXPECT_TRUE(re2.settings().getRemoveExtension());
	EXPECT_TRUE(re2.settings().getKeepSubFolders());

	// Update settings from fake command arguments
	re2.init(argc2, argv2);

	// Now check everything changed:
	EXPECT_TRUE(re2.settings().getRecursive());
	EXPECT_FALSE(re2.settings().getRemoveExtension());
	EXPECT_FALSE(re2.settings().getKeepSubFolders());
}


