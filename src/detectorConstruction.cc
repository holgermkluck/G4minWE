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
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
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
	auto* matAir = nistMgr->FindOrBuildMaterial("G4_AIR");

	//Geant4 use half the edge size to define a cube
	G4double worldHalfLength = 30.*cm;
	//The defination f a volume is split in three steps:
	//1) The "solid volume" give the geometrical shape of a volume
	auto* worldVolume_solid = new G4Box(
			"world",          //Name of the solid volume
			worldHalfLength,  //Half-edge length along x-direction
			worldHalfLength,  //Half-edge length along y-direction
			worldHalfLength   //Half-edge length along z-direction
			);
	//2) The "logical volume" fills the solid volume with a material
	auto* worldVolume_logic = new G4LogicalVolume(
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
			worldVolume_logic,//The logical volume that is placed
			nullptr,          //No mother volume because this is the mother volume
			false,            //No Boolean operation
			0,                //Copy number
			checkOverlaps     //Check for overlapping volumes
			);


	//-Implement the Geiger-Marsden apparatus from their 1913 paper-------------
	// [Philos. Mag., Ser. 6, 25 (148) 604–623, DOI:10.1080/14786440408634197]
	// For the sake of simplicity ignore the paper lining of the vessel and the
	// Al diaphragm (for which also no dimensions are given).

	//Give all dimensions of the apparatus relative to the source-foil distance; it is
	//the only absolute dimension given in the paper on p.608:
	//"In some experiments the distance between the source and scattering foil was 2.5cm"
	G4double scale = 1.2*cm;

	//The vessel
	G4double wallThickness = 0.3*scale;
	G4double innerRadius = 2.*scale;
	G4double outerRadius = innerRadius+wallThickness;
	G4double innerHeight = 3.5*scale;
	G4double outerHeight = innerHeight+wallThickness;
	auto* vessel_solid = new G4Tubs("vessel", 0., outerRadius, outerHeight/2., 0.*degree, 360.*degree);
	G4Material* matWall = nistMgr->FindOrBuildMaterial("G4_STAINLESS-STEEL");
	auto* vessel_logic = new G4LogicalVolume(vessel_solid, matWall, "vessel");
	new G4PVPlacement(nullptr, G4ThreeVector(), vessel_logic, "vessel", worldVolume_logic, false, 0, checkOverlaps);

	//The vacuum inside the vessel
	auto* vac_solid = new G4Tubs("vacuum", 0., innerRadius, innerHeight/2., 0.*degree, 360.*degree);
	G4Material* matVac = nistMgr->FindOrBuildMaterial("G4_Galactic");
	auto* vac_logic = new G4LogicalVolume(vac_solid, matVac, "vacuum");
	new G4PVPlacement(nullptr, G4ThreeVector(0., 0., wallThickness/2.), vac_logic, "vacuum", vessel_logic, false, 0, checkOverlaps);

	//The glass lid on top
	G4double lidThickness = 0.4*scale;
	auto* lid_solid = new G4Tubs("lid", 0., outerRadius, lidThickness/2., 0.*degree, 360.*degree);
	G4Material* matLid = nistMgr->FindOrBuildMaterial("G4_GLASS_PLATE");
	auto* lid_logic = new G4LogicalVolume(lid_solid, matLid, "lid");
	new G4PVPlacement(nullptr, G4ThreeVector(0., 0., (outerHeight+lidThickness)/2.), lid_logic, "lid", worldVolume_logic, false, 0, checkOverlaps);

	//The gold foil
	G4double foilLength = 1.5*scale;
	//In their 1913 paper, they used two Au foils, see p.609: "equivalent to
	//0.1cm air" and 0.3cm air.Chose the first value: 0.1cm air.
	//To calculate the absolute thickness, assume that the column densities of
	//gold foil has to match the column density of an 0.1cm long air column:
	//t_Au*rho_Au = 0.1cm*rho_Air -> t_Au = 0.1cm*(rho_Air/rho_Au)
	//Acc. to wikipedia [https://en.wikipedia.org/wiki/Density_of_air]
	//                   rho_Air=1.204 kg/m3 for standard pressure and temperaure
	//Acc. to wikipedia [https://en.wikipedia.org/wiki/Gold]
	//                   rho_Au=19.3g/cm³
	//Absolute thickness of the foil: 0.1cm*((1.204e-3 g/cm³)/(19.3 g/cm³))~62nm
	G4double foilThickness = 62*nm;
	G4double distBottomFoil = 0.8*scale;
	auto* foil_solid = new G4Box("foil", foilThickness/2., foilLength/2., foilLength/2.);
	G4Material* matFoil = nistMgr->FindOrBuildMaterial("G4_Au");
	auto* foil_logic = new G4LogicalVolume(foil_solid, matFoil, "foil");
	new G4PVPlacement(nullptr, G4ThreeVector(0., 0., -innerHeight/2+foilLength/2+distBottomFoil), foil_logic, "foil", vac_logic, false, 0, checkOverlaps);

	//The Pb block minus the hole
	G4double blockHeight = 1.8*scale;
	G4double blockThickness = (1.3-0.5)*scale;
	G4double distBottomBlock = 0.2*scale;
	auto* block_ = new G4Box("block_", blockThickness/2., blockThickness/2., blockHeight/2.);
	G4double notchDepth = 0.1*scale;
	G4double notchHeight = 0.2*scale;
	G4double distBottomNotch = 1.4*scale;
	G4double distBlockCapsule_x = (1-0.5)*scale;
	G4double distBlock = 0.5*scale;//Distance between block and center of vessel along x-axis
	G4double margin = 1.*mm;
	auto* hole = new G4Tubs("hole", 0, notchDepth/2., distBlockCapsule_x/2., 0*degree, 360*degree);
	G4RotationMatrix* mat = new G4RotationMatrix();
	mat->rotateY(270*degree);
	auto* block_solid = new G4SubtractionSolid("block", block_, hole, mat, G4ThreeVector(-blockThickness/2.+distBlockCapsule_x/2., 0, -blockHeight/2.+notchHeight/2.+(distBottomNotch-distBottomBlock)));
	G4Material* matLead = nistMgr->FindOrBuildMaterial("G4_Pb");
	auto* block_logic = new G4LogicalVolume(block_solid, matLead, "block");
	new G4PVPlacement(nullptr, G4ThreeVector(blockThickness/2.+distBlock, 0., -innerHeight/2+blockHeight/2+distBottomBlock), block_logic, "block", vac_logic, false, 0, checkOverlaps);

	//The ZnS screen
	G4double screenHeight = 0.7*scale;
	G4double screenRadius = 0.8*scale;
	//Not given in paper, assume ~100um like here [https://mediatum.ub.tum.de/doc/1614751/document.pdf]
	G4double screenThickness = 100.*um;
	G4double screenStartAngle = 30.*degree;
	G4double screenAngleSegment = 300.*degree;//From -150° to 150°
	auto* screen_solid = new G4Tubs("screen", screenRadius-screenThickness, screenRadius, screenHeight/2., screenStartAngle, screenAngleSegment);
	//ZnS is not predefined by Geant4, so have to defined it from scratch
	//https://en.wikipedia.org/wiki/Zinc_sulfide
	G4Material* matZnS = new G4Material("ZnS", 4.090 * g / cm3, 2, kStateSolid, 293.15 * kelvin, 1. * bar);
	//https://en.wikipedia.org/wiki/Zinc
	G4Element *elZn = new G4Element("Zinc", "Zn", 30., 65.38 * g / mole);
	matZnS->AddElement(elZn, 1);
	//https://en.wikipedia.org/wiki/Sulfur
	G4Element *elS = new G4Element("Sulfur", "S", 16., 32.06 * g / mole);
	matZnS->AddElement(elS, 1);
	auto* screen_logic = new G4LogicalVolume(screen_solid, matZnS, "screen");
	new G4PVPlacement(nullptr, G4ThreeVector(), screen_logic, "screen", vac_logic, false, 0, checkOverlaps);

	return worldVolume_physic;
}
