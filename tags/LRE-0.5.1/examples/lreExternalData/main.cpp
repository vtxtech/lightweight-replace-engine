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
	
	// Ignore any command line arguments
	// we setup everything manually here.
	re.setInput("../data");
	re.setOutputDirectory("./generated");
	re.setRecursive(true);
	re.setRemoveExtension(true);
	re.setFilePattern("*.in");
	re.setAppendixString("\n");
	re.setAddAppendixAfterLastSet(false);
	
	// Define where to find our replacement data (lre::Component and lre::Set)
	re.setDataDirectory("../data/data-multifile");
	re.setDataPattern("*.lre");

	// Finally to the work
	return re.run();
}

