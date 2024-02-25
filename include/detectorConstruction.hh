/*
 * Copyright 2024 Holger Kluck
 * This file detectorConstruction.hh is part of G4minWE.
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

#ifndef INCLUDE_DETECTORCONSTRUCTION_HH_
#define INCLUDE_DETECTORCONSTRUCTION_HH_

#include "G4VUserDetectorConstruction.hh"
class G4VPhysicalVolume;

namespace G4minWE {

class DetectorConstruction : public G4VUserDetectorConstruction {
public:

	//Let C++ define default constructor and destrcutor
	DetectorConstruction() = default;
	~DetectorConstruction() override = default;

	//This method is needed; it will assemble the actual
	//geometry of the setup to be simulated
	G4VPhysicalVolume* Construct() override;
	//This method will create "sensitive detectors"
	void ConstructSDandField() override;
};
}

#endif /* INCLUDE_DETECTORCONSTRUCTION_HH_ */
