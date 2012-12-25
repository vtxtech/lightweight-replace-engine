// unitTest.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}

