/*
 * Lightweight Replace Engine - GoogleTest based unit tests
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

// --- GTest --- //
#include <gtest/gtest.h>

// --- LRE --- //
//#include <blabla.h>

// --- STL --- //
//#include <string>
//#include <stdio.h>
//#include <math.h>

class LRETest : public ::testing::Test {
  public:

    virtual void SetUp()
	{
    }
    
    virtual void TearDown()
	{

	}

};

TEST_F(LRETest, SomeTest)
{
//	EXPECT_TRUE(client_.valid());
}


