/*
 * Copyright 2024 Holger Kluck
 * This file plotDirection.C is part of geiger-marsden-experiment.
 *
 * geiger-marsden-experiment is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * geiger-marsden-experiment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with geiger-marsden-experiment. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"
#include <string>
#include <iostream>

void plotDirection(std::string name){
	TFile *f = TFile::Open(name.c_str(), "READ");
	TTree *t = nullptr;
	f->GetObject("screen", t);
	if(!t){
		std::cout << "Tree not found!";
		return;
	}
	Double_t x, y, z;
	t->SetBranchAddress("PosX", &x);
	t->SetBranchAddress("PosY", &y);
	t->SetBranchAddress("PosZ", &z);
	TH1D *h = new TH1D("h","",360,0,360);
	for(int i = 0; i < t->GetEntries(); ++i){
		t->GetEntry(i);
		Double_t mag = sqrt((x*x)+(y*y)+(z*z));
		//Double_t azimuth = TMath::ATan2(x/mag +1, y/mag);
		Double_t azimuth = 0;
		if(x<0){
			azimuth = TMath::ATan(y/x);
		}else{
			azimuth = 90+TMath::ATan(x/y);
		}
		azimuth *= 180./TMath::Pi();
		h->Fill(azimuth);
		std::cout << "(" << x << "|" << y << "|" << z << ") mag=" << mag << " -> azimuth =" << azimuth << std::endl;
	}

}


