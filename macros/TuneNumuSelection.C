#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"

void TuneNumuSelection(const std::string &inputFileName);
void DrawSelectionHistograms(NeutrinoEventVector &nuEventVector_full);
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full);
double ComputeSelectionMetric(const double nSignal, const TH1D *signal, const TH1D *background, const double pandizzle, double &nueEfficiency, double &nuePurity);
double GetOscWeight(const NeutrinoEvent &nu);

bool PERFORM_OLD_NUE_SELECTION = false;

void TuneNumuSelection(const std::string &inputFileName_full)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_OLD_NUE_SELECTION ? "old " : "new ") << "\033[31m" << "nue selection" << "\033[0m" << std::endl; 

    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    //DrawSelectionHistograms(nuEventVector_full);    
    FindSelectionCuts(nuEventVector_full);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawSelectionHistograms(NeutrinoEventVector &nuEventVector_full)
{
    int pandizzleBins = 25;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;

    TH1D * pandizzle_signal = new TH1D("pandizzle_signal", "pandizzle_signal", pandizzleBins, pandizzleMin, pandizzleMax);
    TH1D * pandizzle_background = new TH1D("pandizzle_background", "pandizzle_background", pandizzleBins, pandizzleMin, pandizzleMax);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        if (PassNueSelection(nu))
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu));

        if (IsNumuCCSignal(nu))
        {
            pandizzle_signal->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else
        {
            pandizzle_background->Fill(nu.m_selTrackPandizzleScore, weight);
        }
    }

    TCanvas * pandizzle = new TCanvas("pandizzle", "pandizzle");
    pandizzle_signal->SetTitle(";Pandizzle Score;nEvents");
    pandizzle_signal->SetLineColor(kBlue);
    pandizzle_background->SetLineColor(kRed);
    pandizzle_signal->Draw("hist same");
    pandizzle_background->Draw("hist same");

    auto pandizzleLegend = new TLegend(0.1,0.7,0.48,0.9);
    pandizzleLegend->AddEntry(pandizzle_signal, "signal", "l");
    pandizzleLegend->AddEntry(pandizzle_background, "background", "l");
    pandizzleLegend->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full)
{
    int pandizzleBins = 1000;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    double pandizzleStepSize = (pandizzleMax - pandizzleMin) / pandizzleBins;
    double pandizzleCuts[pandizzleBins];

    for (int i = 0; i < pandizzleBins; ++i)
        pandizzleCuts[i] = pandizzleMin + (i * pandizzleStepSize);

    TH1D * signal = new TH1D("signal", "signal", pandizzleBins, pandizzleMin, pandizzleMax);
    TH1D * background = new TH1D("background", "background", pandizzleBins, pandizzleMin, pandizzleMax);

    double nSignal(0);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        const bool isRecoInFV(IsRecoInFiducialVolume(nu));
        const bool passNueSelection(PERFORM_OLD_NUE_SELECTION ? PassOldNueSelection(nu) : PassNueSelection(nu));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu));

        if (IsNumuCCSignal(nu))
        {
            nSignal += weight;

            if (isRecoInFV && !passNueSelection)
                signal->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else
        {
            if (isRecoInFV && !passNueSelection)
                background->Fill(nu.m_selTrackPandizzleScore, weight);
        }
    }

    double bestPandizzle(0.0);
    double bestSelectionMetric(0.0);
    unsigned int count(0);

    double nueEfficiency[pandizzleBins], nuePurity[pandizzleBins], selectionMetric[pandizzleBins];

    for (int pandizzleIndex = 0; pandizzleIndex < pandizzleBins; ++pandizzleIndex)
    {
        ++count;
        std::cout << "Iteration: " << count << "/" << pandizzleBins << std::endl;

        selectionMetric[pandizzleIndex] = (ComputeSelectionMetric(nSignal, signal, background, pandizzleCuts[pandizzleIndex], nueEfficiency[pandizzleIndex], nuePurity[pandizzleIndex]));

        if (selectionMetric[pandizzleIndex] > bestSelectionMetric)
        {
            bestPandizzle = pandizzleCuts[pandizzleIndex];
            bestSelectionMetric = selectionMetric[pandizzleIndex];
        }
    }

    TGraph * efficiencyGraph = new TGraph(pandizzleBins, pandizzleCuts, nueEfficiency);
    TGraph * purityGraph = new TGraph(pandizzleBins, pandizzleCuts, nuePurity);
    TGraph * selectionMetricGraph = new TGraph(pandizzleBins, pandizzleCuts, selectionMetric);

    TCanvas * c1 = new TCanvas ("c1", "c1");
    purityGraph->SetTitle(";PandizzleScore;");
    purityGraph->SetLineColor(kRed);    
    purityGraph->SetLineWidth(2);    
    efficiencyGraph->SetLineColor(kBlue);    
    efficiencyGraph->SetLineWidth(2);
    selectionMetricGraph->SetLineWidth(2);
    purityGraph->Draw("AC");
    efficiencyGraph->Draw("C");
    selectionMetricGraph->Draw("C");

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(purityGraph, "purity", "l");
    legend->AddEntry(efficiencyGraph, "efficiency", "l");
    legend->AddEntry(selectionMetricGraph, "efficiency x purity", "l");
    legend->Draw("same");

    std::cout << "Best selection metric: " << bestSelectionMetric << " obtained with: " << std::endl;
    std::cout << "Pandizzle cut: " << bestPandizzle << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const double nSignal, const TH1D *signal, const TH1D *background, const double pandizzle, double &nueEfficiency, double &nuePurity)
{
    double pandizzleMin(-std::numeric_limits<int>::max()), pandizzleCut(pandizzle), pandizzleMax(std::numeric_limits<int>::max());
    int pandizzleMinBin(std::numeric_limits<int>::max()), pandizzleCutBin(-std::numeric_limits<int>::max()), pandizzleMaxBin(-std::numeric_limits<int>::max());

    for (int pandizzleIndex = 0; pandizzleIndex <= (signal->GetNbinsX() + 1); pandizzleIndex++)
    {
        const double binCenter = signal->GetXaxis()->GetBinCenter(pandizzleIndex);

        if (pandizzleIndex > pandizzleMaxBin)
            pandizzleMaxBin = pandizzleIndex;

        if (pandizzleIndex < pandizzleMinBin)
            pandizzleMinBin = pandizzleIndex;

        if ((binCenter < pandizzleCut) && (pandizzleIndex > pandizzleCutBin))
            pandizzleCutBin = pandizzleIndex;
    }

    double selectedSignal(0);

    for (int pandizzleIndex = (pandizzleCutBin + 1); pandizzleIndex <= pandizzleMaxBin; ++pandizzleIndex)
        selectedSignal += signal->GetBinContent(pandizzleIndex);

    double selectedBackground(0);

    for (int pandizzleIndex = (pandizzleCutBin + 1); pandizzleIndex <= pandizzleMaxBin; ++pandizzleIndex)
        selectedBackground += background->GetBinContent(pandizzleIndex);

    double totalSelected = selectedSignal + selectedBackground;
    nueEfficiency = selectedSignal / nSignal;
    nuePurity = (totalSelected < std::numeric_limits<double>::epsilon()) ? 0.0 : selectedSignal / (selectedSignal + selectedBackground);
    const double selectionMetric(nueEfficiency * nuePurity);

    return selectionMetric;
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
