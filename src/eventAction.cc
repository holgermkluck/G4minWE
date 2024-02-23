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

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"

#include <map>

//Need constructor only to set the pointer to the analysis manager
eventAction::eventAction() : anaMgr(G4AnalysisManager::Instance()) {
}

void eventAction::EndOfEventAction(const G4Event* anEvent) {

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

	//2)   Select the hit collection of scrorer "edep" of SD "cube"
	//2.1) Get the ID of the scorer "edep" of SD "cube"
	G4int id = G4SDManager::GetSDMpointer()->GetCollectionID("cube/edep");
	//2.2) With the ID select the HC
	auto* hitCol = hce->GetHC(id);
	//2.3) Cast the collection to a map
	G4THitsMap<G4double>* hitMap = static_cast<G4THitsMap<G4double>*>(hitCol);

	//3)   Iterate over the entries in the map; the entries a pairs of <key, value>
	//     we are interest in the value, i.e. the second element of a pair.
	//     The value is the quantity measured by the scorer, i.e. for the "cube/edep"
	//     it is the energy deposited inside the SD cube.
	std::map<G4int,G4double*>::iterator itr;
	for (itr = hitMap->GetMap()->begin(); itr != hitMap->GetMap()->end(); itr++){
		//The iterator itr points to an pair, to get the second element, i.e.
		//the value of the pair, do:
		G4double eDep = *(itr->second);
		//We want the energy in multiples of MeV, so divide it by MeV
		G4cout << "Energy deposited in cube: " << eDep/MeV << " MeV\n";
		//Fill energy into Ntuple and histogram
		//(one has to know that "cube_Edep" histogram was the first
		//created in runAction, i.e. that it has the ID=0)
		anaMgr->FillH1(
				0,     //ID of the histogram to fill
				eDep   //Value to fill in the histogram
				);
		anaMgr->FillNtupleDColumn(
				0,     //ID of the histogram to fill
				eDep   //Value to fill in the histogram
				);
		anaMgr->AddNtupleRow();
	}

}

