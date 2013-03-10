/*
 * Lightweight Replace Engine - Example for out-of-source data
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
	// handle command line args because to allow the user to enable verbose mode
	// using --verbose or -V
	re.init(argc, argv);

	// Ignore any command line arguments
	// we setup everything manually here.
	re.settings().setInput("../data");
	re.settings().setOutputDirectory("./generated-by-lreExternalData");
	re.settings().setRecursive(true);
	re.settings().setRemoveExtension(true);
	re.settings().setFilePattern("*.in");
	re.settings().setAppendixString("\n");
	re.settings().setAddAppendixAfterLastSet(false);
	
	// Define where to find our replacement data (lre::Component and lre::Set)
	re.settings().setDataDirectory("../data/data-multifile");
	re.settings().setDataPattern("*.lre");

	// Finally to the work
	return re.run();
}

