# -----------------------------------------------------------------------------
#
# Lightweight Replace Engine
# Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
#
# This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
# but is distributed WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# -----------------------------------------------------------------------------
# LRE uses some portions of code from the great OpenSceneGraph project which
# are licensed under the OSGPL which is mainly a LGPL-derivate.
# -----------------------------------------------------------------------------
#
# For help on building see GETTING_STARTED.txt
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.1.2.1 (2013-11-08), Stable Release
# --------------------------------------------------------
#
# * Compiler support for Visual Studio 2012 x86 / x64
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.1.2.0 (2013-05-16), Stable Release
# --------------------------------------------------------
#
# * Compiler support for Visual Studio 2010 x86
# * Compiler support for GCC 4.7 x64
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.1.1.0 (2013-03-10), Developer Release
# --------------------------------------------------------
#
# * Added arguments --copyPattern and --copyAll to copy files within the input
#   directory matching a pattern without processing them.
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.1.0.0 (2013-02-06), Stable Release
# --------------------------------------------------------
#
# * Introduced lre::Notify
# * Introduced verbose and silent output modes
# * Tidied up output of lre::ReplaceEngine::run()
# * Added colored console output (Windows only)
# * Added tests for checking handling of invalid input data
# * Fixed a bug where files were not saved if they had not to be processed
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.0.9.0 (2013-02-04), Developer Release
# --------------------------------------------------------
#
# * Formatting of lre::ArgumentParser::reportOptions()
# * Refactored lre::ArgumentParser's internal data structures
# * Fixed FileUtils::removeExtension occasionally returning an empty string
# * Added tests for FileUtils.h
# * Fixed int64 issues on x64 targets
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.0.7.0 (2013-01-20), Developer Release
# --------------------------------------------------------
#
# * Moved settings from lre::ReplaceEngine to separate lre::Settings class
# * Argument parser now reports invalid command arguments and missing parameters
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.0.5.1 (2013-01-17), Developer Release
# --------------------------------------------------------
#
# * Updated README.txt which was outdated in 0.5.0
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.0.5.0 (2013-01-16), Developer Release
# --------------------------------------------------------
#
# * Support of gcc for Linux builds (Tested on gcc 4.6.3)
# * Updated CMake System to support Linux paths more generally
# * Added GETTING_STARTED.txt
# * Fixed crash bug when using filenames on input path or output path
# * Fixed arguments --forgetSubfolders and --keepExtension not changing config
# * Improved test coverage
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.0.3.0 (2013-01-03, Developer Release)
# --------------------------------------------------------
#
# * Added file I/O handling (load/save data)
# * Introduced multiple command line arguments for runnning LRE as standalone application
# * Added GTest-based tests
#
# -----------------------------------------------------------------------------
# Change Notes LRE v.0.1.0 (2012-12-24, Developer Release)
# --------------------------------------------------------
#
# * First release
# * Base functionality running
#
# -----------------------------------------------------------------------------
