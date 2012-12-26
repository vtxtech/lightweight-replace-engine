cmake_minimum_required(VERSION 2.8)
 
PROJECT(LRE)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
SET(CMAKE_DEBUG_POSTFIX "d")

#######################################
# Versions
#######################################
SET(LRE_MAJOR_VERSION 0)
SET(LRE_MINOR_VERSION 1)
SET(LRE_PATCH_VERSION 0)

SET(LRE_VERSION ${LRE_MAJOR_VERSION}.${LRE_MINOR_VERSION}.${LRE_PATCH_VERSION})

#######################################
# Library files
#######################################
SET(INSTALL_BINDIR "bin")
SET(INSTALL_LIBDIR "lib")
SET(INSTALL_ARCHIVEDIR "lib")
SET(INSTALL_INCDIR "include")

OPTION(LRE_BUILD_SHARED_LIBS "Set to ON to build LRE for dynamic linking.  Use OFF for static." ON)
IF (LRE_BUILD_SHARED_LIBS)
    SET(LRE_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
ELSE (LRE_BUILD_SHARED_LIBS)
    SET(LRE_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
ENDIF(LRE_BUILD_SHARED_LIBS)

OPTION(LRE_BUILD_EXAMPLES "Set to ON to build LRE examples." ON)
OPTION(LRE_BUILD_TESTS "Set to ON to build LRE tests." ON)
#OPTION(LRE_INSTALL_DATA "Set to ON to install LRE data." ON)
#SET(LRE_DATA_INSTALLDIR $ENV{OSG_FILE_PATH} CACHE PATH "Path where to install LRE data")
SET(LRE_EXAMPLES_INSTALLDIR "${CMAKE_INSTALL_PREFIX}/share/LRE/bin" CACHE PATH "Path where to install examples")
SET(LRE_TESTS_INSTALLDIR "${CMAKE_INSTALL_PREFIX}/share/LRE/bin" CACHE PATH "Path where to install examples")

INCLUDE_DIRECTORIES(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

ADD_SUBDIRECTORY(src)

IF(LRE_BUILD_EXAMPLES)
	OPTION(LRE_INSTALL_EXAMPLES "Set to ON to install LRE examples." ON)
	ADD_SUBDIRECTORY(examples)
ENDIF(LRE_BUILD_EXAMPLES)

IF(LRE_BUILD_TESTS)
#	FIND_PACKAGE( GTest ) # not required -> source included
	OPTION(LRE_INSTALL_TESTS "Set to ON to install LRE examples." ON)
	ADD_SUBDIRECTORY(test)
ENDIF(LRE_BUILD_TESTS)

#ADD_SUBDIRECTORY(data)

################################################################################
### uninstall target
################################################################################
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstallLRE
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
