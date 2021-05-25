#include "NeutrinoEvent.C"
#include "Selection.C"
#include "Signal.C"

void EnergyResolutionPlots(const std::string &inputFileName_full);
double GetOscWeight(const NeutrinoEvent &nu);

void EnergyResolutionPlots(const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    int nEnergyBins = 80;
    double minEnergy = 0.0, maxEnergy = 20.0;

    TH1D * nueEnergyDist = new TH1D("nueEnergyDist", "nueEnergyDist", nEnergyBins, minEnergy, maxEnergy);
    TH1D * numuEnergyDistDefault = new TH1D("numuEnergyDistDefault", "numuEnergyDistDefault", nEnergyBins, minEnergy, maxEnergy);
    TH1D * numuEnergyDistMCS = new TH1D("numuEnergyDistMCS", "numuEnergyDistMCS", nEnergyBins, minEnergy, maxEnergy);
    TH1D * numuEnergyDistRange = new TH1D("numuEnergyDistRange", "numuEnergyDistRange", nEnergyBins, minEnergy, maxEnergy);

    TH2D * nueTrueRecoEnergy = new TH2D("nueTrueRecoEnergy", "nueTrueRecoEnergy", nEnergyBins, minEnergy, maxEnergy, nEnergyBins, minEnergy, maxEnergy);
    TH2D * numuTrueRecoEnergy = new TH2D("numuTrueRecoEnergy", "numuTrueRecoEnergy", nEnergyBins, minEnergy, maxEnergy, nEnergyBins, minEnergy, maxEnergy);

    TH1D * nueEnergyResolution = new TH1D("nueEnergyResolution", "nueEnergyResolution", 100, -2.0, 2.0);
    TH1D * numuEnergyResolutionContained = new TH1D("numuEnergyResolutionContained", "numuEnergyResolutionContained", 100, -2.0, 2.0);
    TH1D * numuEnergyResolutionUncontained = new TH1D("numuEnergyResolutionUncontained", "numuEnergyResolutionUncontained", 100, -2.0, 2.0);

    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu)); //<-- WRONG WEIGHTING

        if (IsNueSelected(nu))
        {
            if (!IsNueCCSignal(nu))
                continue;

            nueEnergyDist->Fill(nu.m_nueRecoENu, weight);
            nueTrueRecoEnergy->Fill(nu.m_eNu, nu.m_nueRecoENu, weight);
            nueEnergyResolution->Fill((nu.m_nueRecoENu - nu.m_eNu) / nu.m_eNu, weight);
        }
        else if (IsNumuSelected(nu))
        {
            if (!IsNumuCCSignal(nu))
                continue;

            if (nu.m_selTrackMomentumMethod == -1) 
                numuEnergyDistDefault->Fill(nu.m_numuRecoENu, weight);
            else if (nu.m_selTrackMomentumMethod == 0)
                numuEnergyDistMCS->Fill(nu.m_numuRecoENu, weight);
            else if (nu.m_selTrackMomentumMethod == 1)
                numuEnergyDistRange->Fill(nu.m_numuRecoENu, weight);
            else
                std::cout << "UNKNOWN OPTION ISOBEL" << std::endl;

            if (nu.m_selTrackMomentumMethod == 0)
                numuTrueRecoEnergy->Fill(nu.m_eNu, nu.m_numuRecoENu, weight);

            if (nu.m_selTrackContained == 0)
                numuEnergyResolutionUncontained->Fill((nu.m_numuRecoENu - nu.m_eNu) / nu.m_eNu, weight);

            if (nu.m_selTrackContained == 1)
                numuEnergyResolutionContained->Fill((nu.m_numuRecoENu - nu.m_eNu) / nu.m_eNu, weight);
        }
    }

    gStyle->SetOptFit(1111);

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

    TCanvas * c3 = new TCanvas("c3", "c3");
    nueEnergyResolution->SetTitle("nue energy resolution; (reco - true)/reco; nEvents");
    nueEnergyResolution->Fit("gaus");
    nueEnergyResolution->Draw("hist");
    nueEnergyResolution->GetFunction("gaus")->Draw("same");
    nueEnergyResolution->Print("all");

    TCanvas * c4 = new TCanvas("c4", "c4");
    numuEnergyResolutionContained->SetTitle("Contained numu energy resolution; (reco - true)/reco; nEvents");
    numuEnergyResolutionContained->Fit("gaus");
    numuEnergyResolutionContained->Draw("hist");
    numuEnergyResolutionContained->GetFunction("gaus")->Draw("same");
    numuEnergyResolutionContained->Print("all");

    TCanvas * c5 = new TCanvas("c5", "c5");
    numuEnergyResolutionUncontained->SetTitle("Exiting numu energy resolution; (reco - true)/reco; nEvents");
    numuEnergyResolutionUncontained->Fit("gaus");
    numuEnergyResolutionUncontained->Draw("hist");
    numuEnergyResolutionUncontained->GetFunction("gaus")->Draw("same");
    numuEnergyResolutionUncontained->Print("all");

    TCanvas * c6 = new TCanvas("c6", "c6");
    nueTrueRecoEnergy->SetTitle("Selected signal nue CC true vs reco energy;True #nu_{e} energy [GeV];Reco #nu_{e} energy [GeV]");
    nueTrueRecoEnergy->Draw("colz");

    TCanvas * c7 = new TCanvas("c7", "c7");
    numuTrueRecoEnergy->SetTitle("Selected signal numu CC true vs reco energy;True #nu_{#mu} energy [GeV];Reco #nu_{#mu} energy [GeV]");
    numuTrueRecoEnergy->Draw("colz");
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
