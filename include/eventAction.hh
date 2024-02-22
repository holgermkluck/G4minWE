/*
 * Copyright 2024 Holger Kluck
 * This file eventAction.hh is part of G4minWE.
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

#ifndef INCLUDE_EVENTACTION_HH_
#define INCLUDE_EVENTACTION_HH_

#include "G4UserEventAction.hh"

class eventAction : public G4UserEventAction{

public:
	eventAction() = default;
	~eventAction() override = default;

	void BeginOfEventAction() override;
	void EndOfEventaction() override;
};

#endif /* INCLUDE_EVENTACTION_HH_ */
