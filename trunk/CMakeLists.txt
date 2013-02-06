cmake_minimum_required(VERSION 2.8)
 
PROJECT(LRE)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")
SET(CMAKE_DEBUG_POSTFIX "d")

OPTION(CMAKE_SUPPRESS_REGENERATION "CMAKE_SUPPRESS_REGENERATION" OFF)
OPTION(CMAKE_USE_RELATIVE_PATHS "CMAKE_USE_RELATIVE_PATHS" ON)

#######################################
# Versions
#######################################
SET(LRE_MAJOR_VERSION 1)
SET(LRE_MINOR_VERSION 0)
SET(LRE_PATCH_VERSION 1)

SET(LRE_VERSION ${LRE_MAJOR_VERSION}.${LRE_MINOR_VERSION}.${LRE_PATCH_VERSION})

#######################################
# Library files
#######################################
SET(INSTALL_BINDIR "bin")
SET(INSTALL_LIBDIR "lib")
SET(INSTALL_ARCHIVEDIR "lib")
SET(INSTALL_INCDIR "include")

OPTION(LRE_BUILD_SHARED_LIBS "Build LRE shared libraries (SOs/DLLs)." ON)
IF (LRE_BUILD_SHARED_LIBS)
    SET(LRE_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
    SET(LABEL_LRE_USER_DEFINED_DYNAMIC_OR_STATIC "Shared")
ELSE (LRE_BUILD_SHARED_LIBS)
    SET(LRE_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
    SET(LABEL_LRE_USER_DEFINED_DYNAMIC_OR_STATIC "Static")
ENDIF(LRE_BUILD_SHARED_LIBS)

OPTION(LRE_BUILD_EXAMPLES "Set to ON to build LRE examples." ON)
OPTION(LRE_BUILD_TESTS "Set to ON to build LRE tests." ON)

SET(HOME_DIR $ENV{HOME})

IF("${HOME_DIR}" STREQUAL "")
	SET(LRE_SHARE_DEFAULT "${CMAKE_INSTALL_PREFIX}/share/lre")
ELSE("${HOME_DIR}" STREQUAL "")
	SET(LRE_SHARE_DEFAULT "$ENV{HOME}/lre")	
ENDIF("${HOME_DIR}" STREQUAL "")

SET(LRE_SHARE_INSTALLDIR ${LRE_SHARE_DEFAULT} CACHE PATH "Path where to install data ('./data'), examples and tests ('./bin')")
SET(LRE_SHARE_INSTALLDIR_BIN "${LRE_SHARE_INSTALLDIR}/bin")
SET(LRE_SHARE_INSTALLDIR_DATA "${LRE_SHARE_INSTALLDIR}/data")

# Tell the user to set the LD_LIBRARY_PATH appropriately if not installing to /usr
IF(UNIX)
	IF(NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
		MESSAGE("Your install path is set to ${CMAKE_INSTALL_PREFIX}, thus your system might not find the library files at ${CMAKE_INSTALL_PREFIX}/lib.")
		MESSAGE("Before running LRE be sure to set the LD_LIBRARY_PATH appropriately to your library install path")
		MESSAGE("On bash use: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${CMAKE_INSTALL_PREFIX}/lib")
	ENDIF(NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
ENDIF(UNIX)

INCLUDE_DIRECTORIES(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

ADD_SUBDIRECTORY(src)

IF(LRE_BUILD_EXAMPLES)
	OPTION(LRE_INSTALL_EXAMPLES "Set to ON to install LRE examples." ON)
	ADD_SUBDIRECTORY(examples)
ENDIF(LRE_BUILD_EXAMPLES)

IF(LRE_BUILD_TESTS)
	OPTION(LRE_INSTALL_TESTS "Set to ON to install LRE examples." ON)
	ADD_SUBDIRECTORY(test)
ENDIF(LRE_BUILD_TESTS)

# Both examples and tests require the data to be installed
IF((LRE_BUILD_EXAMPLES OR LRE_BUILD_TESTS) AND (LRE_INSTALL_EXAMPLES OR LRE_INSTALL_TESTS))
	ADD_SUBDIRECTORY(data)
ENDIF((LRE_BUILD_EXAMPLES OR LRE_BUILD_TESTS) AND (LRE_INSTALL_EXAMPLES OR LRE_INSTALL_TESTS))

ADD_SUBDIRECTORY(applications)

################################################################################
### uninstall target
################################################################################
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
