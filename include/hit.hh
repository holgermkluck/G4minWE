/*
 * Copyright 2024 Holger Kluck
 * This file hit.hh is part of G4minWE.
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

#ifndef INCLUDE_HIT_HH_
#define INCLUDE_HIT_HH_

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

namespace G4minWE{
class Hit: public G4VHit {

public:
	Hit() = default;
	Hit(const Hit&) = default;
	~Hit() override = default;

	Hit& operator=(const Hit&) = default;
	G4bool operator==(const Hit& right) const;

	inline void* operator new(size_t);
	inline void operator delete(void*);

	void Draw() override;
	void Print() override;

	void SetEnergDeposit(G4double edep);
	void SetPosition(const G4ThreeVector &pos);

	G4double GetEnergyDeposit() const;
	G4ThreeVector GetPosition() const;

private:
	G4double EnergyDeposit { 0. };
	G4ThreeVector Position;
};

using HitsCollection = G4THitsCollection<Hit>;

extern G4ThreadLocal G4Allocator<Hit> *TrackerHitAllocator;

inline void* Hit::operator new(size_t) {
	if (!TrackerHitAllocator) {
		TrackerHitAllocator = new G4Allocator<Hit>;
	}
	return (void*) TrackerHitAllocator->MallocSingle();
}

inline void Hit::operator delete(void *hit) {
	TrackerHitAllocator->FreeSingle((Hit*) hit);
}
}

#endif /* INCLUDE_HIT_HH_ */
