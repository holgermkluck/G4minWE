/*
 * Copyright 2024 Holger Kluck
 * This file sensitiveDetector.cc is part of G4minWE.
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

#include "sensitiveDetector.hh"

//#include "hit.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include <cstddef>

SensitiveDetector::SensitiveDetector(const G4String &name,
		const G4String &hitsCollectionName) :
		G4VSensitiveDetector(name) {
	collectionName.insert(hitsCollectionName);
}

void SensitiveDetector::Initialize(G4HCofThisEvent *hitCollection) {
	HCollection = new HitsCollection(SensitiveDetectorName, collectionName[0]);

	//Add this collection to hce
	G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(
			collectionName[0]);
	hitCollection->AddHitsCollection(hcID, HCollection);

}

G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory*) {
	//Get energy deposit by current hit
	G4double edep = step->GetTotalEnergyDeposit();
	//If no energy is deposit, nothing to do
	if (edep == 0.) {
		return false;
	}
	//Otherwise create a new hit
	Hit *newHit = new Hit();
	//And set the data
	newHit->SetEnergyDeposit(edep);
	newHit->SetPosition(step->GetPostStepPoint()->GetPosition());

	HCollection->insert(newHit);

	return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
	if (verboseLevel > 1) {
		std::size_t nbHits = HCollection->entries();
		G4cout << "\n" << "-------->Hits Collection: in this event they are "
				<< nbHits << " hits: " << G4endl;
		for (std::size_t i = 0; i < nbHits; ++i) {
			(*HCollection)[i]->Print();
		}
	}

}
