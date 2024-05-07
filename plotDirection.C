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
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TVector3.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TCanvas.h"
#include <string>
#include <iostream>

//General function of the form a/sin(x/b)^c
Double_t rutherford(Double_t *x, Double_t *par) {
	//Go from degree to radians
	Double_t theta = x[0]*TMath::Pi()/180.;
	return par[0] / pow(sin(theta/par[1]), par[2]);
}

void plotDirection(TFile *file) {
	//Setup the tree reader
	TTreeReader reader("screen", file);
	TTreeReaderValue<Double_t> posX(reader, "PosX");
	TTreeReaderValue<Double_t> posY(reader, "PosY");
	TTreeReaderValue<Double_t> posZ(reader, "PosZ");
	TTreeReaderValue<Int_t> nHits(reader, "Nhits");

	//Histograms for the hit position
	TH2D *hPosXY = new TH2D("hPosXY", "Hit position x-y-plane", 500, -25, 25,
			500, -25, 25);
	hPosXY->GetYaxis()->SetTitle("y / mm");
	hPosXY->GetXaxis()->SetTitle("x / mm");
	TH2D *hPosXZ = new TH2D("hPosXZ", "Hit position x-z-plane", 500, -25, 25,
			500, -25, 25);
	hPosXZ->GetYaxis()->SetTitle("z / mm");
	hPosXZ->GetXaxis()->SetTitle("x / mm");
	TH2D *hPosYZ = new TH2D("hPosYZ", "Hit position y-z-plane", 500, -25, 25,
			500, -25, 25);
	hPosYZ->GetYaxis()->SetTitle("z / mm");
	hPosYZ->GetXaxis()->SetTitle("y / mm");
	//Foil offset in mm
	TVector3 sourcePos(0., 0., -1.25);

	//Histograms for the direction of the transmitted alpha
	TH2D *hDir = new TH2D("hDir", "Direction of hit", 360, 0., 360., 180, 0.,
			180.);
	hDir->GetYaxis()->SetTitle("#theta / #circ");
	hDir->GetXaxis()->SetTitle("#phi / #circ");

	TH1D *hMag = new TH1D("hMag", "Magnitude of hit position", 250, 0., 25.);
	hMag->GetYaxis()->SetTitle("Nb of events");
	hMag->GetXaxis()->SetTitle("r / mm");
	TH1D *hPhi = new TH1D("hPhi", "Azimuth of hit position", 3600, 0., 360.);
	hPhi->GetYaxis()->SetTitle("Nb of events");
	hPhi->GetXaxis()->SetTitle("#phi / #circ");
	TH1D *hTheta = new TH1D("hTheta", "Zenith of hit position", 1800, 0., 180.);
	hTheta->GetYaxis()->SetTitle("Nb of events");
	hTheta->GetXaxis()->SetTitle("#theta / #circ");
	TH1D *hAngle = new TH1D("hAngle", "Absolute value of azimuth angle", 1800,
			0., 180.);
	hAngle->GetYaxis()->SetTitle("Nb of events");
	hAngle->GetXaxis()->SetTitle("|#phi| / #circ");
	TH1D *hPhiAbs = new TH1D("hPhiAbs",
			"PhiAbs between hit position and intial direction", 1800, 0., 180.);
	hPhiAbs->GetYaxis()->SetTitle("Nb of events");
	hPhiAbs->GetXaxis()->SetTitle("#alpha / #circ");
	TVector3 initDir(-1., 0., 0.);

	constexpr Double_t rad2deg = 180. / TMath::Pi();

	while (reader.Next()) {
		//Check that none of the coordinates are NaN
		if (!TMath::IsNaN(*posX) && !TMath::IsNaN(*posY)
				&& !TMath::IsNaN(*posZ)) {
			//Check that there is at least one hit
			if (*nHits > 0 && *nHits<2) {
				hPosXY->Fill(*posX, *posY);
				hPosXZ->Fill(*posX, *posZ);
				hPosYZ->Fill(*posY, *posZ);
				//Construct 3dim vector from components
				TVector3 pos(*posX, *posY, *posZ);
				//Correct by foil offset
				pos -= sourcePos;

				Double_t phi = pos.Phi();
				phi *= rad2deg;
				//TVector3::Phi() use the atan2(y,x) which returns [-180,180)
				//-> to get phi in [0,360), add 360° for atan2>180
				phi = phi < 0. ? 360. + phi : phi;
				hPhi->Fill(phi);
				hPhiAbs->Fill(phi - 180. < 0 ? 180. - phi : phi - 180.);

				Double_t theta = pos.Theta();
				theta *= rad2deg;
				hTheta->Fill(theta);
				hDir->Fill(phi, theta);
				Double_t mag = pos.Mag();
				hMag->Fill(mag);

				Double_t angle = pos.Angle(initDir);
				angle *= rad2deg;
				hAngle->Fill(angle);
				//std::cout << "(" << *posX << "|" << *posY << "|" << *posZ << ") mag=" << mag << " -> azimuth =" << phi << std::endl;
			}
		}
	}
	//Normalize bin count to width of first bin
	hPhi->Scale(1. / hPhi->GetBinWidth(1));
	hTheta->Scale(1. / hTheta->GetBinWidth(1));
	hAngle->Scale(1. / hAngle->GetBinWidth(1));
	hMag->Scale(1. / hMag->GetBinWidth(1));
	hPhiAbs->Scale(1. / hPhiAbs->GetBinWidth(1));
	//hDir->Scale(1./hDir->GetBinWidth(1));
	//hPosXY->Scale(1./hPosXY->GetBinWidth(1));
	//hPosXZ->Scale(1./hPosXZ->GetBinWidth(1));
	//hPosYZ->Scale(1./hPosYZ->GetBinWidth(1));

	TCanvas *c1 = new TCanvas("c1", "Hit positions", 1024, 1024);
	c1->Divide(2, 2);
	c1->Draw();
	c1->cd(1);
	gPad->SetLogz(1);
	hPosXZ->Draw("colz");
	c1->cd(2);
	gPad->SetLogz(1);
	hPosYZ->Draw("colz");
	c1->cd(3);
	gPad->SetLogz(1);
	hPosXY->Draw("colz");

	TCanvas *c2 = new TCanvas("c2", "Hit direction", 1024, 768);
	c2->Divide(2, 2);
	c2->Draw();
	c2->cd(1);
	hPhi->Draw("histo");
	gPad->SetLogy(1);
	c2->cd(2);
	hMag->Draw("histo");
	gPad->SetLogy(1);
	c2->cd(3);
	hDir->Draw("colz");
	gPad->SetLogz(1);
	c2->cd(4);
	hTheta->Draw("hbar histo");
	gPad->SetLogx(1);

	//Normalise to unity to get PDF
	hPhiAbs->Scale(1. / hPhiAbs->Integral("width"));
	std::cout << hPhiAbs->Integral("width") << std::endl;
	hPhiAbs->GetYaxis()->SetTitle("PDF");

	hAngle->Scale(1. / hAngle->Integral("width"));
	std::cout << hAngle->Integral("width") << std::endl;
	hAngle->GetYaxis()->SetTitle("PDF");

	TH1D* hPhiAbsOmega = (TH1D*)hPhiAbs->Clone("hPhiAbsOmega");
	hPhiAbsOmega->SetLineColor(kGreen);
	for(int i = 1; i <= hPhiAbsOmega->GetNbinsX(); ++i){
		Double_t angle = hPhiAbsOmega->GetBinCenter(i);
		angle /= rad2deg;
		hPhiAbsOmega->SetBinContent(i, hPhiAbsOmega->GetBinContent(i)/sin(angle));
	}

	TH1D* hAngleOmega = (TH1D*)hAngle->Clone("hAngleOmega");
	hAngleOmega->SetLineColor(kMagenta);
	for(int i = 1; i <= hAngleOmega->GetNbinsX(); ++i){
		Double_t angle = hAngleOmega->GetBinCenter(i);
		angle /= rad2deg;
		hAngleOmega->SetBinContent(i, hAngleOmega->GetBinContent(i)/sin(angle));
	}

	TCanvas *c3 = new TCanvas("c3", "", 1024, 768);
	c3->Draw();
	c3->Divide(1,4);
	c3->cd(1);
	hPhiAbs->Draw("histo");
	hAngle->SetLineColor(kRed);
	hAngle->Draw("histo same");
	hPhiAbsOmega->Draw("histo same");
	hAngleOmega->Draw("histo same");
	gPad->SetLogy(1);

	c3->cd(2);
	hPhiAbs->Draw("histo");
	gPad->SetLogy(1);

	std::cout << "Fit absolute phi" << std::endl;
	std::cout << "Let all parameter free" << std::endl;
	TF1 *fitFunc = new TF1("fitFunc", rutherford, 3, 23, 3);
	fitFunc->SetNpx(1000);
	fitFunc->SetLineColor(kRed);
	fitFunc->SetParameter(0, 1e-8);
	fitFunc->SetParameter(1, 2.);
	fitFunc->SetParameter(2, 4);
	TFitResultPtr res = hPhiAbs->Fit("fitFunc", "LSRM");
	fitFunc->Draw("same");
	std::cout << "chi2/ndf = " << res->Chi2() << "/" << res->Ndf() << std::endl;

	std::cout << "Fix denominator" << std::endl;
	TF1 *fitFunc2 = new TF1("fitFunc2", rutherford, 3, 23, 3);
	fitFunc2->SetNpx(1000);
	fitFunc2->SetLineColor(kRed);
	fitFunc2->SetLineStyle(10);
	fitFunc2->SetParameter(0, fitFunc->GetParameter(0));
	fitFunc2->FixParameter(1, 2.);
	fitFunc2->SetParameter(2, 4);
	TFitResultPtr res2 = hPhiAbs->Fit("fitFunc2", "LSRM");
	fitFunc2->Draw("same");
	std::cout << "chi2/ndf = " << res2->Chi2() << "/" << res2->Ndf() << std::endl;

	c3->cd(3);
	hPhiAbsOmega->Draw("histo");
	gPad->SetLogy(1);

	std::cout << "Fit phi/dOmega" << std::endl;
	std::cout << "Let all parameter free" << std::endl;
	TF1 *fitFunc3 = new TF1("fitFunc3", rutherford, 3, 23, 3);
	fitFunc3->SetNpx(1000);
	fitFunc3->SetLineColor(kRed);
	fitFunc3->SetParameter(0, 1e-8);
	fitFunc3->SetParameter(1, 2.);
	fitFunc3->SetParameter(2, 4);
	TFitResultPtr res3 = hPhiAbsOmega->Fit("fitFunc3", "LSRM");
	fitFunc3->Draw("same");
	std::cout << "chi2/ndf = " << res3->Chi2() << "/" << res3->Ndf() << std::endl;

	std::cout << "Fix denominator" << std::endl;
	TF1 *fitFunc4 = new TF1("fitFunc4", rutherford, 3, 23, 3);
	fitFunc4->SetNpx(1000);
	fitFunc4->SetLineColor(kRed);
	fitFunc4->SetLineStyle(10);
	fitFunc4->SetParameter(0, fitFunc3->GetParameter(0));
	fitFunc4->FixParameter(1, 2.);
	fitFunc4->SetParameter(2, 4);
	TFitResultPtr res4 = hPhiAbsOmega->Fit("fitFunc4", "LSRM");
	fitFunc4->Draw("same");
	std::cout << "chi2/ndf = " << res4->Chi2() << "/" << res4->Ndf() << std::endl;

	std::cout << "Fix exponent" << std::endl;
	TF1 *fitFunc5 = new TF1("fitFunc5", rutherford, 3, 23, 3);
	fitFunc5->SetNpx(1000);
	fitFunc5->SetLineColor(kRed);
	fitFunc5->SetLineStyle(10);
	fitFunc5->SetParameter(0, fitFunc4->GetParameter(0));
	fitFunc5->SetParameter(1, 2.);
	fitFunc5->FixParameter(2, 4);
	TFitResultPtr res5 = hPhiAbsOmega->Fit("fitFunc5", "LSRM");
	fitFunc5->Draw("same");
	std::cout << "chi2/ndf = " << res5->Chi2() << "/" << res5->Ndf() << std::endl;

	TF1 *func = new TF1("func", rutherford, 3, 180, 3);
	func->SetParameter(0, fitFunc3->Eval(3)*pow(sin(3/(2*rad2deg)),4));
	func->SetParameter(1,2);
	func->SetParameter(2,4);
	func->SetLineColor(kOrange);
	func->Draw("same");

	c3->cd(4);
	hAngleOmega->Draw("histo");
	gPad->SetLogy(1);

	std::cout << "Fit angle/dOmega" << std::endl;
	std::cout << "Let all parameter free" << std::endl;
	TF1 *fitFunc6 = new TF1("fitFunc6", rutherford, 3, 23, 3);
	fitFunc6->SetNpx(1000);
	fitFunc6->SetLineColor(kRed);
	fitFunc6->SetParameter(0, 1e-8);
	fitFunc6->SetParameter(1, 2.);
	fitFunc6->SetParameter(2, 4);
	TFitResultPtr res6 = hAngleOmega->Fit("fitFunc6", "LSRM");
	fitFunc6->Draw("same");
	std::cout << "chi2/ndf = " << res6->Chi2() << "/" << res6->Ndf() << std::endl;

	std::cout << "Fix denominator" << std::endl;
	TF1 *fitFunc7 = new TF1("fitFunc7", rutherford, 3, 23, 3);
	fitFunc7->SetNpx(1000);
	fitFunc7->SetLineColor(kRed);
	fitFunc7->SetLineStyle(10);
	fitFunc7->SetParameter(0, fitFunc3->GetParameter(0));
	fitFunc7->FixParameter(1, 2.);
	fitFunc7->SetParameter(2, 4);
	TFitResultPtr res7 = hAngleOmega->Fit("fitFunc7", "LSRM");
	fitFunc7->Draw("same");
	std::cout << "chi2/ndf = " << res7->Chi2() << "/" << res7->Ndf() << std::endl;

	std::cout << "Fix exponent" << std::endl;
	TF1 *fitFunc8 = new TF1("fitFunc8", rutherford, 3, 23, 3);
	fitFunc8->SetNpx(1000);
	fitFunc8->SetLineColor(kRed);
	fitFunc8->SetLineStyle(10);
	fitFunc8->SetParameter(0, fitFunc4->GetParameter(0));
	fitFunc8->SetParameter(1, 2.);
	fitFunc8->FixParameter(2, 4);
	TFitResultPtr res8 = hAngleOmega->Fit("fitFunc8", "LSRM");
	fitFunc8->Draw("same");
	std::cout << "chi2/ndf = " << res8->Chi2() << "/" << res8->Ndf() << std::endl;

	TF1 *func2 = new TF1("func2", rutherford, 3, 180, 3);
	func2->SetParameter(0, fitFunc7->Eval(3)*pow(sin(3/(2*rad2deg)),4));
	func2->SetParameter(1,2);
	func2->SetParameter(2,4);
	func2->SetLineColor(kOrange);
	func2->Draw("same");
}

