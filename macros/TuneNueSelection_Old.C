#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"

void TuneNueSelection_Old(const std::string &inputFileName);
void DrawSelectionHistograms(NeutrinoEventVector &nuEventVector_full);
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full);
double ComputeSelectionMetric(const double nSignal, const TH3D *signal, const TH3D *background, const double pandizzle, const double electronScore, const double showerSep);
double GetOscWeight(const NeutrinoEvent &nu);

void TuneNueSelection_Old(const std::string &inputFileName_full)
{
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

    int electronScoreBins = 100;
    double electronScoreMin = -1.0, electronScoreMax = 1.0;

    TH1D * electronScore_signal = new TH1D("electronScore_signal", "electronScore_signal", electronScoreBins, electronScoreMin, electronScoreMax);
    TH1D * electronScore_background = new TH1D("electronScore_background", "electronScore_background", electronScoreBins, electronScoreMin, electronScoreMax);

    int showerSepBins = 50;
    double showerSepMin = 0.0, showerSepMax = 50.0;

    TH1D * showerSep_signal = new TH1D("showerSep_signal", "showerSep_signal", showerSepBins, showerSepMin, showerSepMax);
    TH1D * showerSep_background = new TH1D("showerSep_background", "showerSep_background", showerSepBins, showerSepMin, showerSepMax);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu));

        const double showerDisplacementX(nu.m_selShowerRecoStartX - nu.m_nuRecoVertexX);
        const double showerDisplacementY(nu.m_selShowerRecoStartY - nu.m_nuRecoVertexY);
        const double showerDisplacementZ(nu.m_selShowerRecoStartZ - nu.m_nuRecoVertexZ);
        const double showerDisplacementSquared((showerDisplacementX * showerDisplacementX) + (showerDisplacementY * showerDisplacementY) + (showerDisplacementZ * showerDisplacementZ));
        const double showerDisplacement(std::sqrt(showerDisplacementSquared));

        if (IsNueCCSignal(nu))
        {
            pandizzle_signal->Fill(nu.m_selTrackPandizzleScore, weight);
            electronScore_signal->Fill(nu.m_selShowerElectronScore, weight);
            showerSep_signal->Fill(showerDisplacement, weight);
        }
        else
        {
            pandizzle_background->Fill(nu.m_selTrackPandizzleScore, weight);
            electronScore_background->Fill(nu.m_selShowerElectronScore, weight);
            showerSep_background->Fill(showerDisplacement, weight);
        }
    }

    gStyle->SetOptStat(0);

    TCanvas * pandizzle = new TCanvas("pandizzle", "pandizzle");
    pandizzle_background->SetTitle(";Pandizzle Score;nEvents");
    pandizzle_signal->SetLineColor(kBlue);
    pandizzle_background->SetLineColor(kRed);
    pandizzle_background->Draw("hist same");
    pandizzle_signal->Draw("hist same");

    auto pandizzleLegend = new TLegend(0.1,0.7,0.48,0.9);
    pandizzleLegend->AddEntry(pandizzle_signal, "signal", "l");
    pandizzleLegend->AddEntry(pandizzle_background, "background", "l");
    pandizzleLegend->Draw("same");

    TCanvas * electronScore = new TCanvas("electronScore", "electronScore");
    electronScore_background->SetTitle(";ElectronScore Score;nEvents");
    electronScore_signal->SetLineColor(kBlue);
    electronScore_background->SetLineColor(kRed);
    electronScore_background->Draw("hist same");
    electronScore_signal->Draw("hist same");

    auto electronScoreLegend = new TLegend(0.1,0.7,0.48,0.9);
    electronScoreLegend->AddEntry(electronScore_signal, "signal", "l");
    electronScoreLegend->AddEntry(electronScore_background, "background", "l");
    electronScoreLegend->Draw();

    TCanvas * showerSep = new TCanvas("showerSep", "showerSep");
    showerSep_background->SetTitle(";ShowerSep Score;nEvents");
    showerSep_signal->SetLineColor(kBlue);
    showerSep_background->SetLineColor(kRed);
    showerSep_background->Draw("hist same");
    showerSep_signal->Draw("hist same");

    auto showerSepLegend = new TLegend(0.1,0.7,0.48,0.9);
    showerSepLegend->AddEntry(showerSep_signal, "signal", "l");
    showerSepLegend->AddEntry(showerSep_background, "background", "l");
    showerSepLegend->Draw();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full)
{
    int pandizzleBins = 100;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    double pandizzleStepSize = (pandizzleMax - pandizzleMin) / pandizzleBins;
    std::vector<double> pandizzleCuts;

    for (int i = 0; i < pandizzleBins; ++i)
    {
        const double pandizzleCut(pandizzleMin + (i * pandizzleStepSize));

        pandizzleCuts.push_back(pandizzleCut);
    }

    int electronScoreBins = 100;
    double electronScoreMin = -1.0, electronScoreMax = 1.0;
    double electronScoreStepSize = (electronScoreMax - electronScoreMin) / electronScoreBins;
    std::vector<double> electronScoreCuts;

    for (int i = 0; i < electronScoreBins; ++i)
    {
        const double electronScoreCut(electronScoreMin + (i * electronScoreStepSize));

        electronScoreCuts.push_back(electronScoreCut);
    }

    int showerSepBins = 50;
    double showerSepMin = 0.0, showerSepMax = 10.0;
    double showerSepStepSize = (showerSepMax - showerSepMin) / showerSepBins;
    std::vector<double> showerSepCuts;

    for (int i = 0; i < showerSepBins; ++i)
    {
        const double showerSepCut(showerSepMin + (i * showerSepStepSize));

        showerSepCuts.push_back(showerSepCut);
    }

    TH3D * signal = new TH3D("signal", "signal", pandizzleBins, pandizzleMin, pandizzleMax, electronScoreBins, electronScoreMin, electronScoreMax, showerSepBins, showerSepMin, showerSepMax);
    TH3D * background = new TH3D("background", "background", pandizzleBins, pandizzleMin, pandizzleMax, electronScoreBins, electronScoreMin, electronScoreMax, showerSepBins, showerSepMin, showerSepMax);

    double nSignal(0);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        bool isRecoInFV(IsRecoInFiducialVolume(nu));

        //const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu));

        const double showerDisplacementX(nu.m_selShowerRecoStartX - nu.m_nuRecoVertexX);
        const double showerDisplacementY(nu.m_selShowerRecoStartY - nu.m_nuRecoVertexY);
        const double showerDisplacementZ(nu.m_selShowerRecoStartZ - nu.m_nuRecoVertexZ);
        const double showerDisplacementSquared((showerDisplacementX * showerDisplacementX) + (showerDisplacementY * showerDisplacementY) + (showerDisplacementZ * showerDisplacementZ));
        const double showerDisplacement(std::sqrt(showerDisplacementSquared));

        if (IsNueCCSignal(nu))
        {
            nSignal += weight;

            if (isRecoInFV)
                signal->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerElectronScore, showerDisplacement, weight);
        }
        else
        {
            if (isRecoInFV)
                background->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerElectronScore, showerDisplacement, weight);
        }
    }

    const int nTests(pandizzleCuts.size() * electronScoreCuts.size() * showerSepCuts.size());
    double bestPandizzle(0.0), bestElectronScore(0.0), bestShowerSep(0.0);
    double bestSelectionMetric(0.0);
    unsigned int count(0);

    for (int pandizzleIndex = 0; pandizzleIndex < pandizzleCuts.size(); ++pandizzleIndex)
    {
        const double pandizzleCut = pandizzleCuts.at(pandizzleIndex);

        for (int electronScoreIndex = 0; electronScoreIndex < electronScoreCuts.size(); ++electronScoreIndex)
        {
            const double electronScoreCut = electronScoreCuts.at(electronScoreIndex);

            for (int showerSepIndex = 0; showerSepIndex < showerSepCuts.size(); ++showerSepIndex)
            {
                ++count;

                //if (count % 100 == 0)
                    std::cout << "Iteration: " << count << "/" << nTests << std::endl;

                const double showerSepCut = showerSepCuts.at(showerSepIndex);
                const double selectionMetric(ComputeSelectionMetric(nSignal, signal, background, pandizzleCut, electronScoreCut, showerSepCut));

                if (selectionMetric > bestSelectionMetric)
                {
                    bestPandizzle = pandizzleCut;
                    bestElectronScore = electronScoreCut;
                    bestShowerSep = showerSepCut;
                    bestSelectionMetric = selectionMetric;
                }
            }
        }
    }

    std::cout << "Best selection metric: " << bestSelectionMetric << " obtained with: " << std::endl;
    std::cout << "Pandizzle cut: " << bestPandizzle << std::endl;
    std::cout << "Electron score cut: " << bestElectronScore << std::endl;
    std::cout << "Shower separation cut: " << bestShowerSep << std::endl;

}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const double nSignal, const TH3D *signal, const TH3D *background, const double pandizzle, const double electronScore, const double showerSep)
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

    double electronScoreMin(-std::numeric_limits<int>::max()), electronScoreCut(electronScore), electronScoreMax(std::numeric_limits<int>::max());
    int electronScoreMinBin(std::numeric_limits<int>::max()), electronScoreCutBin(-std::numeric_limits<int>::max()), electronScoreMaxBin(-std::numeric_limits<int>::max());

    for (int electronScoreIndex = 0; electronScoreIndex <= (signal->GetNbinsY() + 1); electronScoreIndex++)
    {
        const double binCenter = signal->GetYaxis()->GetBinCenter(electronScoreIndex);

        if (electronScoreIndex > electronScoreMaxBin)
            electronScoreMaxBin = electronScoreIndex;

        if (electronScoreIndex < electronScoreMinBin)
            electronScoreMinBin = electronScoreIndex;

        if ((binCenter < electronScoreCut) && (electronScoreIndex > electronScoreCutBin))
            electronScoreCutBin = electronScoreIndex;
    }

    double showerSepMin(-std::numeric_limits<int>::max()), showerSepCut(showerSep), showerSepMax(std::numeric_limits<int>::max());
    int showerSepMinBin(std::numeric_limits<int>::max()), showerSepCutBin(-std::numeric_limits<int>::max()), showerSepMaxBin(-std::numeric_limits<int>::max());

    for (int showerSepIndex = 0; showerSepIndex <= (signal->GetNbinsZ() + 1); showerSepIndex++)
    {
        const double binCenter = signal->GetZaxis()->GetBinCenter(showerSepIndex);

        /*
        if (showerSepIndex > showerSepMaxBin)
            showerSepMaxBin = showerSepIndex;
        */
        if (showerSepIndex < showerSepMinBin)
            showerSepMinBin = showerSepIndex;

        if ((binCenter < showerSepCut) && (showerSepIndex > showerSepCutBin))
            showerSepCutBin = showerSepIndex;
    }
    /*
    std::cout << "pandizzleMinBin: " << pandizzleMinBin << std::endl;
    std::cout << "pandizzleMaxBin: " << pandizzleMaxBin << std::endl;
    std::cout << "electronScoreMinBin: " << electronScoreMinBin << std::endl;
    std::cout << "electronScoreMaxBin: " << electronScoreMaxBin << std::endl;
    std::cout << "showerSepMinBin: " << showerSepMinBin << std::endl;
    std::cout << "showerSepMaxBin: " << showerSepMaxBin << std::endl;
    */
    TH1D *signalProjection = signal->ProjectionZ("frog", pandizzleMinBin, pandizzleCutBin, (electronScoreCutBin + 1), electronScoreMaxBin, "o");
    TH1D *backgroundProjection = background->ProjectionZ("janet", pandizzleMinBin, pandizzleCutBin, (electronScoreCutBin + 1), electronScoreMaxBin, "o");

    double selectedSignal(0);

    for (int showerSepIndex = showerSepMinBin; showerSepIndex <= showerSepCutBin; ++showerSepIndex)
        selectedSignal += signalProjection->GetBinContent(showerSepIndex);

    double selectedBackground(0);

    for (int showerSepIndex = showerSepMinBin; showerSepIndex <= showerSepCutBin; ++showerSepIndex)
        selectedBackground += backgroundProjection->GetBinContent(showerSepIndex);

    const double nueEfficiency(selectedSignal / nSignal);
    const double nuePurity(selectedSignal / (selectedSignal + selectedBackground));
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
