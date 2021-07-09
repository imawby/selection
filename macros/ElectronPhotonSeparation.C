#include "NeutrinoEvent.C"
#include "Selection.C"
#include "Signal.C"

void ElectronPhotonSeparation(const std::string &inputFileName_full);
void DrawSeparationHistograms(NeutrinoEventVector &nuEventVector_full);
double GetOscWeight(const NeutrinoEvent &nu);

const bool IS_NEUTRINO = true;

void ElectronPhotonSeparation(const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    DrawSeparationHistograms(nuEventVector_full);    
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawSeparationHistograms(NeutrinoEventVector &nuEventVector_full)
{
    TFile * file = new TFile("ElectronPhotonSeparation.root", "CREATE");

    int pandrizzleBins = 25;
    double pandrizzleMin = -1.0, pandrizzleMax = 1.0;
    TH1D * pandrizzle_electron = new TH1D("pandrizzle_electron", "pandrizzle_electron", pandrizzleBins, pandrizzleMin, pandrizzleMax);
    pandrizzle_electron->SetDirectory(0);
    TH1D * pandrizzle_photon = new TH1D("pandrizzle_photon", "pandrizzle_photon", pandrizzleBins, pandrizzleMin, pandrizzleMax);
    pandrizzle_photon->SetDirectory(0);

    int pandizzleBins = 25;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    TH1D * pandizzle_electron = new TH1D("pandizzle_electron", "pandizzle_electron", pandizzleBins, pandizzleMin, pandizzleMax);
    pandizzle_electron->SetDirectory(0);
    TH1D * pandizzle_photon = new TH1D("pandizzle_photon", "pandizzle_photon", pandizzleBins, pandizzleMin, pandizzleMax);
    pandizzle_photon->SetDirectory(0);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));

        if ((std::abs(nu.m_selShowerTruePdg) == 11) && IsNueCCSignal(nu, IS_NEUTRINO))
        {
            pandrizzle_electron->Fill(nu.m_selShowerPandrizzleScore, weight);
            pandizzle_electron->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == 22)
        {
            pandrizzle_photon->Fill(nu.m_selShowerPandrizzleScore, weight);
            pandizzle_photon->Fill(nu.m_selTrackPandizzleScore, weight);
        }
    }
    /*
    pandrizzle_electron->Scale(1.0/pandrizzle_electron->Integral());
    pandrizzle_photon->Scale(1.0/pandrizzle_photon->Integral());
    pandizzle_electron->Scale(1.0/pandizzle_electron->Integral());
    pandizzle_photon->Scale(1.0/pandizzle_photon->Integral());
    */
    gStyle->SetOptStat(0);

    TCanvas * pandrizzle = new TCanvas("pandrizzle", "pandrizzle");
    pandrizzle_photon->SetTitle(";Pandrizzle Score;nEvents");
    pandrizzle_electron->SetLineColor(kBlue);
    pandrizzle_photon->SetLineColor(kRed);
    pandrizzle_photon->Draw("hist same");
    pandrizzle_electron->Draw("hist same");

    auto pandrizzleLegend = new TLegend(0.1,0.7,0.48,0.9);
    pandrizzleLegend->AddEntry(pandrizzle_electron, "electron", "l");
    pandrizzleLegend->AddEntry(pandrizzle_photon, "photon", "l");
    pandrizzleLegend->Draw("same");

    TCanvas * pandizzle = new TCanvas("pandizzle", "pandizzle");
    pandizzle_photon->SetTitle(";Pandizzle Score;nEvents");
    pandizzle_electron->SetLineColor(kBlue);
    pandizzle_photon->SetLineColor(kRed);
    pandizzle_photon->Draw("hist same");
    pandizzle_electron->Draw("hist same");

    auto pandizzleLegend = new TLegend(0.1,0.7,0.48,0.9);
    pandizzleLegend->AddEntry(pandizzle_electron, "electron", "l");
    pandizzleLegend->AddEntry(pandizzle_photon, "photon", "l");
    pandizzleLegend->Draw("same");

    pandrizzle_electron->Write("pandrizzle_electron");
    pandrizzle_photon->Write("pandrizzle_photon");
    pandizzle_electron->Write("pandizzle_electron");
    pandizzle_photon->Write("pandizzle_photon");

    file->Close();
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu)
{
    double DEF_SIN2THETA12 = 0.310;
    double DEF_SIN2THETA13 = 0.02240;
    double DEF_SIN2THETA23 = 0.582;
    double DEF_DLITTLEM2 = 7.39e-5;
    double DEF_DBIGM2 = 2.525e-3;
    double DEF_DELTA = 0;

    double weight = getProb3ppWeights(DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, DEF_DELTA, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}

//------------------------------------------------------------------------------------------------------------------------------------------

