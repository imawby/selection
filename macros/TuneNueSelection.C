#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"

void TuneNueSelection(const std::string &inputFileName);
void DrawSelectionHistograms(NeutrinoEventVector &nuEventVector_full);
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full);
double ComputeSelectionMetric(const double nSignal, const TH1D *signal, const TH1D *background, const double pandrizzle, double &nueEfficiency, double &nuePurity);
double ComputeSelectionMetric(const double nSignal, const TH2D *signal, const TH2D *background, const double pandrizzle, const double pandizzle, double &nueEfficiency, double &nuePurity);
double ComputeSelectionMetric(const double nSignal, const TH2D *signal, const TH2D *background, const double intercept, const double gradient, double &nueEfficiency, double &nuePurity);
double GetOscWeight(const NeutrinoEvent &nu);

void TuneNueSelection(const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    //DrawSelectionHistograms(nuEventVector_full);    
    FindSelectionCuts(nuEventVector_full);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawSelectionHistograms(NeutrinoEventVector &nuEventVector_full)
{
    int pandrizzleBins = 25;
    double pandrizzleMin = -1.0, pandrizzleMax = 1.0;
    TH1D * pandrizzle_signal = new TH1D("pandrizzle_signal", "pandrizzle_signal", pandrizzleBins, pandrizzleMin, pandrizzleMax);
    TH1D * pandrizzle_background = new TH1D("pandrizzle_background", "pandrizzle_background", pandrizzleBins, pandrizzleMin, pandrizzleMax);

    int pandizzleBins = 25;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    TH1D * pandizzle_signal = new TH1D("pandizzle_signal", "pandizzle_signal", pandizzleBins, pandizzleMin, pandizzleMax);
    TH1D * pandizzle_background = new TH1D("pandizzle_background", "pandizzle_background", pandizzleBins, pandizzleMin, pandizzleMax);

    TH2D * both_signal = new TH2D("both_signal", "both_signal", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);
    TH2D * both_background = new TH2D("both_background", "both_background", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu)); <-- WRONG

        if (IsNueCCSignal(nu))
        {
            pandrizzle_signal->Fill(nu.m_selShowerPandrizzleScore, weight);
            pandizzle_signal->Fill(nu.m_selTrackPandizzleScore, weight);
            both_signal->Fill(nu.m_selShowerPandrizzleScore, nu.m_selTrackPandizzleScore, weight);
        }
        else
        {
            pandrizzle_background->Fill(nu.m_selShowerPandrizzleScore, weight);
            pandizzle_background->Fill(nu.m_selTrackPandizzleScore, weight);
            both_background->Fill(nu.m_selShowerPandrizzleScore, nu.m_selTrackPandizzleScore, weight);
        }
    }

    gStyle->SetOptStat(0);

    TCanvas * pandrizzle = new TCanvas("pandrizzle", "pandrizzle");
    pandrizzle_background->SetTitle(";Pandrizzle Score;nEvents");
    pandrizzle_signal->SetLineColor(kBlue);
    pandrizzle_background->SetLineColor(kRed);
    pandrizzle_background->Draw("hist same");
    pandrizzle_signal->Draw("hist same");

    auto pandrizzleLegend = new TLegend(0.1,0.7,0.48,0.9);
    pandrizzleLegend->AddEntry(pandrizzle_signal, "signal", "l");
    pandrizzleLegend->AddEntry(pandrizzle_background, "background", "l");
    pandrizzleLegend->Draw("same");

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

    TCanvas * both_signal_canvas = new TCanvas("signal canvas", "signal canvas");
    both_signal->SetTitle("Signal;Pandrizzle Score;Pandizzle Score;");
    both_signal->Draw("colz");

    TCanvas * both_background_canvas = new TCanvas("background canvas", "background canvas");
    both_background->SetTitle("Background;Pandrizzle Score;Pandizzle Score;");
    both_background->Draw("colz");

    TCanvas * both_ratio_canvas = new TCanvas("ratio canvas", "ratio canvas");
    TH2D * both_ratio = both_background;
    both_ratio->Divide(both_signal);
    both_ratio->GetZaxis()->SetRangeUser(0.0, 2.0);
    both_ratio->SetTitle("Background / Signal;Pandrizzle Score;Pandizzle Score;");
    both_ratio->Draw("colz");
}

//------------------------------------------------------------------------------------------------------------------------------------------
/*
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full)
{
    int interceptBins = 100;
    double interceptMin = -1.0, interceptMax = 0.0;
    double interceptStepSize = (interceptMax - interceptMin) / interceptBins;
    double interceptCuts[interceptBins];

    for (int i = 0; i < interceptBins; ++i)
        interceptCuts[i] = interceptMin + (i * interceptStepSize);

    int gradientBins = 100;
    double gradientMin = 0.0, gradientMax = 10.0;
    double gradientStepSize = (gradientMax - gradientMin) / gradientBins;
    double gradientCuts[gradientBins];

    for (int i = 0; i < gradientBins; ++i)
        gradientCuts[i] = gradientMin + (i * gradientStepSize);

    int pandrizzleBins = 100, pandizzleBins = 100;
    double pandrizzleMin = -1.0, pandrizzleMax = 1.0, pandizzleMin = -1.0, pandizzleMax = 1.0;

    TH2D * signal = new TH2D("signal", "signal", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);
    TH2D * background = new TH2D("background", "background", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);

    double nSignal(0);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        bool isRecoInFV(IsRecoInFiducialVolume(nu));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu));

        if (IsNueCCSignal(nu))
        {
            nSignal += weight;

            if (isRecoInFV)
                signal->Fill(nu.m_selShowerPandrizzleScore, nu.m_selTrackPandizzleScore, weight);
        }
        else
        {
            if (isRecoInFV)
                background->Fill(nu.m_selShowerPandrizzleScore, nu.m_selTrackPandizzleScore, weight);
        }
    }

    double bestIntercept(0.0), bestGradient(0.0);
    double bestSelectionMetric(0.0);
    unsigned int count(0);

    TH2D * nueEfficiencyHist = new TH2D("efficiency", "efficiency", interceptBins, interceptMin, interceptMax, gradientBins, gradientMin, gradientMax);
    TH2D * nuePurityHist = new TH2D("purity", "purity", interceptBins, interceptMin, interceptMax, gradientBins, gradientMin, gradientMax);
    TH2D * selectionMetricHist = new TH2D("selectionMetric", "selectionMetric", interceptBins, interceptMin, interceptMax, gradientBins, gradientMin, gradientMax);

    for (int interceptIndex = 0; interceptIndex < interceptBins; ++interceptIndex)
    {
        for (int gradientIndex = 0; gradientIndex < gradientBins; ++gradientIndex)
        {
            ++count;
            std::cout << "Iteration: " << count << "/" << interceptBins * gradientBins << std::endl;

            double nueEfficiency(0.0), nuePurity(0.0);

            const double selectionMetric(ComputeSelectionMetric(nSignal, signal, background, interceptCuts[interceptIndex], gradientCuts[gradientIndex], nueEfficiency, nuePurity));

            if (selectionMetric > bestSelectionMetric)
            {
                bestIntercept = interceptCuts[interceptIndex];
                bestGradient = gradientCuts[gradientIndex];
                bestSelectionMetric = selectionMetric;
            }

            nueEfficiencyHist->SetBinContent(nueEfficiencyHist->GetBin(interceptIndex + 1, gradientIndex + 1), nueEfficiency);
            nuePurityHist->SetBinContent(nuePurityHist->GetBin(interceptIndex + 1, gradientIndex + 1), nuePurity);
            selectionMetricHist->SetBinContent(selectionMetricHist->GetBin(interceptIndex + 1, gradientIndex + 1), selectionMetric);
        }
    }

    TCanvas * c1 = new TCanvas ("efficiency canvas", "efficiency canvas");
    nueEfficiencyHist->SetTitle("Efficiency;Intercept Score;Gradient Score");
    nueEfficiencyHist->Draw("colz");

    TCanvas * c2 = new TCanvas ("purity canvas", "purity canvas");
    nuePurityHist->SetTitle("Purity;Intercept Score;Gradient Score");
    nuePurityHist->Draw("colz");

    TCanvas * c3 = new TCanvas ("selection metric canvas", "selection metric canvas");
    selectionMetricHist->SetTitle("SelectionMetric;Intercept Score;Gradient Score");
    selectionMetricHist->Draw("colz");

    std::cout << "Best selection metric: " << bestSelectionMetric << " obtained with: " << std::endl;
    std::cout << "Intercept cut: " << bestIntercept << std::endl;
    std::cout << "Gradient cut: " << bestGradient << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const double nSignal, const TH2D *signal, const TH2D *background, const double intercept, const double gradient, 
    double &nueEfficiency, double &nuePurity)
{
    double pandrizzleMin(-std::numeric_limits<int>::max()), pandrizzleMax(std::numeric_limits<int>::max());
    int pandrizzleMinBin(std::numeric_limits<int>::max()), pandrizzleMaxBin(-std::numeric_limits<int>::max());

    for (int pandrizzleIndex = 0; pandrizzleIndex <= (signal->GetNbinsX() + 1); pandrizzleIndex++)
    {
        const double binCenter = signal->GetXaxis()->GetBinCenter(pandrizzleIndex);

        if (pandrizzleIndex > pandrizzleMaxBin)
            pandrizzleMaxBin = pandrizzleIndex;

        if (pandrizzleIndex < pandrizzleMinBin)
            pandrizzleMinBin = pandrizzleIndex;
    }

    double pandizzleMin(-std::numeric_limits<int>::max()), pandizzleMax(std::numeric_limits<int>::max());
    int pandizzleMinBin(std::numeric_limits<int>::max()), pandizzleMaxBin(-std::numeric_limits<int>::max());

    for (int pandizzleIndex = 0; pandizzleIndex <= (signal->GetNbinsY() + 1); pandizzleIndex++)
    {
        const double binCenter = signal->GetXaxis()->GetBinCenter(pandizzleIndex);

        if (pandizzleIndex > pandizzleMaxBin)
            pandizzleMaxBin = pandizzleIndex;

        if (pandizzleIndex < pandizzleMinBin)
            pandizzleMinBin = pandizzleIndex;
    }

    const double cutPandrizzleMin((0.0 - intercept) / gradient);
    const double cutPandizzleMax((gradient * 1.0) + intercept);

    TCutG * cut = new TCutG("cut", 3);
    cut->SetPoint(0, cutPandrizzleMin, 0.0);
    cut->SetPoint(1, 1.0, 0.0);
    cut->SetPoint(2, 1.0, cutPandizzleMax);

    TH1D *signalProjection = signal->ProjectionX("frog", pandizzleMinBin, pandizzleMaxBin, "[cut]");
    TH1D *backgroundProjection = background->ProjectionX("janet", pandizzleMinBin, pandizzleMaxBin, "[cut]");

    double selectedSignal(0);

    for (int pandrizzleIndex = pandrizzleMinBin; pandrizzleIndex <= pandrizzleMaxBin; ++pandrizzleIndex)
        selectedSignal += signalProjection->GetBinContent(pandrizzleIndex);

    double selectedBackground(0);

    for (int pandrizzleIndex = pandrizzleMinBin; pandrizzleIndex <= pandrizzleMaxBin; ++pandrizzleIndex)
        selectedBackground += backgroundProjection->GetBinContent(pandrizzleIndex);

    double totalSelected = selectedSignal + selectedBackground;
    nueEfficiency = selectedSignal / nSignal;
    nuePurity = (totalSelected < std::numeric_limits<double>::epsilon()) ? 0.0 : selectedSignal / (selectedSignal + selectedBackground);

    const double selectionMetric(nueEfficiency * nuePurity);

    return selectionMetric;
}
*/
//------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------

void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full)
{
    int pandrizzleBins = 100;
    double pandrizzleMin = -1.0, pandrizzleMax = 1.0;
    double pandrizzleStepSize = (pandrizzleMax - pandrizzleMin) / pandrizzleBins;
    double pandrizzleCuts[pandrizzleBins];

    for (int i = 0; i < pandrizzleBins; ++i)
        pandrizzleCuts[i] = pandrizzleMin + (i * pandrizzleStepSize);

    int pandizzleBins = 100;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    double pandizzleStepSize = (pandizzleMax - pandizzleMin) / pandizzleBins;
    double pandizzleCuts[pandizzleBins];

    for (int i = 0; i < pandizzleBins; ++i)
        pandizzleCuts[i] = pandizzleMin + (i * pandizzleStepSize);

    TH2D * signal = new TH2D("signal", "signal", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);
    TH2D * background = new TH2D("background", "background", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);

    double nSignal(0);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        bool isRecoInFV(IsRecoInFiducialVolume(nu));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu));

        if (IsNueCCSignal(nu))
        {
            nSignal += weight;

            if (isRecoInFV)
                signal->Fill(nu.m_selShowerPandrizzleScore, nu.m_selTrackPandizzleScore, weight);
        }
        else
        {
            if (isRecoInFV)
                background->Fill(nu.m_selShowerPandrizzleScore, nu.m_selTrackPandizzleScore, weight);
        }
    }

    double bestPandrizzle(0.0), bestPandizzle(0.0);
    double bestSelectionMetric(0.0);
    unsigned int count(0);

    TH2D * nueEfficiencyHist = new TH2D("efficiency", "efficiency", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);
    TH2D * nuePurityHist = new TH2D("purity", "purity", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);
    TH2D * selectionMetricHist = new TH2D("selectionMetric", "selectionMetric", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);

    for (int pandrizzleIndex = 0; pandrizzleIndex < pandrizzleBins; ++pandrizzleIndex)
    {
        for (int pandizzleIndex = 0; pandizzleIndex < pandizzleBins; ++pandizzleIndex)
        {
            ++count;
            std::cout << "Iteration: " << count << "/" << pandrizzleBins * pandizzleBins << std::endl;

            double nueEfficiency(0.0), nuePurity(0.0);

            const double selectionMetric(ComputeSelectionMetric(nSignal, signal, background, pandrizzleCuts[pandrizzleIndex], pandizzleCuts[pandizzleIndex], nueEfficiency, nuePurity));

            if (selectionMetric > bestSelectionMetric)
            {
                bestPandrizzle = pandrizzleCuts[pandrizzleIndex];
                bestPandizzle = pandizzleCuts[pandizzleIndex];
                bestSelectionMetric = selectionMetric;
            }

            nueEfficiencyHist->SetBinContent(nueEfficiencyHist->GetBin(pandrizzleIndex + 1, pandizzleIndex + 1), nueEfficiency);
            nuePurityHist->SetBinContent(nuePurityHist->GetBin(pandrizzleIndex + 1, pandizzleIndex + 1), nuePurity);
            selectionMetricHist->SetBinContent(selectionMetricHist->GetBin(pandrizzleIndex + 1, pandizzleIndex + 1), selectionMetric);
        }
    }

    TCanvas * c1 = new TCanvas ("efficiency canvas", "efficiency canvas");
    nueEfficiencyHist->SetTitle("Efficiency;Pandrizzle Score;Pandizzle Score");
    nueEfficiencyHist->Draw("colz");

    TCanvas * c2 = new TCanvas ("purity canvas", "purity canvas");
    nuePurityHist->SetTitle("Purity;Pandrizzle Score;Pandizzle Score");
    nuePurityHist->Draw("colz");

    TCanvas * c3 = new TCanvas ("selection metric canvas", "selection metric canvas");
    selectionMetricHist->SetTitle("SelectionMetric;Pandrizzle Score;Pandizzle Score");
    selectionMetricHist->Draw("colz");

    std::cout << "Best selection metric: " << bestSelectionMetric << " obtained with: " << std::endl;
    std::cout << "Pandrizzle cut: " << bestPandrizzle << std::endl;
    std::cout << "Pandizzle cut: " << bestPandizzle << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const double nSignal, const TH2D *signal, const TH2D *background, const double pandrizzle, const double pandizzle, 
    double &nueEfficiency, double &nuePurity)
{
    double pandrizzleMin(-std::numeric_limits<int>::max()), pandrizzleCut(pandrizzle), pandrizzleMax(std::numeric_limits<int>::max());
    int pandrizzleMinBin(std::numeric_limits<int>::max()), pandrizzleCutBin(-std::numeric_limits<int>::max()), pandrizzleMaxBin(-std::numeric_limits<int>::max());

    for (int pandrizzleIndex = 0; pandrizzleIndex <= (signal->GetNbinsX() + 1); pandrizzleIndex++)
    {
        const double binCenter = signal->GetXaxis()->GetBinCenter(pandrizzleIndex);

        if (pandrizzleIndex > pandrizzleMaxBin)
            pandrizzleMaxBin = pandrizzleIndex;

        if (pandrizzleIndex < pandrizzleMinBin)
            pandrizzleMinBin = pandrizzleIndex;

        if ((binCenter < pandrizzleCut) && (pandrizzleIndex > pandrizzleCutBin))
            pandrizzleCutBin = pandrizzleIndex;
    }

    double pandizzleMin(-std::numeric_limits<int>::max()), pandizzleCut(pandizzle), pandizzleMax(std::numeric_limits<int>::max());
    int pandizzleMinBin(std::numeric_limits<int>::max()), pandizzleCutBin(-std::numeric_limits<int>::max()), pandizzleMaxBin(-std::numeric_limits<int>::max());

    for (int pandizzleIndex = 0; pandizzleIndex <= (signal->GetNbinsY() + 1); pandizzleIndex++)
    {
        const double binCenter = signal->GetXaxis()->GetBinCenter(pandizzleIndex);

        if (pandizzleIndex > pandizzleMaxBin)
            pandizzleMaxBin = pandizzleIndex;

        if (pandizzleIndex < pandizzleMinBin)
            pandizzleMinBin = pandizzleIndex;

        if ((binCenter < pandizzleCut) && (pandizzleIndex > pandizzleCutBin))
            pandizzleCutBin = pandizzleIndex;
    }

    TH1D *signalProjection = signal->ProjectionX("frog", pandizzleMinBin, pandizzleCutBin, "o");
    TH1D *backgroundProjection = background->ProjectionX("janet", pandizzleMinBin, pandizzleCutBin, "o");

    double selectedSignal(0);

    for (int pandrizzleIndex = (pandrizzleCutBin + 1); pandrizzleIndex <= pandrizzleMaxBin; ++pandrizzleIndex)
        selectedSignal += signalProjection->GetBinContent(pandrizzleIndex);

    double selectedBackground(0);

    for (int pandrizzleIndex = (pandrizzleCutBin + 1); pandrizzleIndex <= pandrizzleMaxBin; ++pandrizzleIndex)
        selectedBackground += backgroundProjection->GetBinContent(pandrizzleIndex);

    double totalSelected = selectedSignal + selectedBackground;
    nueEfficiency = selectedSignal / nSignal;
    nuePurity = (totalSelected < std::numeric_limits<double>::epsilon()) ? 0.0 : selectedSignal / (selectedSignal + selectedBackground);

    const double selectionMetric(nueEfficiency * nuePurity);

    return selectionMetric;
}

//------------------------------------------------------------------------------------------------------------------------------------------

/*
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full)
{
    int pandrizzleBins = 10000;
    double pandrizzleMin = -1.0, pandrizzleMax = 1.0;
    double pandrizzleStepSize = (pandrizzleMax - pandrizzleMin) / pandrizzleBins;
    double pandrizzleCuts[pandrizzleBins];

    for (int i = 0; i < pandrizzleBins; ++i)
        pandrizzleCuts[i] = pandrizzleMin + (i * pandrizzleStepSize);

    TH1D * signal = new TH1D("signal", "signal", pandrizzleBins, pandrizzleMin, pandrizzleMax);
    TH1D * background = new TH1D("background", "background", pandrizzleBins, pandrizzleMin, pandrizzleMax);

    double nSignal(0);

    // Select
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        bool isRecoInFV(IsRecoInFiducialVolume(nu));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        //const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu));

        if (IsNueCCSignal(nu))
        {
            nSignal += weight;

            if (isRecoInFV)
                signal->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else
        {
            if (isRecoInFV)
                background->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
    }

    double bestPandrizzle(0.0);
    double bestSelectionMetric(0.0);
    unsigned int count(0);

    double nueEfficiency[pandrizzleBins], nuePurity[pandrizzleBins], selectionMetric[pandrizzleBins];

    for (int pandrizzleIndex = 0; pandrizzleIndex < pandrizzleBins; ++pandrizzleIndex)
    {
        ++count;
        std::cout << "Iteration: " << count << "/" << pandrizzleBins << std::endl;

        selectionMetric[pandrizzleIndex] = (ComputeSelectionMetric(nSignal, signal, background, pandrizzleCuts[pandrizzleIndex], nueEfficiency[pandrizzleIndex], nuePurity[pandrizzleIndex]));

        if (selectionMetric[pandrizzleIndex] > bestSelectionMetric)
        {
            bestPandrizzle = pandrizzleCuts[pandrizzleIndex];
            bestSelectionMetric = selectionMetric[pandrizzleIndex];
        }
    }


    TGraph * efficiencyGraph = new TGraph(pandrizzleBins, pandrizzleCuts, nueEfficiency);
    TGraph * purityGraph = new TGraph(pandrizzleBins, pandrizzleCuts, nuePurity);
    TGraph * selectionMetricGraph = new TGraph(pandrizzleBins, pandrizzleCuts, selectionMetric);

    TCanvas * c1 = new TCanvas ("c1", "c1");
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
    std::cout << "Pandrizzle cut: " << bestPandrizzle << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const double nSignal, const TH1D *signal, const TH1D *background, const double pandrizzle, double &nueEfficiency, double &nuePurity)
{
    double pandrizzleMin(-std::numeric_limits<int>::max()), pandrizzleCut(pandrizzle), pandrizzleMax(std::numeric_limits<int>::max());
    int pandrizzleMinBin(std::numeric_limits<int>::max()), pandrizzleCutBin(-std::numeric_limits<int>::max()), pandrizzleMaxBin(-std::numeric_limits<int>::max());

    for (int pandrizzleIndex = 0; pandrizzleIndex <= (signal->GetNbinsX() + 1); pandrizzleIndex++)
    {
        const double binCenter = signal->GetXaxis()->GetBinCenter(pandrizzleIndex);

        if (pandrizzleIndex > pandrizzleMaxBin)
            pandrizzleMaxBin = pandrizzleIndex;

        if (pandrizzleIndex < pandrizzleMinBin)
            pandrizzleMinBin = pandrizzleIndex;

        if ((binCenter < pandrizzleCut) && (pandrizzleIndex > pandrizzleCutBin))
            pandrizzleCutBin = pandrizzleIndex;
    }

    double selectedSignal(0);

    for (int pandrizzleIndex = (pandrizzleCutBin + 1); pandrizzleIndex <= pandrizzleMaxBin; ++pandrizzleIndex)
        selectedSignal += signal->GetBinContent(pandrizzleIndex);

    double selectedBackground(0);

    for (int pandrizzleIndex = (pandrizzleCutBin + 1); pandrizzleIndex <= pandrizzleMaxBin; ++pandrizzleIndex)
        selectedBackground += background->GetBinContent(pandrizzleIndex);

    double totalSelected = selectedSignal + selectedBackground;
    nueEfficiency = selectedSignal / nSignal;
    nuePurity = (totalSelected < std::numeric_limits<double>::epsilon()) ? 0.0 : selectedSignal / (selectedSignal + selectedBackground);

    const double selectionMetric(nueEfficiency * nuePurity);

    return selectionMetric;
}
*/
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
