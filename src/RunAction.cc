/*
 * Copyright 2024 Holger Kluck
 * This file runAction.cc is part of G4minWE.
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

#include "../include/runAction.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"

G4minWE::RunAction::RunAction() {
	//Get instance of the analysis manager, because we include
	//g4root.hh we will get a G4RootAnalysisManager
	anaMgr = G4AnalysisManager::Instance();
	//Create Ntuple
	anaMgr->CreateNtuple(
			"cube",              //Name of the Ntuple
			"Data from cube SD"  //Description of the Ntuple
			);
	//Create a column of doubles
	anaMgr->CreateNtupleDColumn("Edep");
	//Finalize the Ntuple
	anaMgr->FinishNtuple();
	//Create 1D histogram
	anaMgr->CreateH1(
			"cube_Edep",                    //Name of the histogram
			"Energy deposition in cube CS", //Title of the histogram
			1000,                           //1000 bins ...
			0.,                             //between 0 ...
			100.*keV                        //and 100 keV
			);
}

void G4minWE::RunAction::BeginOfRunAction(const G4Run*) {
	//Open the output file
	G4String fileName = "cube.root";
	anaMgr->OpenFile(fileName);
}

void G4minWE::RunAction::EndOfRunAction(const G4Run*) {
	//Write data to file
	anaMgr->Write();
	//Close file
	anaMgr->CloseFile();
}

