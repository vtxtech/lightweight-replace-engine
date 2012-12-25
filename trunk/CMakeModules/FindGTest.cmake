FIND_PATH(GTEST_DIRECTORY "include/gtest/gtest.h"
	"../../../../test/vendor/gtest-1.6.0"
)

FIND_LIBRARY(GTEST_LIBRARY "gtest"
	${GTEST_DIRECTORY}/build/lib
	${GTEST_DIRECTORY}/build/lib/Release
	"../../../../build/lib/Release"
)

FIND_LIBRARY(GTEST_MAIN_LIBARY "gtest_main"
	${GTEST_DIRECTORY}/build/lib
	${GTEST_DIRECTORY}/build/lib/Release
	"../../../../build/lib/Release"
)

SET( GTEST_FOUND "NO" )
IF( GTEST_DIRECTORY AND GTEST_LIBRARY AND GTEST_MAIN_LIBARY )
    SET( GTEST_FOUND "YES" )
ENDIF( GTEST_DIRECTORY AND GTEST_LIBRARY AND GTEST_MAIN_LIBARY )

# vim: set sw=4 ts=8 et ic ai:
