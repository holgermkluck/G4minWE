/*
 * Copyright 2024 Holger Kluck
 * This file main.cc is part of G4minWE.
 *
 * G4minWE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * G4minWE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with G4minWE. If not, see <http://www.gnu.org/licenses/>.
 */

#include "detectorConstruction.hh"

#include "G4UIExecutive.hh"
#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "Shielding.hh"

int main(int argc, char **argv) {
	/*-Info printout-----------------------------------------------------------*/
	G4cout
	<< "            --- G4minWE ---\n"
	<< " A minimum working example for Geant4\n"
	<< "\n"
	<< " Usage:\n"
	<< "  for interactive mode: gme\n"
	<< "  for batch mode:...... gme <path/to/macroFile>\n"
	<< "  where <path/to/macroFile> is the path to the macro file to be executed.\n"
	<< "\n"
	<< " Contact: Holger Kluck (holger.kluck@oeaw.ac.at)\n"
	<< "\n"
	<< " G4minWE Copyright (C) 2024 Holger Kluck\n"
	<< "  This program comes with ABSOLUTELY NO WARRANTY; this is free software,\n"
	<< "  and you are welcome to redistribute it under certain conditions;\n"
	<< "  see LICENSE.md for details." << G4endl;

	/*-Setup user interface----------------------------------------------------*/
	G4UIExecutive *ui = nullptr;
	if(argc == 1){
		//If no command line argument is given (1st argument id always by default
		//the program name), then instantiate UI for interactive session.
		ui = new G4UIExecutive(argc, argv);
	}

	/*-Setup run manager and user classes--------------------------------------*/
	auto* runMgr = new G4RunManager;
	//Set the detector construction
	runMgr->SetUserInitialization(new G4minWE::DetectorConstruction);
	//Set the physics list
	runMgr->SetUserInitialization(new Shielding);

	/*-Initialise visualisation manager----------------------------------------*/
	G4VisManager* visMgr = new G4VisExecutive;
	visMgr->Initialize();

	/*-Start user session------------------------------------------------------*/
	auto* uiMgr = G4UImanager::GetUIpointer();
	if(!ui){
		//ui pointer is nullptr -> we're in batch mode, so pass path to macro
		//file to the UI manager
		G4String command = "/control/execute ";
		G4String path = argv[1];
		uiMgr->ApplyCommand(command + path);
	}else{
		//ui pointer is not nullptr -> we're in interactive mode, so start
		//interactive session
		ui->SessionStart();
		delete ui;
	}

	/*-Clean-up----------------------------------------------------------------*/
	delete visMgr;
	delete runMgr;

	return 0;
}



