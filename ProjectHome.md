# News #
## 2014-10-31: Visual Studio 2013 also supported ##
Around the last months I migrated my projects to MSVC 2013 as main IDE. LRE-1.2.1 runs on Visual Studio 2013 x64 and x86 very fine, as well.

## 2013-11-08: LRE-1.2.1 Stable ##
A new version of LRE is released! LRE-1.2.1 now adds Visual Studio 2012 x64/x86 support.

## 2013-05-16: LRE-1.2.0 Stable ##
A new version of LRE is released! LRE-1.2.0 now adds Visual Studio 2010 and gcc 4.7 support.

# Description #
Lightweight Replace Engine is a easy-to-use text file preprocessor that is used to generate text documents based on template documents. LRE does have no dependencies so it may be used as single file standalone executable for preprocessing any type of text file. LRE may also be used as linked C++ library directly from inside your C++ source code.

LRE is intended to create offline documentation files (e.g. tex, txt or csv) from your application's source code or internal application data.

LRE takes a set of template and data files from an input directory. Inside the input files, LRE searches for keyword-tags that are replaced by the sets of data from the data files. After the replace run the generated files are copied to the target location preserving directory structure.

Actually LRE may also be used to preprocess any programming languages source files where preprocessing by CMake's "configure\_file" or any other build system you use is not applicable. This may be the case if inclusion of runtime information is necessary to generate your target files.

# Downloading #
Source code releases as well as binaries for Visual Studio 2008 and 2010 can be found at the download section. Note that x64 support for Visual Studio 2010 is not tested, yet.

If you are using the repository we recommend checking out the latest stable release of Lightweight Replace Engine from our SVN repository (LRE-1.2.1) which is found at:
```
svn checkout http://lightweight-replace-engine.googlecode.com/svn/tags/LRE-1.2.1/
```
# Building #
For building LRE the cross-platform make tool CMake is required to generate project files suiting your compiler. LRE is both tested on Windows and Linux.

Supported (tested) compilers:
  * Windows: Visual Studio 2008 Express x86 / x64
  * Windows: Visual Studio 2010 Express x86 (x64 untested, yet)
  * Windows: Visual Studio 2012 Express x86 / x64
  * Windows: Visual Studio 2013 Express x86 / x64
  * Linux: gcc 4.6 on Lubuntu 11.04, but LRE should compile on gcc 4.4 or higher, as well.
  * Linux: gcc 4.6 on Ubuntu 12.04
  * Linux: gcc 4.7 on Xubuntu 13.04

We recommend using "cmake-gui" rather than the commandline version if you are not familiar with CMake.
  1. Checkout latest tagged or trunk version of LRE from Subversion (see "Source" page). Remember checkout path, e.g. "/home/my\_user/LRE-trunk" or "C:\LRE\trunk"
  1. Run cmake-gui
    1. Setup source path to your checkout path, e.g. "/home/my\_user/LRE-trunk" or "C:\LRE\trunk"
    1. Setup build path to a folder of your choice, e.g. "/home/my\_user/LRE-build" or "C:\LRE\build"
    1. Press "Configure" button and select your target make tool or IDE (e.g. Visual Studio, Eclipse CDT, make, etc.)
    1. Setup CMAKE\_INSTALL\_PREFIX to the directory where the LRE binaries should be installed to.
    1. Setup LRE\_SHARE\_INSTALLDIR to the directory where data, tests and examples should be installed to.
    1. Press "Configure" button until there are no more red lines and "Generate" button is active
    1. Press "Generate" to finish

On Windows default CMake settings should be fine.

On Linux you might think about either installing into "/usr" instead of "/usr/local" (set your CMAKE\_INSTALL\_PREFIX) or you set the LD\_LIBRARY\_PATH according to your settings (e.g. "/usr/local/lib"). The LRE CMake build script reports some help on that during configuration.

One general hint regardless of which compiler you may use: **Always build the INSTALL project** to make sure your build files are copied to the appropriate locations. When using make that means: Always execute "make", then "make install". When using Visual Studio that means: Right click on the "INSTALL" project, then select "Build".

# Licensing #
LRE is supplied AS-IS without any warranty and is licensed under the GNU Lesser GPL Version 3.0. If you require support feel free to mail us.

# Usage #
LRE features a simple XML-like syntax to define placeholders inside the template documents. During generation run that placeholders are replaced by values from your user defined data sets. LRE comes with a set of examples and test cases documenting most use-cases for easily getting started.

## Files-only example ##
The easiest example of usage is using template input and data files both from disk:

**Template files** usually have a `*`.in extension. Content of example1.csv.in defining the template document:
```
Name;Englisch description;German description
<LRE:COMPONENT:Command><LRE:KEY:name>;<LRE:KEY:description_en>;<LRE:KEY:description_de></LRE:COMPONENT:Command>
```

**Data files** usually end with `*`.lre extension. Contents of data.lre:
```
<LRE:COMPONENT:Command>
	<LRE:SET>
		<LRE:KEY:description_de>Importiert eine Geometriedatei</LRE:KEY:description_de>
		<LRE:KEY:description_en>Imports a geometry file</LRE:KEY:description_en>
		<LRE:KEY:name>import</LRE:KEY:name>
	</LRE:SET>
	<LRE:SET>
		<LRE:KEY:description_de>Beendet die Anwendung</LRE:KEY:description_de>
		<LRE:KEY:description_en>Exits application</LRE:KEY:description_en>
		<LRE:KEY:name>exit</LRE:KEY:name>
	</LRE:SET>
</LRE:COMPONENT:Command>
```

**Running LRE**: LRE may be run on a command line terminal using the lre executable.
```
lre --input <path>/example1.csv.in --data <path>/data.lre --output <output-path> --appendix \n --noFinalAppendix
```
Description of command line arguments used in example (this is only an extract of the arguments available):
```
--input <path or filename>: Sets the input path or filename. Required option.
--data <path or filename>: Data file containg lre::Components or path to multiple files matching --dataPattern. Empty by default. Required option, if no data is defined in source code, only.
--output <path>: Sets the output path. Required option.
--appendix <string>: Define a string (e.g. line break) that shall be appended after each generated set
--noFinalAppendix: Disable appendix string for the last set of a lre::Component
```

**Result files** usually have the last extension of the template file removed. So, then the resulting output file will be: example1.csv.
```
This is a list of the available commands:
Name;Englisch description;German description
import;Imports a geometry file;Importiert eine Geometriedatei
exit;Exits application;Beendet die Anwendung
```

## From-source example ##
LRE may also be run as a linked library from inside your C++ application. Note that only input files are read and any data applied to the templates code is defined from source.

```
/*
 * Lightweight Replace Engine - Example for built-in data
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but distributed WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <lre/ReplaceEngine.h>

//-- STL --//
#include <iostream>

int main(int argc, char** argv)
{
	// Create a ReplaceEngine to do the job
	lre::ReplaceEngine re;
	
	// Ignore any command line arguments
	// we setup everything manually here.
	re.setInput("../data");
	re.setOutputDirectory("./generated");
	re.setRecursive(true);
	re.setRemoveExtension(true);
	re.setFilePattern("*.in");
	re.setAppendixString("\n");
	re.setAddAppendixAfterLastSet(false);
	
	// *re.setDataDirectory(...)* is not required because
	// the data used for replacing is following now:

	// Add the data for file generation here
	lre::Component* cmd = re.addComponent("Command");
	lre::Set* set1 = cmd->addSet();
	set1->addPair("name", "import");
	set1->addPair("description_en", "Imports a geometry file");
	set1->addPair("description_de", "Importiert eine Geometriedatei");
	lre::Set* set2 = cmd->addSet();
	set2->addPair("name", "exit");
	set2->addPair("description_en", "Exits application");
	set2->addPair("description_de", "Beendet die Anwendung");

	// Finally to the work
	return re.run();
}
```

Running the code from above generates output data identical to the first example:
```
This is a list of the available commands:
Name;Englisch description;German description
import;Imports a geometry file;Importiert eine Geometriedatei
exit;Exits application;Beendet die Anwendung
```