/*
 * Lightweight Replace Engine - Application
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <lre/ReplaceEngine.h>

int main(int argc, char** argv)
{
	// Create a ReplaceEngine to do the job
	lre::ReplaceEngine re;
	
	// Handle command line arguments, report error, display some help
	int result = re.init(argc, argv);
	if (result != 0) { return result; }

	// Finally to the work
	return re.run();
}

