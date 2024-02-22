/*
 * Copyright 2024 Holger Kluck
 * This file actionInitialiser.cc is part of G4minWE.
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

#include "actionInitialiser.hh"
#include "primaryParticleAction.hh"
#include "eventAction.hh

void actionInitialiser::Build() const {
	//Set primary particle generator
	SetUserAction(new primaryParticleAction);
	//Set event action
	SetUserAction(new eventAction);
}
