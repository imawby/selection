#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"
#include "NuSelection.h"
//#include "/dune/app/users/dbrailsf/oscillation/nu_mu/cutsel/tunings/getOscWeights/getProb3ppWeights.C"

bool PERFORM_OLD_NUE_SELECTION = false;

void NuSelection(const std::string &inputFileName)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_OLD_NUE_SELECTION ? "old " : "new ") << "\033[31m" << "nue selection" << "\033[0m" << std::endl; 

    NeutrinoEventVector neutrinoEventVector;
    ReadFile(inputFileName, neutrinoEventVector);

    NuSelectionHistogramCollection nueHistogramCollection;
    InitialiseNuSelectionHistogramCollection(nueHistogramCollection, "nue");

    NuSelectionHistogramCollection numuHistogramCollection;
    InitialiseNuSelectionHistogramCollection(numuHistogramCollection, "numu");

    PerformSelection(neutrinoEventVector, nueHistogramCollection, numuHistogramCollection);

    ProcessHistogramCollection(nueHistogramCollection);
    ProcessHistogramCollection(numuHistogramCollection);

    DrawHistogramCollection(nueHistogramCollection, "nue");
    DrawHistogramCollection(numuHistogramCollection, "numu");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void InitialiseNuSelectionHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix)
{
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_eNuHists, histPrefix + "_ENu", 50, 0.0, 10.0);
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_qSqrHists, histPrefix + "_QSqr", 50, 0.0, 10.0);
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_lepMomHists, histPrefix + "_LepMom", 50, 0.0, 10.0);
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_lepNuOpeningAngleHists, histPrefix + "_LepNuOpeningAngle", 50, 0.0, 18.0);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void InitialiseNuSelectionHistograms(NuSelectionHistograms &nuSelectionHistograms, const std::string &histPrefix, const unsigned int nBins, const float xMin, const float xMax)
{
    nuSelectionHistograms.m_signal = new TH1D((histPrefix + "_Signal").c_str(), (histPrefix + "_Signal").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_background = new TH1D((histPrefix + "_Background").c_str(), (histPrefix + "_Background").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_selected = new TH1D((histPrefix + "_Selected").c_str(), (histPrefix + "_Selected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_signalSelected = new TH1D((histPrefix + "_SignalSelected").c_str(), (histPrefix + "_SignalSelected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_backgroundRejected = new TH1D((histPrefix + "_BackgroundRejected").c_str(), (histPrefix + "_BackgroundRejected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_efficiency = new TH1D((histPrefix + "_Efficiency").c_str(), (histPrefix + "_Efficiency").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_purity = new TH1D((histPrefix + "_Purity").c_str(), (histPrefix + "_Purity").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_backgroundRejection = new TH1D((histPrefix + "_BackgroundRejection").c_str(), (histPrefix + "_BackgroundRejection").c_str(), nBins, xMin, xMax);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void PerformSelection(const NeutrinoEventVector &nuVector, NuSelectionHistogramCollection &nueSelectionHistogramCollection, 
    NuSelectionHistogramCollection &numuSelectionHistogramCollection)
{
    double nueSignal(0.0), nueBackground(0.0), nueSelected(0.0), nueSignalSelected(0.0), nueBackgroundRejected(0.0);
    double numuSignal(0.0), numuBackground(0.0), numuSelected(0.0), numuSignalSelected(0.0), numuBackgroundRejected(0.0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        const bool isNueCCSignal(IsNueCCSignal(nu));
        const bool isNumuCCSignal(isNueCCSignal ? false : IsNumuCCSignal(nu));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu)); //<-- WRONG WEIGHTING

        // Fill truth histograms
        if (isNueCCSignal)
        { 
            nueSignal += weight;
            nueSelectionHistogramCollection.m_eNuHists.m_signal->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_signal->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_signal->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signal->Fill(nu.m_lepNuOpeningAngle, weight);
        }
        else
        {
            nueBackground += weight;
            nueSelectionHistogramCollection.m_eNuHists.m_background->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_background->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_background->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_background->Fill(nu.m_lepNuOpeningAngle, weight);
        }

        if (isNumuCCSignal)
        { 
            numuSignal += weight;
            numuSelectionHistogramCollection.m_eNuHists.m_signal->Fill(nu.m_eNu, weight);
            numuSelectionHistogramCollection.m_qSqrHists.m_signal->Fill(nu.m_qSqr, weight);
            numuSelectionHistogramCollection.m_lepMomHists.m_signal->Fill(nu.m_lepMom, weight);
            numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signal->Fill(nu.m_lepNuOpeningAngle, weight);
        }
        else
        {
            numuBackground += weight;
            numuSelectionHistogramCollection.m_eNuHists.m_background->Fill(nu.m_eNu, weight);
            numuSelectionHistogramCollection.m_qSqrHists.m_background->Fill(nu.m_qSqr, weight);
            numuSelectionHistogramCollection.m_lepMomHists.m_background->Fill(nu.m_lepMom, weight);
            numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_background->Fill(nu.m_lepNuOpeningAngle, weight);
        }

        // Apply Selection
        if ((!PERFORM_OLD_NUE_SELECTION && PassNueSelection(nu)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu)))
        {
            nueSelected += weight;
            nueSelectionHistogramCollection.m_eNuHists.m_selected->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_selected->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_selected->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_selected->Fill(nu.m_lepNuOpeningAngle, weight);

            if (isNueCCSignal)
            {
                nueSignalSelected += weight;
                nueSelectionHistogramCollection.m_eNuHists.m_signalSelected->Fill(nu.m_eNu, weight);
                nueSelectionHistogramCollection.m_qSqrHists.m_signalSelected->Fill(nu.m_qSqr, weight);
                nueSelectionHistogramCollection.m_lepMomHists.m_signalSelected->Fill(nu.m_lepMom, weight);
                nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
            }
        }
        else
        {
            if (!isNueCCSignal)
            {
                nueBackgroundRejected += weight;
                nueSelectionHistogramCollection.m_eNuHists.m_backgroundRejected->Fill(nu.m_eNu, weight);
                nueSelectionHistogramCollection.m_qSqrHists.m_backgroundRejected->Fill(nu.m_qSqr, weight);
                nueSelectionHistogramCollection.m_lepMomHists.m_backgroundRejected->Fill(nu.m_lepMom, weight);
                nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_backgroundRejected->Fill(nu.m_lepNuOpeningAngle, weight);
            }

            if (PassNumuSelection(nu))
            {    
                numuSelected += weight;
                numuSelectionHistogramCollection.m_eNuHists.m_selected->Fill(nu.m_eNu, weight);
                numuSelectionHistogramCollection.m_qSqrHists.m_selected->Fill(nu.m_qSqr, weight);
                numuSelectionHistogramCollection.m_lepMomHists.m_selected->Fill(nu.m_lepMom, weight);
                numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_selected->Fill(nu.m_lepNuOpeningAngle, weight);

                if (isNumuCCSignal)
                {
                    numuSignalSelected += weight;
                    numuSelectionHistogramCollection.m_eNuHists.m_signalSelected->Fill(nu.m_eNu, weight);
                    numuSelectionHistogramCollection.m_qSqrHists.m_signalSelected->Fill(nu.m_qSqr, weight);
                    numuSelectionHistogramCollection.m_lepMomHists.m_signalSelected->Fill(nu.m_lepMom, weight);
                    numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
                }
            }
            else
            {
                if (!isNumuCCSignal)
                {
                    numuBackgroundRejected += weight;
                    numuSelectionHistogramCollection.m_eNuHists.m_backgroundRejected->Fill(nu.m_eNu, weight);
                    numuSelectionHistogramCollection.m_qSqrHists.m_backgroundRejected->Fill(nu.m_qSqr, weight);
                    numuSelectionHistogramCollection.m_lepMomHists.m_backgroundRejected->Fill(nu.m_lepMom, weight);
                    numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_backgroundRejected->Fill(nu.m_lepNuOpeningAngle, weight);
                }
            }
        }
    }

    const double nueEfficiency(nueSignalSelected / nueSignal);
    const double nuePurity(nueSignalSelected / nueSelected);
    const double nueBackgroundRejection(nueBackgroundRejected / nueBackground);

    const double numuEfficiency(numuSignalSelected / numuSignal);
    const double numuPurity(numuSignalSelected / numuSelected);
    const double numuBackgroundRejection(numuBackgroundRejected / numuBackground);

    std::cout << "nueEfficiency: " << nueEfficiency * 100.0 << "%" << std::endl;
    std::cout << "nuePurity: " << nuePurity * 100.0 << "%" << std::endl;
    std::cout << "nueBackgroundRejection: " << nueBackgroundRejection * 100.0 << "%" << std::endl;

    std::cout << "numuEfficiency: " << numuEfficiency * 100.0 << "%" << std::endl;
    std::cout << "numuPurity: " << numuPurity * 100.0 << "%" << std::endl;
    std::cout << "numuBackgroundRejection: " << numuBackgroundRejection * 100.0 << "%" << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ProcessHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection)
{
    ProcessHistograms(nuSelectionHistogramCollection.m_eNuHists);
    ProcessHistograms(nuSelectionHistogramCollection.m_qSqrHists);
    ProcessHistograms(nuSelectionHistogramCollection.m_lepMomHists);
    ProcessHistograms(nuSelectionHistogramCollection.m_lepNuOpeningAngleHists);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ProcessHistograms(NuSelectionHistograms &nuSelectionHistograms)
{
    // throw if not same binning
    int nBins = nuSelectionHistograms.m_signal->GetXaxis()->GetNbins();

    // Efficiency
    for (int i = -1; i < nBins; ++i)
    {
        const double signalSelected(nuSelectionHistograms.m_signalSelected->GetBinContent(i + 1));
        const double signal(nuSelectionHistograms.m_signal->GetBinContent(i + 1));
        const float efficiency(signal > 0.0 ? signalSelected / signal : 0.0);
        nuSelectionHistograms.m_efficiency->SetBinContent(i + 1, efficiency);
        nuSelectionHistograms.m_efficiency->SetBinError(i + 1, 0.0);
    }

    // Purity
    for (int i = -1; i < nBins; ++i)
    {
        const double signalSelected(nuSelectionHistograms.m_signalSelected->GetBinContent(i + 1));
        const double selected(nuSelectionHistograms.m_selected->GetBinContent(i + 1));
        const float purity(selected > 0.0 ? signalSelected / selected : 0.0);
        nuSelectionHistograms.m_purity->SetBinContent(i + 1, purity);
        nuSelectionHistograms.m_purity->SetBinError(i + 1, 0.0);
    }

    // BackgroundRejected
    for (int i = -1; i < nBins; ++i)
    {
        const double backgroundRejected(nuSelectionHistograms.m_backgroundRejected->GetBinContent(i + 1));
        const double background(nuSelectionHistograms.m_background->GetBinContent(i + 1));
        const float backgroundRejection(background > 0.0 ? backgroundRejected / background : 0.0);
        nuSelectionHistograms.m_backgroundRejection->SetBinContent(i + 1, backgroundRejection);
        nuSelectionHistograms.m_backgroundRejection->SetBinError(i + 1, 0.0);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix)
{
    gStyle->SetOptStat(0);

    TCanvas * c1 = new TCanvas(histPrefix.c_str(), histPrefix.c_str());

    TH1D * scaledEnergyDistribution = (TH1D*) nuSelectionHistogramCollection.m_eNuHists.m_signal->Clone();
    scaledEnergyDistribution->Scale(10.0/scaledEnergyDistribution->GetEntries());

    scaledEnergyDistribution->GetYaxis()->SetRangeUser(0.0, 1.0);
    scaledEnergyDistribution->SetTitle(";TrueNeutrinoEnergy;Arbritray Units");
    scaledEnergyDistribution->SetFillColor(kGray);
    scaledEnergyDistribution->SetLineColor(kGray);

    nuSelectionHistogramCollection.m_eNuHists.m_purity->SetMarkerColor(kRed);
    nuSelectionHistogramCollection.m_eNuHists.m_purity->SetMarkerStyle(8);
    nuSelectionHistogramCollection.m_eNuHists.m_purity->SetMarkerSize(1);
    nuSelectionHistogramCollection.m_eNuHists.m_efficiency->SetMarkerColor(kBlack);
    nuSelectionHistogramCollection.m_eNuHists.m_efficiency->SetMarkerStyle(8);
    nuSelectionHistogramCollection.m_eNuHists.m_efficiency->SetMarkerSize(1);
    nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejection->SetMarkerColor(kBlue);
    nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejection->SetMarkerStyle(8);
    nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejection->SetMarkerSize(1);

    scaledEnergyDistribution->Draw("hist");
    nuSelectionHistogramCollection.m_eNuHists.m_purity->Draw("P same");
    nuSelectionHistogramCollection.m_eNuHists.m_efficiency->Draw("P same");
    nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejection->Draw("P same");

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_purity, "purity", "p");
    legend->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_efficiency, "efficiency", "p");
    legend->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejection, "background rejection", "p");
    legend->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu)
{
    double weight = getProb3ppWeights(DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, DEF_DELTA, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}

//------------------------------------------------------------------------------------------------------------------------------------------