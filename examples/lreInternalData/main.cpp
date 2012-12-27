/*
 * Lightweight Replace Engine - Example for build-in data
 * Copyright (C) 2012-2013 Johannes Scholz. All rights reserved.
 *
 * This file is licensed under the GNU Lesser General Public License 3 (LGPLv3),
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
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
	
	// Handle command line arguments, report error, display some help
	int result = re.init(argc, argv);
	if (result != 0) { return result; }

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

	lre::Component* users = re.addComponent("Users");
	lre::Set* set11 = users->addSet();
	set11->addPair("name", "jos");
	set11->addPair("password", "abc123");
	lre::Set* set12 = users->addSet();
	set12->addPair("name", "mw");
	set12->addPair("password", "dummy");

	// Finally to the work
	return re.run();
}

