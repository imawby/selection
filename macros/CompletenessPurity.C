#include "NeutrinoEvent.C"
//#include "Signal.C"
#include "Selection.C"

void CompletenessPurity(const std::string &inputFileName);
void DrawHistograms(NeutrinoEventVector &nuEventVector_full);
//double GetOscWeight(const NeutrinoEvent &nu);

void CompletenessPurity(const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    DrawHistograms(nuEventVector_full);    
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawHistograms(NeutrinoEventVector &nuEventVector_full)
{
    int pandrizzleBins = 25, completenessBins = 25, purityBins = 25;
    double pandrizzleMin = -1.0, completenessMin = 0.0, purityMin = 0.0;
    double pandrizzleMax = 1.0, completenessMax = 1.0, purityMax = 1.0;

    TH1D * completenessSel = new TH1D("completenessSel", "completenessSel", completenessBins, completenessMin, completenessMax);
    TH1D * puritySel = new TH1D("puritySel", "puritySel", purityBins, purityMin, purityMax);

    TH2D * completenessPandrizzleSel = new TH2D("completenessPandrizzleSel", "completenessPandrizzleSel", pandrizzleBins, pandrizzleMin, pandrizzleMax, 
        completenessBins, completenessMin, completenessMax);
    TH2D * purityPandrizzleSel = new TH2D("purityPandrizzleSel", "purityPandrizzleSel", pandrizzleBins, pandrizzleMin, pandrizzleMax, purityBins, purityMin, purityMax);

    TH2D * countPandrizzleSel = new TH2D("countPandrizzleSel", "countPandrizzleSel", completenessBins, completenessMin, completenessMax, purityBins, purityMin, purityMax);
    TH2D * avPandrizzleSel = new TH2D("avPandrizzleSel", "avPandrizzleSel", completenessBins, completenessMin, completenessMax, purityBins, purityMin, purityMax);

    TH1D * completenessAll = new TH1D("completenessAll", "completenessAll", completenessBins, completenessMin, completenessMax);
    TH1D * purityAll = new TH1D("purityAll", "purityAll", purityBins, purityMin, purityMax);

    TH2D * completenessPandrizzleAll = new TH2D("completenessPandrizzleAll", "completenessPandrizzleAll", pandrizzleBins, pandrizzleMin, pandrizzleMax, 
        completenessBins, completenessMin, completenessMax);
    TH2D * purityPandrizzleAll = new TH2D("purityPandrizzleAll", "purityPandrizzleAll", pandrizzleBins, pandrizzleMin, pandrizzleMax, purityBins, purityMin, purityMax);

    TH2D * countPandrizzleAll = new TH2D("countPandrizzleAll", "countPandrizzleAll", completenessBins, completenessMin, completenessMax, purityBins, purityMin, purityMax);
    TH2D * avPandrizzleAll = new TH2D("avPandrizzleAll", "avPandrizzleAll", completenessBins, completenessMin, completenessMax, purityBins, purityMin, purityMax);

    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        // Fill selected shower histograms
        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            completenessSel->Fill(nu.m_selShowerCompleteness, 1.0);
            puritySel->Fill(nu.m_selShowerPurity, 1.0);

            avPandrizzleSel->Fill(nu.m_selShowerCompleteness, nu.m_selShowerPurity, nu.m_selShowerPandrizzleScore);
            countPandrizzleSel->Fill(nu.m_selShowerCompleteness, nu.m_selShowerPurity, 1.0);

            completenessPandrizzleSel->Fill(nu.m_selShowerPandrizzleScore, nu.m_selShowerCompleteness, 1.0);
            purityPandrizzleSel->Fill(nu.m_selShowerPandrizzleScore, nu.m_selShowerPurity, 1.0);
        }

        for (unsigned int i = 0; i < nu.m_nRecoShowers; ++i)
        {
            if (nu.m_allShowerIsPrimary[i] && (std::abs(nu.m_allShowerTruePdg[i]) == 11))
            {
                completenessAll->Fill(nu.m_allShowerCompleteness[i], 1.0);
                purityAll->Fill(nu.m_allShowerPurity[i], 1.0);

                avPandrizzleAll->Fill(nu.m_allShowerCompleteness[i], nu.m_allShowerPurity[i], nu.m_allShowerPandrizzleScore[i]);
                countPandrizzleAll->Fill(nu.m_allShowerCompleteness[i], nu.m_allShowerPurity[i], 1.0);

                completenessPandrizzleAll->Fill(nu.m_allShowerPandrizzleScore[i], nu.m_allShowerCompleteness[i], 1.0);
                purityPandrizzleAll->Fill(nu.m_allShowerPandrizzleScore[i], nu.m_allShowerPurity[i], 1.0);
            }
        }
    }

    // Obtain average pandrizzle scores
    avPandrizzleSel->Divide(countPandrizzleSel);
    avPandrizzleAll->Divide(countPandrizzleAll);

    gStyle->SetOptStat(0);

    TCanvas * cAvPandrizzleSel = new TCanvas("AvPandrizzleSel", "AvPandrizzleSel");
    cAvPandrizzleSel->Divide(2,1);
    cAvPandrizzleSel->cd(1);
    avPandrizzleSel->SetTitle("av. Pandrizzle score - Selected True Electrons;Completeness;Purity");
    avPandrizzleSel->Draw("colz");
    cAvPandrizzleSel->cd(2);
    countPandrizzleSel->SetTitle("Entries;Completeness;Purity");
    countPandrizzleSel->Draw("colz");

    TCanvas * cCompletenessSel = new TCanvas("CompletenessSel", "CompletenessSel");
    completenessSel->SetTitle("Completeness - Selected True Electrons;Completeness;nElectrons");
    completenessSel->Draw("hist");

    TCanvas * cPuritySel = new TCanvas("PuritySel", "PuritySel");
    puritySel->SetTitle("Purity - Selected True Electrons;Purity;nElectrons");
    puritySel->Draw("hist");

    TCanvas * cCompletenessPandrizzleSel = new TCanvas("CompletenessPandrizzleSel", "CompletenessPandrizzleSel");
    completenessPandrizzleSel->SetTitle(";Pandrizzle Score;Completeness");
    completenessPandrizzleSel->Draw("colz");

    TCanvas * cPurityPandrizzleSel = new TCanvas("PurityPandrizzleSel", "PurityPandrizzleSel");
    purityPandrizzleSel->SetTitle(";Pandrizzle Score;Purity");
    purityPandrizzleSel->Draw("colz");



    TCanvas * cAvPandrizzleAll = new TCanvas("AvPandrizzleAll", "AvPandrizzleAll");
    cAvPandrizzleAll->Divide(2,1);
    cAvPandrizzleAll->cd(1);
    avPandrizzleAll->SetTitle("av. Pandrizzle score - All True Electrons;Completeness;Purity");
    avPandrizzleAll->Draw("colz");
    cAvPandrizzleAll->cd(2);
    countPandrizzleAll->SetTitle("Entries;Completeness;Purity");
    countPandrizzleAll->Draw("colz");

    TCanvas * cCompletenessAll = new TCanvas("CompletenessAll", "CompletenessAll");
    completenessAll->SetTitle("Completeness - All True Electrons;Completeness;nElectrons");
    completenessAll->Draw("hist");

    TCanvas * cPurityAll = new TCanvas("PurityAll", "PurityAll");
    purityAll->SetTitle("Purity - All True Electrons;Purity;nElectrons");
    purityAll->Draw("hist");

    TCanvas * cCompletenessPandrizzleAll = new TCanvas("CompletenessPandrizzleAll", "CompletenessPandrizzleAll");
    completenessPandrizzleAll->SetTitle(";Pandrizzle Score;Completeness");
    completenessPandrizzleAll->Draw("colz");

    TCanvas * cPurityPandrizzleAll = new TCanvas("PurityPandrizzleAll", "PurityPandrizzleAll");
    purityPandrizzleAll->SetTitle(";Pandrizzle Score;Purity");
    purityPandrizzleAll->Draw("colz");


    /*
    pandrizzle_signal->Write("pandrizzle_signal");
    pandrizzle_background->Write("pandrizzle_background");
    pandizzle_signal->Write("pandizzle_signal");
    pandizzle_background->Write("pandizzle_background");

    file->Close();
    */
}

//------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------
/*
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
*/
