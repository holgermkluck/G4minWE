/*
 * Copyright 2024 Holger Kluck
 * This file eventAction.cc is part of G4minWE.
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

#include "eventAction.hh"
#include "hit.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

void G4minWE::EventAction::EndOfEventAction(const G4Event* anEvent) {
	//After the current event is finished, process the "hits" recorded
	//by the scorer "edep" of SD "cube" to get the energy deposited
	//inside "cube"

	//1) Get "hit collection" (hc) of this event, i.e. collection
	//   of _all_ hits recorded during the simulation of the current event
	//   by _all_ SDs
	auto* hce = anEvent->GetHCofThisEvent();
	if(!hce){
		//If a nullptr was returned, then there were no hits collected
		//during the current event. Nothing to do here, so end this method.
		return;
	}

	//2)   Select the hit collection of sensitive detector "pixelHC" of SD "pixel"
	//2.1) Get the ID of the hit collection "pixelHC"
	G4int id = G4SDManager::GetSDMpointer()->GetCollectionID("pixelHC");
	//2.2) With the ID select the HC
	auto* hitCol = hce->GetHC(id);
	//2.3) Get a vector
	auto* hitVec = static_cast<G4minWE::HitsCollection*>(hitCol)->GetVector();

	//3)   Iterate over the entries in the vector; the entries a pairs a pointers
	//     to hits
	for (auto* hit : *hitVec){
		//Get the data from the hit
		G4double eDep = hit->GetEnergyDeposit();
		G4int pix_x = hit->GetPixelX();
		G4int pix_y = hit->GetPixelY();
		G4int eventID = anEvent->GetEventID();
		//If verbosity is at least 1, then print the energy to screen
		if(evtMgr->GetVerboseLevel() >= 1){
			//We want the energy in multiples of MeV, so divide it by MeV
			G4cout << "Energy deposited in pixel(" <<
			 pix_x << " | " << pix_y << "): " << eDep/MeV << " MeV" << G4endl;
		}
		//Fill energy into Ntuple and histogram
		//(one has to know that "pixel_map" histogram was the first
		//created in runAction, i.e. that it has the ID=0; similarly
		//the IDs of the columns Edep, PixX, PixY, EventID are 
		//0, 1, 2, 3, respectively)
		anaMgr->FillH2(
				0,     //ID of the histogram to fill
				pix_x, //x-coordinate of the bin
				pix_y, //y-coordinate of the bin
				eDep   //Bin value
				);
		anaMgr->FillNtupleDColumn(
				0,     //ID of the column to fill
				eDep   //Value to fill in the column
				);
		anaMgr->FillNtupleDColumn(
				1,     //ID of the column to fill
				pix_x  //Value to fill in the column
				);
		anaMgr->FillNtupleDColumn(
				2,     //ID of the column to fill
				pix_y  //Value to fill in the column
				);
		anaMgr->FillNtupleDColumn(
				3,      //ID of the column to fill
				eventID //Value to fill in the column
				);
		anaMgr->AddNtupleRow();

	}

}
