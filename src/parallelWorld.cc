/*
 * Copyright 2024 Holger Kluck
 * This file parallelWorld.cc is part of G4minWE.
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

#include "parallelWorld.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

G4minWE::ParallelWorld::ParallelWorld(G4String name) : G4VUserParallelWorld(name){    
}

void G4minWE::ParallelWorld::Construct(){
    //Get a pointer to the physical world volume
    auto* world_physic = GetWorld();

    //Dimensions of one pixel: 50um x 50um, thickness 500um
    G4double pix_x = 50.*um;
    G4double pix_y = pix_x;
    G4double pix_z = 500.*um;

    //Create an eveloping volume large enough to contain a matric of 160 x 160 pixel on the
    //top side of the 1cm³ cube at (-10, -10, -10)cm
    G4ThreeVector pos(-10.*cm, -10.*cm,
        -10.*cm      //center of the 1cm³
        + (1.*cm/2.) //upper surface of the 1cm³
        - (pix_z/2.) //pixels surface should align with cube surface
        );
    //Repeat pixel 160 times along y-axis
    G4int rep_x = 160;
    G4int rep_y = rep_x;
    //Create an eveloping volume
    auto* matrix_solid = new G4Box("matrix", (rep_x*pix_x)/2., (rep_y*pix_y)/2., pix_z/2.);
    auto* matrix_logic = new G4LogicalVolume(
        matrix_solid,
        nullptr, //We are in a "parallel world", volumes need no material
        "matrix");
    new G4PVPlacement(nullptr, pos, "matrix", matrix_logic, world_physic, false, 0);

    //Fill the matrix with "rows" along the y-axis    
    auto* row_solid = new G4Box("row", (rep_x*pix_x)/2., pix_y/2., pix_z/2.);
    auto* row_logic = new G4LogicalVolume(row_solid, nullptr, "row");
    new G4PVReplica("row_rep", row_logic, matrix_logic, kYAxis, rep_y, pix_y);

    //Fill the rows with pixels along the x-axis
    auto* pixel_solid = new G4Box("pixel", pix_x/2., pix_y/2., pix_z/2);
    auto* pixel_logic = new G4LogicalVolume(pixel_solid, nullptr, "pixel");
    new G4PVReplica("pix_rep", pixel_logic, row_logic, kXAxis, rep_x, pix_x); 
}

void G4minWE::ParallelWorld::ConstructSD(){

}