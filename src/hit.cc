/*
 * Copyright 2024 Holger Kluck
 * This file hit.cc is part of G4minWE.
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

#include "hit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>

G4bool Hit::operator ==(const Hit &right) const {
	return (this == &right) ? true : false;
}

void Hit::operator delete(void *hit) {
	TrackerHitAllocator->FreeSingle((Hit*) hit);
}

void Hit::Draw() {
	auto *pVVisManager = G4VVisManager::GetConcreteInstance();
	if (pVVisManager) {
		G4Circle circle(Position);
		circle.SetScreenSize(4.);
		circle.SetFillStyle(G4Circle::filled);
		G4VisAttributes visAttribs(G4Colour::Blue());
		circle.SetVisAttributes(visAttribs);
		pVVisManager->Draw(circle);
	}
}

void Hit::Print() {
	G4cout
	<< " Edep: " << std::setw(7) << G4BestUnit(EnergyDeposit, "Energy")
	<< " Position: " << std::setw(7) << G4BestUnit(Position, "Length")
	<< G4endl;

}

void Hit::SetEnergDeposit(G4double edep) {
	EnergyDeposit = edep;
}

void Hit::SetPosition(const G4ThreeVector &pos) {
	Position = pos;
}

G4double Hit::GetEnergyDeposit() const {
	return EnergyDeposit;
}

G4ThreeVector Hit::GetPosition() const {
	return Position;
}

