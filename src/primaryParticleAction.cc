/*
 * Copyright 2024 Holger Kluck
 * This file primaryParticleAction.cc is part of G4minWE.
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

#include "primaryParticleAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

primaryParticleAction::primaryParticleAction() {
	//Create a "particle gun" that shoot one particle during each event
	gun = new G4ParticleGun(1);
	//Particle should be a alpha of 10MeV shot in direction (1,0,0)
	//from position (0,0,0)m
	auto* particleDef = G4ParticleTable::GetParticleTable()->FindParticle("alpha");
	gun->SetParticleDefinition(particleDef);
	gun->SetParticleMomentumDirection(G4ThreeVector(1., 0., 0.));
	gun->SetParticleEnergy(10.*MeV);
	gun->SetParticlePosition(G4ThreeVector());
}

primaryParticleAction::~primaryParticleAction() {
	delete gun;
}

void primaryParticleAction::GeneratePrimaries(G4Event* evt) {
	//This method is called by Geant4 at the beginning of each
	//event: it will create the vertex of the primary particle
	gun->GeneratePrimaryVertex(evt);
}


