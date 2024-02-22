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
//To get detectors
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSEnergyDeposit.hh"

G4VPhysicalVolume* detectorConstruction::Construct() {
	//Check for overlapping volumes?
	G4bool checkOverlaps = false;

	//Define a cube of air with 20m edge length as outer most volume, the
	//so-called "world volume"

	//Get a pointer to the manager containing the NIST defined materials
	auto* nistMgr = G4NistManager::Instance();
	//Get a pointer to the "Air" material; for the names of the materials
	//see https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html
	auto* matAir = nistMgr->FindOrBuildMaterial("G4_AIR");

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

	//Place a small 1cm³ cube of PMMA (C5H8O2) within the word volume
	//This time: define material from scratch
	G4Material* matPMMA = new G4Material(
			"PMMA",           //Name
			1.18 * g / cm3,   //Density
			3,                //number of constituents
			kStateSolid,      //It's a solid
			293.15 * kelvin,  //Room temperature 20°C=293.15K
			1. * bar          //Standard pressure
			);
	//Materials consist of elements
	G4Element *C = new G4Element(
			"Carbon",         //Name of element
			"C",              //Symbol of element
			6.,               //Atomic number
			12.01 * g / mole  //Atomic weight
			);
	matPMMA->AddElement(C, 5);//One PMMA unit consists of 5 carbon atoms
	G4Element *H = new G4Element("Hydrogen", "H", 1., 1.01 * g / mole);
	matPMMA->AddElement(H, 8);//and 8 hydrogen atoms
	G4Element *O = new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
	matPMMA->AddElement(O, 2);//and 2 oxygen atoms
	//Build the cube, associate PMMA to it, place it at (-10,-10,-10)cm
	G4double cubeHalfLength = 0.5*cm;
	auto* cube_solid = new G4Box("cube", cubeHalfLength, cubeHalfLength, cubeHalfLength);
	auto* cube_logic = new G4LogicalVolume(cube_solid, matAir, "cube");
	new G4PVPlacement(nullptr, G4ThreeVector(-10.*cm, -10.*cm, -10.*cm), cube_logic, "cube", worldVoluem_logic, false, 0, checkOverlaps);

	return worldVolume_physic;
}

void detectorConstruction::ConstructSDandField() {
	//Define a "sensitive detector" (SD) that can register in
	//principle several quantities
	auto* detector = new G4MultiFunctionalDetector(
			"cubeDetector"    //Name of SD
			);
	//Create a "scorer" the register deposited energy
	auto* scorer = new G4PSEnergyDeposit(
			"edep"            //Name of scorer
			);
	//Assign the scorer to the SD
	detector->RegisterPrimitive(scorer);
	//Assign the SD to the logical volume named "cube"
	SetSensitiveDetector(
			"cube",           //Name of logical volume
			detector          //Pointer to SD
			);
	//Add the SD to the SD manager
	G4SDManager::GetSDMpointer()->AddNewDetector(detector);
}
