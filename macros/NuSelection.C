#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"
#include "NuSelection.h"

bool PERFORM_CVN_SELECTION = false;
bool PERFORM_OLD_NUE_SELECTION = false;
bool IS_NEUTRINO = true;
const bool IS_JAM_SELECTION = true;
//const double POT_CONVERSION =  1.1/1.47;
const double POT_CONVERSION =  1.0;

void NuSelection(const std::string &inputFileName)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_CVN_SELECTION ? "CVN " : "DIZZLE ") << "\033[31m" << "selection" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_OLD_NUE_SELECTION ? "old " : "new ") << "\033[31m" << "nue selection" << "\033[0m" << std::endl; 
    std::cout << "\033[31m" << "For " << "\033[33m" << (IS_NEUTRINO ? "F" : "R") << "\033[31m" << "HC" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "Applied POT scale (because i am dumb): " << "\033[33m" << POT_CONVERSION << "\033[0m" << std::endl;

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

    TFile * outputFile = new TFile("NuSelectionHistograms.root", "CREATE");

    WriteHistogramCollection(nueHistogramCollection, "nue_");
    WriteHistogramCollection(numuHistogramCollection, "numu_");

    outputFile->Close();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void InitialiseNuSelectionHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix)
{
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_eNuHists, histPrefix + "_ENu", 32, 0.0, 8.0);
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
    nuSelectionHistograms.m_flavourSignalSelected = new TH1D((histPrefix + "_FlavourSignalSelected").c_str(), (histPrefix + "_FlavourSignalSelected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_backgroundRejected = new TH1D((histPrefix + "_BackgroundRejected").c_str(), (histPrefix + "_BackgroundRejected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_efficiency = new TH1D((histPrefix + "_Efficiency").c_str(), (histPrefix + "_Efficiency").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_purity = new TH1D((histPrefix + "_Purity").c_str(), (histPrefix + "_Purity").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_backgroundRejection = new TH1D((histPrefix + "_BackgroundRejection").c_str(), (histPrefix + "_BackgroundRejection").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_selectedRecoEnergy = new TH1D((histPrefix + "_SelectedRecoEnergy").c_str(), (histPrefix + "_SelectedRecoEnergy").c_str(), nBins, xMin, xMax);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void PerformSelection(const NeutrinoEventVector &nuVector, NuSelectionHistogramCollection &nueSelectionHistogramCollection, 
    NuSelectionHistogramCollection &numuSelectionHistogramCollection)
{
    double nueSignal(0.0), nueBackground(0.0), nueSelected(0.0), nueSignalSelected(0.0), nueFlavourSignalSelected(0.0), nueBackgroundRejected(0.0);
    double numuSignal(0.0), numuBackground(0.0), numuSelected(0.0), numuSignalSelected(0.0), numuFlavourSignalSelected(0.0), numuBackgroundRejected(0.0);

    for (int i = 0; i < nuVector.size(); i++)
    {
        const NeutrinoEvent nu = nuVector[i];
        const bool isNueCCSignal(IsNueCCSignal(nu, IS_NEUTRINO));
        const bool isNueFlavourCCSignal(IsNueFlavourCCSignal(nu));

        const bool isNumuCCSignal(isNueCCSignal ? false : IsNumuCCSignal(nu, IS_NEUTRINO));
        const bool isNumuFlavourCCSignal(isNueFlavourCCSignal ? false : IsNumuFlavourCCSignal(nu));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)) * POT_CONVERSION);

        // Fill truth histograms
        if (isNueCCSignal)
        { 
            nueSignal += weight;
            nueSelectionHistogramCollection.m_eNuHists.m_signal->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_signal->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_signal->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signal->Fill(nu.m_lepNuOpeningAngle, weight);
        }

        // Background is everything that is not nue flavour CC
        if (!isNueFlavourCCSignal)
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

        bool passNueSelection(PERFORM_CVN_SELECTION ? PassCVNNueSelection(nu) : IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION));

        // Apply Selection
        if (passNueSelection)
        {
            nueSelected += weight;
            nueSelectionHistogramCollection.m_eNuHists.m_selected->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_selected->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_selected->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_selected->Fill(nu.m_lepNuOpeningAngle, weight);
            nueSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy->Fill(nu.m_nueRecoENu, weight);

            // To be used as numerator in efficiency
            if (isNueCCSignal)
            {
                nueSignalSelected += weight;
                nueSelectionHistogramCollection.m_eNuHists.m_signalSelected->Fill(nu.m_eNu, weight);
                nueSelectionHistogramCollection.m_qSqrHists.m_signalSelected->Fill(nu.m_qSqr, weight);
                nueSelectionHistogramCollection.m_lepMomHists.m_signalSelected->Fill(nu.m_lepMom, weight);
                nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
            }

            // To be used as numerator in purity
            if (isNueFlavourCCSignal)
            {
                nueFlavourSignalSelected += weight;
                nueSelectionHistogramCollection.m_eNuHists.m_flavourSignalSelected->Fill(nu.m_eNu, weight);
                nueSelectionHistogramCollection.m_qSqrHists.m_flavourSignalSelected->Fill(nu.m_qSqr, weight);
                nueSelectionHistogramCollection.m_lepMomHists.m_flavourSignalSelected->Fill(nu.m_lepMom, weight);
                nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_flavourSignalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
            }
        }
        else
        {
            // Count nue background events rejected by nue selection
            if (!isNueFlavourCCSignal)
            {
                nueBackgroundRejected += weight;
                nueSelectionHistogramCollection.m_eNuHists.m_backgroundRejected->Fill(nu.m_eNu, weight);
                nueSelectionHistogramCollection.m_qSqrHists.m_backgroundRejected->Fill(nu.m_qSqr, weight);
                nueSelectionHistogramCollection.m_lepMomHists.m_backgroundRejected->Fill(nu.m_lepMom, weight);
                nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_backgroundRejected->Fill(nu.m_lepNuOpeningAngle, weight);
            }

            // Can only measure background rejection on events that we actually get to reject - in my probably wrong opinion
            if (!isNumuFlavourCCSignal)
            {
                numuBackground += weight;
                numuSelectionHistogramCollection.m_eNuHists.m_background->Fill(nu.m_eNu, weight);
                numuSelectionHistogramCollection.m_qSqrHists.m_background->Fill(nu.m_qSqr, weight);
                numuSelectionHistogramCollection.m_lepMomHists.m_background->Fill(nu.m_lepMom, weight);
                numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_background->Fill(nu.m_lepNuOpeningAngle, weight);
            }

            const bool passNumuSelection(PERFORM_CVN_SELECTION ? PassCVNNumuSelection(nu) : IsNumuSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION));

            if (passNumuSelection)
            {
                numuSelected += weight;
                numuSelectionHistogramCollection.m_eNuHists.m_selected->Fill(nu.m_eNu, weight);
                numuSelectionHistogramCollection.m_qSqrHists.m_selected->Fill(nu.m_qSqr, weight);
                numuSelectionHistogramCollection.m_lepMomHists.m_selected->Fill(nu.m_lepMom, weight);
                numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_selected->Fill(nu.m_lepNuOpeningAngle, weight);
                numuSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy->Fill(nu.m_numuRecoENu, weight);

                // To be used as numerator in efficiency
                if (isNumuCCSignal)
                {
                    numuSignalSelected += weight;
                    numuSelectionHistogramCollection.m_eNuHists.m_signalSelected->Fill(nu.m_eNu, weight);
                    numuSelectionHistogramCollection.m_qSqrHists.m_signalSelected->Fill(nu.m_qSqr, weight);
                    numuSelectionHistogramCollection.m_lepMomHists.m_signalSelected->Fill(nu.m_lepMom, weight);
                    numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
                }

                // To be used as numerator in purity
                if (isNumuFlavourCCSignal)
                {
                    numuFlavourSignalSelected += weight;
                    numuSelectionHistogramCollection.m_eNuHists.m_flavourSignalSelected->Fill(nu.m_eNu, weight);
                    numuSelectionHistogramCollection.m_qSqrHists.m_flavourSignalSelected->Fill(nu.m_qSqr, weight);
                    numuSelectionHistogramCollection.m_lepMomHists.m_flavourSignalSelected->Fill(nu.m_lepMom, weight);
                    numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_flavourSignalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
                }
            }
            else
            {
                if (!isNumuFlavourCCSignal)
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

    std::cout << "nueFlavourSignalSelected: " << nueFlavourSignalSelected << std::endl;
    std::cout << "nueSelected: " << nueSelected << std::endl;
    const double nueEfficiency(nueSignalSelected / nueSignal);
    const double nuePurity(nueFlavourSignalSelected / nueSelected);
    const double nueBackgroundRejection(nueBackgroundRejected / nueBackground);

    const double numuEfficiency(numuSignalSelected / numuSignal);
    const double numuPurity(numuFlavourSignalSelected / numuSelected);
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
    // throw if not same binning (TO DO)
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
        const double flavourSignalSelected(nuSelectionHistograms.m_flavourSignalSelected->GetBinContent(i + 1));
        const double selected(nuSelectionHistograms.m_selected->GetBinContent(i + 1));
        const float purity(selected > 0.0 ? flavourSignalSelected / selected : 0.0);
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

    TCanvas * c1 = new TCanvas(("SelectedHistogramGlobal_True" + histPrefix).c_str(), ("SelectedHistogramGlobal_True" + histPrefix).c_str());

    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetTitle(";True Neutrino Energy [GeV];nEvents");

    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetLineWidth(2);
    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetLineColor(kBlack);
    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetFillColor(kGray);
    nuSelectionHistogramCollection.m_eNuHists.m_selected->SetLineWidth(2);
    nuSelectionHistogramCollection.m_eNuHists.m_selected->SetLineColor(kBlue);

    nuSelectionHistogramCollection.m_eNuHists.m_signal->Draw("hist");
    nuSelectionHistogramCollection.m_eNuHists.m_selected->Draw("hist same");

    auto legendTrue = new TLegend(0.1,0.7,0.48,0.9);
    legendTrue->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_signal, "Signal", "l");
    legendTrue->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_selected, "Selected", "l");
    legendTrue->Draw("same");
    //

    /*    
    TCanvas * c2 = new TCanvas(("SelectedHistogramGlobal_Reco" + histPrefix).c_str(), ("SelectedHistogramGlobal_Reco" + histPrefix).c_str());

    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetTitle(";True/Reconstructed Neutrino Energy [GeV];nEvents");

    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetLineWidth(2);
    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetLineColor(kBlack);
    nuSelectionHistogramCollection.m_eNuHists.m_signal->SetFillColor(kGray);
    nuSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy->SetLineWidth(2);
    nuSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy->SetLineColor(kBlue);

    nuSelectionHistogramCollection.m_eNuHists.m_signal->Draw("hist");
    nuSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy->Draw("hist same");

    auto legendReco = new TLegend(0.1,0.7,0.48,0.9);
    legendReco->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_signal, "Signal", "l");
    legendReco->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy, "Selected", "l");
    legendReco->Draw("same");
    */

    TCanvas * c3 = new TCanvas(("SelectedHistogramShape" + histPrefix).c_str(), ("SelectedHistogramShape" + histPrefix).c_str());

    TH1D * normalisedTrueDistribution = (TH1D*)nuSelectionHistogramCollection.m_eNuHists.m_signal->Clone();
    normalisedTrueDistribution->Scale(1.0 / normalisedTrueDistribution->Integral(1,32));

    TH1D * normalisedSelectedDistribution = (TH1D*)nuSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy->Clone();
    normalisedSelectedDistribution->Scale(1.0 / normalisedSelectedDistribution->Integral(1,32));

    normalisedTrueDistribution->SetTitle(";True/Reco Neutrino Energy [GeV];Fraction of Events");
    normalisedTrueDistribution->SetLineWidth(2);
    normalisedTrueDistribution->SetLineColor(kBlack);
    normalisedTrueDistribution->SetFillColor(kGray);
    normalisedSelectedDistribution->SetLineWidth(2);
    normalisedSelectedDistribution->SetLineColor(kBlue);

    normalisedTrueDistribution->Draw("hist");
    normalisedSelectedDistribution->Draw("hist same");

    auto legendShape = new TLegend(0.1,0.7,0.48,0.9);
    legendShape->AddEntry(normalisedTrueDistribution, "Normalised Signal", "l");
    legendShape->AddEntry(normalisedSelectedDistribution, "Normalised Selected", "l");
    legendShape->Draw("same");

    TCanvas * c4 = new TCanvas(("SelectionMetrics" + histPrefix).c_str(), ("SelectionMetrics" + histPrefix).c_str());

    TH1D * scaledEnergyDistribution = (TH1D*) nuSelectionHistogramCollection.m_eNuHists.m_signal->Clone();

    float highestBinWeight(0);
    for (int i = 1; i <= scaledEnergyDistribution->GetNbinsX(); ++i)
    {
        if (scaledEnergyDistribution->GetBinContent(i) > highestBinWeight)
            highestBinWeight = scaledEnergyDistribution->GetBinContent(i);
    }

    for (int i = 1; i <= scaledEnergyDistribution->GetNbinsX(); ++i)
    {
        float scaledWeight(scaledEnergyDistribution->GetBinContent(i) / highestBinWeight);
        scaledEnergyDistribution->SetBinContent(i, scaledWeight);
    }

    scaledEnergyDistribution->GetYaxis()->SetRangeUser(0.0, 1.1);
    scaledEnergyDistribution->SetTitle(";True Neutrino Energy [GeV];Arbritray Units");
    scaledEnergyDistribution->SetFillColor(kGray);
    scaledEnergyDistribution->SetLineColor(kBlack);
    scaledEnergyDistribution->SetLineWidth(2);

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
    legend->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_efficiency, "Selection Efficiency", "p");
    legend->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_purity, "Selection Purity", "p");
    legend->AddEntry(nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejection, "Selection Background Rejection", "p");
    //legend->SetBorderSize(0);
    legend->Draw("same");
}


//------------------------------------------------------------------------------------------------------------------------------------------

void WriteHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix)
{
    nuSelectionHistogramCollection.m_eNuHists.m_signal->Write((histPrefix + "eNuHist_signal").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_background->Write((histPrefix + "eNuHist_background").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_selected->Write((histPrefix + "eNuHist_selected").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_signalSelected->Write((histPrefix + "eNuHist_signalSelected").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_flavourSignalSelected->Write((histPrefix + "eNuHist_flavourSignalSelected").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejected->Write((histPrefix + "eNuHist_backgroundRejected").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_efficiency->Write((histPrefix + "eNuHist_efficiency").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_purity->Write((histPrefix + "eNuHist_purity").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_backgroundRejection->Write((histPrefix + "eNuHist_backgroundRejection").c_str());
    nuSelectionHistogramCollection.m_eNuHists.m_selectedRecoEnergy->Write((histPrefix + "eNuHist_electedRecoEnergy").c_str());
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu)
{
    double weight = getProb3ppWeights(DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, DEF_DELTA, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FindEventsToPass(const NeutrinoEventVector &nuVector, std::vector<int> &eventsToPass)
{
    double nueSelected(0.0), nueBackgroundSelected(0.0), nueFlavourSignalSelected(0.0);

    std::vector<int> selectedBackgroundIndices;

    double TARGET_PURITY = 0.05;
    double MIN_ENERGY = 0.0;
    double MAX_ENERGY = 100000000.0;

    for (int i = 0; i < nuVector.size(); i++)
    {
        const NeutrinoEvent &nu(nuVector[i]);
        const bool isNueFlavourCCSignal(IsNueFlavourCCSignal(nu));
        const bool passNueSelection(PERFORM_CVN_SELECTION ? PassCVNNueSelection(nu) : PassNueSelection(nu, true));
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)) * POT_CONVERSION);

        // Apply Selection
        if (passNueSelection)
        {
            nueSelected += weight;

            if (isNueFlavourCCSignal)
            {
                nueFlavourSignalSelected += weight;
            }
            else
            {
                nueBackgroundSelected += weight; 

                if ((nu.m_nueRecoENu > MIN_ENERGY) && (nu.m_nueRecoENu < MAX_ENERGY))
                    selectedBackgroundIndices.push_back(i);
            }
        }
    }

    for (int element : selectedBackgroundIndices)
        std::cout << "selected background index: " << element << std::endl;

    const double targetSelectedBackground((1.0 / ((TARGET_PURITY / nueFlavourSignalSelected) + (1.0 / nueSelected))) - nueFlavourSignalSelected);
    const double backgroundToRemove(nueBackgroundSelected - targetSelectedBackground);

    std::cout << "nueFlavourSignalSelected: " << nueFlavourSignalSelected << std::endl;
    std::cout << "nueSelected: " << nueSelected << std::endl;
    std::cout << "nueBackgroundSelected: " << nueBackgroundSelected << std::endl;
    std::cout << "targetSelectedBackground: " << targetSelectedBackground << std::endl;
    std::cout << "backgroundToRemove: " << backgroundToRemove << std::endl;

    TRandom * randomNumber = new TRandom();

    double removedBackground = 0.0;
    while((eventsToPass.size() != selectedBackgroundIndices.size()) && (removedBackground < backgroundToRemove))
    {
        const int randomIndex(std::floor(randomNumber->Uniform(0, selectedBackgroundIndices.size() - 1)));

        if (std::find(eventsToPass.begin(), eventsToPass.end(), selectedBackgroundIndices[randomIndex]) != eventsToPass.end())
            continue;

        const NeutrinoEvent &nu(nuVector[selectedBackgroundIndices[randomIndex]]);
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)) * POT_CONVERSION);

        removedBackground += weight;
        std::cout << "randomIndex: " << randomIndex << std::endl;
        eventsToPass.push_back(selectedBackgroundIndices[randomIndex]);
    }

    for (int entr : eventsToPass)
        std::cout << "event to skip index: " << entr << std::endl;

    std::cout << "removedBackground: " << removedBackground << std::endl;
}
