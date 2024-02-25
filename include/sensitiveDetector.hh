/*
 * Copyright 2024 Holger Kluck
 * This file sensitiveDetector.hh is part of G4minWE.
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

#ifndef INCLUDE_SENSITIVEDETECTOR_HH_
#define INCLUDE_SENSITIVEDETECTOR_HH_

#include "hit.hh"

#include "G4VSensitiveDetector.hh"

class SensitiveDetector: public G4VSensitiveDetector {
public:
	SensitiveDetector(const G4String &name, const G4String &hitsCollectionName);
	~SensitiveDetector() override = default;

	// methods from base class
	void Initialize(G4HCofThisEvent *hitCollection) override;
	G4bool ProcessHits(G4Step *step, G4TouchableHistory *history) override;
	void EndOfEvent(G4HCofThisEvent *hitCollection) override;

private:
	HitsCollection *HitsCollection {nullptr};
};

#endif /* INCLUDE_SENSITIVEDETECTOR_HH_ */
