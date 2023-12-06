#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"

void TuneNumuSelection(const std::string &inputFileName);
void DrawSelectionHistograms(NeutrinoEventVector &nuEventVector_full);
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full);
double ComputeSelectionMetric(const double nSignal, const TH1D *signal, const TH1D *flavourSignal, const TH1D *background, const double pandizzle, double &nueEfficiency, double &nuePurity);
double GetOscWeight(const NeutrinoEvent &nu);

bool PERFORM_OLD_NUE_SELECTION = false;
bool IS_NEUTRINO = true;
const bool IS_JAM_SELECTION = true;
//const double POT_CONVERSION =  1.1/1.47;
const double POT_CONVERSION =  1.0;

void TuneNumuSelection(const std::string &inputFileName_full)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_OLD_NUE_SELECTION ? "old " : "new ") << "\033[31m" << "nue selection" << "\033[0m" << std::endl; 
    std::cout << "\033[31m" << "For " << "\033[33m" << (IS_NEUTRINO ? "F" : "R") << "\033[31m" << "HC" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (IS_JAM_SELECTION ? "JAM-ENHANCED PANDRIZZLE " : "PANDRIZZLE ") << "\033[31m" << "selection" << "\033[0m" << std::endl; 
    std::cout << "\033[31m" << "Applied POT scale (because i am dumb): " << "\033[33m" << POT_CONVERSION << "\033[0m" << std::endl;

    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    DrawSelectionHistograms(nuEventVector_full);    
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

        if (IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION))
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)) * POT_CONVERSION);

        if (IsNumuCCSignal(nu, IS_NEUTRINO))
            pandizzle_signal->Fill(nu.m_selTrackPandizzleScore, weight);

        if (!IsNumuFlavourCCSignal(nu))
            pandizzle_background->Fill(nu.m_selTrackPandizzleScore, weight);
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
    int pandizzleBins = 100;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    double pandizzleStepSize = (pandizzleMax - pandizzleMin) / pandizzleBins;
    double pandizzleCuts[pandizzleBins];

    for (int i = 0; i < pandizzleBins; ++i)
        pandizzleCuts[i] = pandizzleMin + (i * pandizzleStepSize);

    TH1D * signal = new TH1D("signal", "signal", pandizzleBins, pandizzleMin, pandizzleMax);
    TH1D * flavourSignal = new TH1D("flavourSignal", "flavourSignal", pandizzleBins, pandizzleMin, pandizzleMax);
    TH1D * background = new TH1D("background", "background", pandizzleBins, pandizzleMin, pandizzleMax);

    double nSignal(0);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        const bool isRecoInFV(IsRecoInFiducialVolume(nu));
        const bool passNueSelection(PERFORM_OLD_NUE_SELECTION ? PassOldNueSelection(nu, IS_NEUTRINO) : IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)) * POT_CONVERSION);

        if (IsNumuCCSignal(nu, IS_NEUTRINO))
        {
            nSignal += weight;

            if (isRecoInFV && !passNueSelection)
                signal->Fill(nu.m_selTrackPandizzleScore, weight);
        }

        if (isRecoInFV && !passNueSelection) 
        {
            if (IsNumuFlavourCCSignal(nu))
            {
                flavourSignal->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else
            {
                background->Fill(nu.m_selTrackPandizzleScore, weight);
            }
        }
    }

    unsigned int count(0);
    double bestPandizzle(0.0), bestSelectionMetric(0.0), bestEfficiency(0.0), bestPurity(0.0);
    double nueEfficiency[pandizzleBins], nuePurity[pandizzleBins], selectionMetric[pandizzleBins];

    for (int pandizzleIndex = 0; pandizzleIndex < pandizzleBins; ++pandizzleIndex)
    {
        ++count;
        if (count % 1000 == 0)
        {
            std::cout << "Iteration: " << count << "/" << pandizzleBins << std::endl;
        }

        selectionMetric[pandizzleIndex] = (ComputeSelectionMetric(nSignal, signal, flavourSignal, background, pandizzleCuts[pandizzleIndex], nueEfficiency[pandizzleIndex], nuePurity[pandizzleIndex]));

        if (selectionMetric[pandizzleIndex] > bestSelectionMetric)
        {
            bestPandizzle = pandizzleCuts[pandizzleIndex];
            bestSelectionMetric = selectionMetric[pandizzleIndex];
            bestEfficiency = nueEfficiency[pandizzleIndex];
            bestPurity = nuePurity[pandizzleIndex];
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
    std::cout << "(a)numu efficiency: " << bestEfficiency << std::endl;
    std::cout << "(a)numu purity: " << bestPurity << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const double nSignal, const TH1D *signal, const TH1D *flavourSignal, const TH1D *background, const double pandizzle, double &nueEfficiency, double &nuePurity)
{
    double pandizzleCut(pandizzle);
    int pandizzleMinBin(0), pandizzleCutBin(-std::numeric_limits<int>::max()), pandizzleMaxBin(signal->GetXaxis()->GetNbins());

    for (int pandizzleIndex = 1; pandizzleIndex <= signal->GetNbinsX(); pandizzleIndex++)
    {
        const double binCenter = signal->GetXaxis()->GetBinCenter(pandizzleIndex);

        if ((binCenter < pandizzleCut) && (pandizzleIndex > pandizzleCutBin))
            pandizzleCutBin = pandizzleIndex;
    }

    if (pandizzleCutBin < 0)
    {
        std::cout << "1 couldn't find cut bin" << std::endl;
        return 0.0;
    }

    double selectedSignal(0.0);

    for (int pandizzleIndex = (pandizzleCutBin + 1); pandizzleIndex <= pandizzleMaxBin; ++pandizzleIndex)
        selectedSignal += signal->GetBinContent(pandizzleIndex);

    double selectedFlavourSignal(0.0);

    for (int pandizzleIndex = (pandizzleCutBin + 1); pandizzleIndex <= pandizzleMaxBin; ++pandizzleIndex)
        selectedFlavourSignal += flavourSignal->GetBinContent(pandizzleIndex);

    double selectedBackground(0.0);

    for (int pandizzleIndex = (pandizzleCutBin + 1); pandizzleIndex <= pandizzleMaxBin; ++pandizzleIndex)
        selectedBackground += background->GetBinContent(pandizzleIndex);

    nueEfficiency = selectedSignal / nSignal;

    double totalSelected = selectedFlavourSignal + selectedBackground;
    nuePurity = (totalSelected < std::numeric_limits<double>::epsilon()) ? 0.0 : selectedFlavourSignal / totalSelected;

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
