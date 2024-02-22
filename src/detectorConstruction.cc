/*
 * Copyright 2024 Holger Kluck
 * This file detectorConstruction.cc is part of G4minWE.
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

//To access predefined materials
#include "G4NistManager.hh"
//To define volumes
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
//To get physics units like meters
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume* detectorConstruction::Construct() {
	//Check for overlapping volumes?
	G4bool checkOverlaps = false;

	//Define a cube of air with 20m edge length as outer most volume, the
	//so-called "world volume"

	//Get a pointer to the manager containing the NIST defined materials
	auto* nistMgr = G4NistManager::Instance();
	//Get a pointer to the "Air" material; for the names of the materials
	//see https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html
	auto* matAir = nistMgr->FindMaterial("G4_AIR");

	//Geant4 use half the edge size to define a cube
	G4double worldHalfLength = 10.*m;
	//The defination f a volume is split in three steps:
	//1) The "solid volume" give the geometrical shape of a volume
	auto* worldVolume_solid = new G4Box(
			"world",          //Name of the solid volume
			worldHalfLength,  //Half-edge length along x-direction
			worldHalfLength,  //Half-edge length along y-direction
			worldHalfLength   //Half-edge length along z-direction
			);
	//2) The "logical volume" fills the solid volume with a material
	auto* worldVoluem_logic = new G4LogicalVolume(
			worldVolume_solid,//The solid volume belong to the logical volume
			matAir,           //The material associate t
			"world"           //The name of the logical volume;
			);                //for convenient the same as for the solid volume
	//3) The "physical volume" rotates and places the logical volume at some
	//   point within an enclosing "mother volume"; if no mother volume
	//   is given, like here, this volume is defined as the mother volume
	//   itself, i.e. the outer most volume
	auto* worldVolume_physic = new G4PVPlacement(
			nullptr,          //No rotation
			G4ThreeVector(),  //Placed at (0,0,0)m; the default value of G4ThreeVector
			"world",          //Name of the physical volume
			worldVoluem_logic,//The logical volume that is placed
			nullptr,          //No mother volume because this is the mother volume
			false,            //No Boolean operation
			0,                //Copy number
			checkOverlaps     //Check for overlapping volumes
			);

	return worldVolume_physic;
}
