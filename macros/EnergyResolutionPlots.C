#include "NeutrinoEvent.C"
#include "Selection.C"
#include "Signal.C"

void EnergyResolutionPlots(const std::string &inputFileName_full);
double GetOscWeight(const NeutrinoEvent &nu, const double deltaCP);

const bool IS_JAM_SELECTION = false;
const double POT_CONVERSION =  1.1/1.47;
const bool IS_FHC = true;

const int N_ENERGY_BINS = 16;
const double MIN_ENERGY = 0.0, MAX_ENERGY = 8.0;
const double ENEGRY_BIN_LIMITS[17] = {0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.50, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 8.0};

void EnergyResolutionPlots(const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    TH1D * nueEnergyDist = new TH1D("nueEnergyDist", "nueEnergyDist", N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY);
    TH1D * numuEnergyDistDefault = new TH1D("numuEnergyDistDefault", "numuEnergyDistDefault", N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY);
    TH1D * numuEnergyDistMCS = new TH1D("numuEnergyDistMCS", "numuEnergyDistMCS", N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY);
    TH1D * numuEnergyDistRange = new TH1D("numuEnergyDistRange", "numuEnergyDistRange", N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY);

    TH2D * nueTrueRecoEnergy = new TH2D("nueTrueRecoEnergy", "nueTrueRecoEnergy", N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY, N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY);
    TH2D * numuTrueRecoEnergy = new TH2D("numuTrueRecoEnergy", "numuTrueRecoEnergy", N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY, N_ENERGY_BINS, MIN_ENERGY, MAX_ENERGY);

    TH1D * nueEnergyResolution = new TH1D("nueEnergyResolution", "nueEnergyResolution", 100, -2.0, 2.0);
    TH1D * numuEnergyResolution = new TH1D("numuEnergyResolution", "numuEnergyResolution", 100, -2.0, 2.0);
    TH1D * numuEnergyResolutionContained = new TH1D("numuEnergyResolutionContained", "numuEnergyResolutionContained", 100, -2.0, 2.0);
    TH1D * numuEnergyResolutionUncontained = new TH1D("numuEnergyResolutionUncontained", "numuEnergyResolutionUncontained", 100, -2.0, 2.0);

    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, 0.0)) * POT_CONVERSION);

        if (IsNueSelected(nu, IS_FHC, IS_JAM_SELECTION))
        {
            if (!IsNueCCSignal(nu, IS_FHC))
                continue;
    
            if (nu.m_nueRecoENu > 8.0)
                continue;

            nueEnergyDist->Fill(nu.m_nueRecoENu, weight);
            nueTrueRecoEnergy->Fill(nu.m_eNu, nu.m_nueRecoENu, weight);
            nueEnergyResolution->Fill((nu.m_nueRecoENu - nu.m_eNu) / nu.m_eNu, weight);
        }
        else if (IsNumuSelected(nu, IS_FHC, IS_JAM_SELECTION))
        {
            if (!IsNumuCCSignal(nu, IS_FHC))
                continue;

            if (nu.m_numuRecoENu > 8.0)
                continue;

            if (nu.m_selTrackMomentumMethod == -1) 
                numuEnergyDistDefault->Fill(nu.m_numuRecoENu, weight);
            else if (nu.m_selTrackMomentumMethod == 0)
                numuEnergyDistMCS->Fill(nu.m_numuRecoENu, weight);
            else if (nu.m_selTrackMomentumMethod == 1)
                numuEnergyDistRange->Fill(nu.m_numuRecoENu, weight);
            else
                std::cout << "UNKNOWN OPTION ISOBEL" << std::endl;

            numuEnergyResolution->Fill((nu.m_numuRecoENu - nu.m_eNu) / nu.m_eNu, weight);

            if (nu.m_selTrackMomentumMethod == 0)
                numuTrueRecoEnergy->Fill(nu.m_eNu, nu.m_numuRecoENu, weight);

            if (nu.m_selTrackContained == 0)
                numuEnergyResolutionUncontained->Fill((nu.m_numuRecoENu - nu.m_eNu) / nu.m_eNu, weight);

            if (nu.m_selTrackContained == 1)
                numuEnergyResolutionContained->Fill((nu.m_numuRecoENu - nu.m_eNu) / nu.m_eNu, weight);
        }
    }
    gStyle->SetOptStat(0000);
    gStyle->SetOptFit(111);

    /*
    TCanvas * c1  = new TCanvas("c1", "c1");
    nueEnergyDist->SetTitle("Selected signal nue CC reco energy distribution;Reco Energy [GeV]; nEvents");
    nueEnergyDist->Draw("hist");

    TCanvas * c2  = new TCanvas("c2", "c2");
    numuEnergyDistRange->SetTitle("Selected signal numu CC reco energy distribution;Reco Energy [GeV]; nEvents");
    numuEnergyDistDefault->SetLineColor(kRed);
    numuEnergyDistMCS->SetLineColor(kBlue);
    numuEnergyDistRange->SetLineColor(kBlack);

    numuEnergyDistRange->Draw("hist");
    numuEnergyDistMCS->Draw("hist same");
    numuEnergyDistDefault->Draw("hist same");

    TLegend * legend1 = new TLegend(0.1,0.7,0.48,0.9);
    legend1->AddEntry(numuEnergyDistDefault, "simple energy calc", "l");
    legend1->AddEntry(numuEnergyDistMCS, "MCS", "l");
    legend1->AddEntry(numuEnergyDistRange, "range", "l");
    legend1->Draw("same");
    */
    /*
    TCanvas * c3 = new TCanvas("cNueEnergyResolution", "cNueEnergyResolution");
    nueEnergyResolution->SetTitle(";(Reconstructed Energy - True Energy)/True Energy;Number of Events");
    nueEnergyResolution->Fit("gaus");
    nueEnergyResolution->Draw("hist");
    nueEnergyResolution->GetFunction("gaus")->Draw("same");
    nueEnergyResolution->Print("all");
    
    TCanvas * c4 = new TCanvas("cNumuEnergyResolution", "cNumuEnergyResolution");
    numuEnergyResolution->SetTitle(";(Reconstructed Energy - True Energy)/True Energy;Number of Events");
    numuEnergyResolution->Fit("gaus");
    numuEnergyResolution->Draw("hist");
    numuEnergyResolution->GetFunction("gaus")->Draw("same");
    numuEnergyResolution->Print("all");
    
    TCanvas * c5 = new TCanvas("cNumuContainedEnergyResolution", "cNumuContainedEnergyResolution");
    numuEnergyResolutionContained->SetTitle(";(Reconstructed Energy - True Energy)/True Energy;Number of Events");
    numuEnergyResolutionContained->Fit("gaus");
    numuEnergyResolutionContained->Draw("hist");
    numuEnergyResolutionContained->GetFunction("gaus")->Draw("same");
    numuEnergyResolutionContained->Print("all");
    */
    TCanvas * c6 = new TCanvas("cNumuUncontainedEnergyResolution", "cNumuUncontainedEnergyResolution");
    numuEnergyResolutionUncontained->SetTitle(";(Reconstructed Energy - True Energy)/True Energy;Number of Events");
    numuEnergyResolutionUncontained->Fit("gaus");
    numuEnergyResolutionUncontained->Draw("hist");
    numuEnergyResolutionUncontained->GetFunction("gaus")->Draw("same");
    numuEnergyResolutionUncontained->Print("all");
    
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu, const double deltaCP)
{
    /*
    // TO MATCH CAFANA
    double DEF_SIN2THETA12 = 0.27926180; //0.310;
    double DEF_SIN2THETA13 = 0.021799994; //0.02240;
    double DEF_SIN2THETA23 = 0.51199968; //0.582;
    double DEF_DLITTLEM2 = 7.53e-5; //7.39e-5;
    double DEF_DBIGM2 = 0.002444; //2.525e-3;
    */
    double DEF_SIN2THETA12 = 0.310;
    double DEF_SIN2THETA13 = 0.02240;
    double DEF_SIN2THETA23 = 0.582;
    double DEF_DLITTLEM2 = 7.39e-5;
    double DEF_DBIGM2 = 2.525e-3;

    double weight = getProb3ppWeights(DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, deltaCP, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}
