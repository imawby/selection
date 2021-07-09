#include "NeutrinoEvent.C"
#include "Sensitivity.C"
#include "Selection.h"

void TuneSelectionDeltaCP(const std::string &inputFileName);
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full);
double ComputeSelectionMetric(const TH3D * nue_zero, const TH3D * nue_pi, const std::vector<TH3D*> nue_deltaCP, const double nuePandizzle, const double pandrizzle, 
    const TH3D * numu_zero, const TH3D * numu_pi, const std::vector<TH3D*> numu_deltaCP);
double ComputeSelectionMetric(const TH3D * nue_zero, const TH3D * nue_pi, const std::vector<TH3D*> nue_deltaCP, const double nuePandizzle, const double pandrizzle, 
    const TH3D * numu_zero, const TH3D * numu_pi, const std::vector<TH3D*> numu_deltaCP, const double numuPandizzle);

const bool IS_NEUTRINO = true;
const float SIGMA = 8.0;


void TuneSelectionDeltaCP(const std::string &inputFileName_full)
{
    std::cout << "ONLY ADDING NUE SENSITIVITY" << std::endl;

    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    FindSelectionCuts(nuEventVector_full);
}

//------------------------------------------------------------------------------------------------------------------------------------------

// Tune on both nue selection (keep numu because the nue selection limits events that can be selected in numu)
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full)
{
    TFile * file = new TFile("DeltaCPSelectionMetric.root", "RECREATE");

    // Set up variable ranges (the plus 1 is so that i look at the 'accept nothing and accept everything' cases
    int pandizzleBins = 50;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    //double pandizzleMin = 0.2, pandizzleMax = 1.0; //FHC
    //double pandizzleMin = -0.1, pandizzleMax = 1.0; //RHC
    double pandizzleStepSize = (pandizzleMax - pandizzleMin) / pandizzleBins;
    double pandizzleCuts[pandizzleBins + 1];

    for (int i = 0; i < pandizzleBins; ++i)
        pandizzleCuts[i] = pandizzleMin + (i * pandizzleStepSize);

    int pandrizzleBins = 50;
    double pandrizzleMin = -1.0, pandrizzleMax = 1.0;
    //double pandrizzleMin = 0.0, pandrizzleMax = 0.5; //FHC
    //double pandrizzleMin = 0.3, pandrizzleMax = 0.8; //RHC
    double pandrizzleStepSize = (pandrizzleMax - pandrizzleMin) / pandrizzleBins;
    double pandrizzleCuts[pandrizzleBins + 1];

    for (int i = 0; i < pandrizzleBins; ++i)
        pandrizzleCuts[i] = pandrizzleMin + (i * pandrizzleStepSize);

    int nDeltaCPValues = 200;
    double minDeltaCP = (-1.0)*TMath::Pi(), maxDeltaCP = TMath::Pi();
    double deltaCPStepSize = (maxDeltaCP - minDeltaCP) / nDeltaCPValues;
    double deltaCPValues[nDeltaCPValues];

    for (int i = 0; i < nDeltaCPValues; ++i)
        deltaCPValues[i] = minDeltaCP + (i * deltaCPStepSize);

    int nEnergyBins = 40;
    double minEnergy = 0.0, maxEnergy = 10.0;

    // Define cut histograms
    TH3D * nue_zero = new TH3D("nue_zero", "nue_zero", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    TH3D * nue_pi = new TH3D("nue_pi", "nue_pi", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    TH3D * numu_zero = new TH3D("numu_zero", "numu_zero", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    TH3D * numu_pi = new TH3D("numu_pi", "numu_pi", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    std::vector<TH3D*> nue_deltaCP, numu_deltaCP;

    for (int i = 0; i < nDeltaCPValues; ++i)
    {
        TH3D * frog =  new TH3D(("nue_" + to_string(i)).c_str(), ("nue_" + to_string(i)).c_str(), pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, 
                                pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);

        TH3D * doggo =  new TH3D(("numu_" + to_string(i)).c_str(), ("numu_" + to_string(i)).c_str(), pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, 
                                pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);

        nue_deltaCP.push_back(frog);
        numu_deltaCP.push_back(doggo);
    }

    // Fill cut histograms
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        const bool isRecoInFV(IsRecoInFiducialVolume(nu));

        if (!isRecoInFV)
            continue;

        // remove events that are actually photons.. 
        if(nu.m_selShowerTruePdg == 22)
            continue;

        const double weight_zero(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, 0.0)));
        nue_zero->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_nueRecoENu, weight_zero);
        numu_zero->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_numuRecoENu, weight_zero);

        const double weight_pi(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, TMath::Pi())));
        nue_pi->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_nueRecoENu, weight_pi);
        numu_pi->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_numuRecoENu, weight_pi);

        for (int i = 0; i < nDeltaCPValues; ++i)
        {
            const double weight_deltaCP(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCPValues[i])));
            nue_deltaCP[i]->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_nueRecoENu, weight_deltaCP);
            numu_deltaCP[i]->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_numuRecoENu, weight_deltaCP);
        }
    }

    double bestNuePandizzle(0.0), bestNuePandrizzle(0.0);
    double bestSelectionMetric(0.0);
    unsigned int count(0);

    TH2D * selectionMetricHist = new TH2D("selectionMetric", "selectionMetric", pandrizzleBins, pandrizzleMin, pandrizzleMax, pandizzleBins, pandizzleMin, pandizzleMax);

    // the equals is so i look at the 'accept nothing and accept everything' cases
    for (int nuePandizzleIndex = 0; nuePandizzleIndex <= pandizzleBins; ++nuePandizzleIndex)
    {
        for (int nuePandrizzleIndex = 0; nuePandrizzleIndex <= pandrizzleBins; ++nuePandrizzleIndex)
        {
            ++count;
            std::cout << "Iteration: " << count << "/" << pandrizzleBins * pandizzleBins << std::endl;

            const double selectionMetric(ComputeSelectionMetric(nue_zero, nue_pi, nue_deltaCP, pandizzleCuts[nuePandizzleIndex], pandrizzleCuts[nuePandrizzleIndex],
                numu_zero, numu_pi, numu_deltaCP));

            if (selectionMetric > bestSelectionMetric)
            {
                bestNuePandizzle = pandizzleCuts[nuePandizzleIndex];
                bestNuePandrizzle = pandrizzleCuts[nuePandrizzleIndex];
                bestSelectionMetric = selectionMetric;
            }

            // the bin corresponds to the bottom enclosed square of lines drawn at cuts
            if ((nuePandrizzleIndex > 0) && (nuePandizzleIndex > 0))
                selectionMetricHist->SetBinContent(selectionMetricHist->GetBin(nuePandrizzleIndex, nuePandizzleIndex), selectionMetric);
        }
    }

    selectionMetricHist->SetTitle("SelectionMetric;Pandrizzle Score;Pandizzle Score");
    selectionMetricHist->Write("selectionMetricHist");

    file->Close();
    
    std::cout << "Best selection metric: " << bestSelectionMetric << " obtained with: " << std::endl;
    std::cout << "Nue pandizzle cut: " << bestNuePandizzle << std::endl;
    std::cout << "Pandrizzle cut: " << bestNuePandrizzle << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const TH3D * nue_zero, const TH3D * nue_pi, const std::vector<TH3D*> nue_deltaCP, const double nuePandizzle, const double pandrizzle, 
    const TH3D * numu_zero, const TH3D * numu_pi, const std::vector<TH3D*> numu_deltaCP)
{
    // need to have underflow bins included because even if a track isnt reconstructed a shower can be FOOL
    double nuePandizzleCut(nuePandizzle);
    int nuePandizzleMinBin(0), nuePandizzleCutBin(-std::numeric_limits<int>::max()), nuePandizzleMaxBin(nue_zero->GetXaxis()->GetNbins() + 1);

    for (int nuePandizzleIndex = 1; nuePandizzleIndex < (nue_zero->GetNbinsX() + 1); nuePandizzleIndex++)
    {
        const double binCenter = nue_zero->GetXaxis()->GetBinCenter(nuePandizzleIndex);

        if ((binCenter < nuePandizzleCut) && (nuePandizzleIndex > nuePandizzleCutBin))
            nuePandizzleCutBin = nuePandizzleIndex;
    }

    // this doesnt work...
    if (nuePandizzleCutBin < 0)
    {
        std::cout << "1 couldn't find cut bin" << std::endl;
        return 0;
    }

    /*
    double numuPandizzleCut(IS_NEUTRINO ? NUMU_PANDIZZLE_CUT : ANUMU_PANDIZZLE_CUT);
    int numuPandizzleMinBin(0), numuPandizzleCutBin(-std::numeric_limits<int>::max()), numuPandizzleMaxBin(numu_zero->GetXaxis()->GetNbins() + 1);

    for (int numuPandizzleIndex = 1; numuPandizzleIndex < (numu_zero->GetNbinsX() + 1); numuPandizzleIndex++)
    {
        const double binCenter = numu_zero->GetXaxis()->GetBinCenter(numuPandizzleIndex);

        if ((binCenter < numuPandizzleCut) && (numuPandizzleIndex > numuPandizzleCutBin))
            numuPandizzleCutBin = numuPandizzleIndex;
    }

    if (numuPandizzleCutBin < 0)
    {
        std::cout << "2 couldn't find cut bin" << std::endl;
        return 0;
    }
    */
    double pandrizzleCut(pandrizzle);
    int pandrizzleMinBin(0), pandrizzleCutBin(-std::numeric_limits<int>::max()), pandrizzleMaxBin(nue_zero->GetYaxis()->GetNbins() + 1);

    for (int pandrizzleIndex = 1; pandrizzleIndex < (nue_zero->GetNbinsY() + 1); pandrizzleIndex++)
    {
        const double binCenter = nue_zero->GetYaxis()->GetBinCenter(pandrizzleIndex);

        if ((binCenter < pandrizzleCut) && (pandrizzleIndex > pandrizzleCutBin))
            pandrizzleCutBin = pandrizzleIndex;
    }

    if (pandrizzleCutBin < 0)
    {
        std::cout << "3 couldn't find cut bin" << std::endl;
        return 0;
    }
    
    TH1D *nueProjection_zero = nue_zero->ProjectionZ("frog", nuePandizzleMinBin, nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");
    TH1D *nueProjection_pi = nue_pi->ProjectionZ("jam", nuePandizzleMinBin, nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");

    /*
    TH1D *numuProjection_zero = numu_zero->ProjectionZ("bug", (numuPandizzleCutBin + 1), numuPandizzleMaxBin, pandrizzleMinBin, pandrizzleMaxBin, "o");
    TH1D *numuProjection_pi = numu_pi->ProjectionZ("janet", (numuPandizzleCutBin + 1), numuPandizzleMaxBin, pandrizzleMinBin, pandrizzleMaxBin, "o");

    // remove nue selected from numu hist 
    if (numuPandizzleCut < nuePandizzle)
    {
        TH1D * bothProjection_zero = numu_zero->ProjectionZ("cactus", (numuPandizzleCutBin + 1), nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");
        TH1D * bothProjection_pi = numu_pi->ProjectionZ("bennie", (numuPandizzleCutBin + 1), nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");

        numuProjection_zero->Add(bothProjection_zero, -1.0);
        numuProjection_pi->Add(bothProjection_pi, -1.0);
    }
    */
    double totalCoveredRange = 0.0;
    double previousSigTotal = -std::numeric_limits<double>::max();
    const double deltaCPStepSize = 1.0 / nue_deltaCP.size();

    for (int i = 0; i < nue_deltaCP.size(); ++i)
    {
        TH1D *nueProjection_deltaCP = nue_deltaCP[i]->ProjectionZ(("sock" + to_string(i)).c_str(), nuePandizzleMinBin, nuePandizzleCutBin, 
                                                                  (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");

        /*
        TH1D *numuProjection_deltaCP = numu_deltaCP[i]->ProjectionZ(("kangeroo" + to_string(i)).c_str(), (numuPandizzleCutBin + 1), numuPandizzleMaxBin, 
                                                                    pandrizzleMinBin, pandrizzleMaxBin, "o");

        // remove nue selected from numu hist 
        if (numuPandizzleCut < nuePandizzle)
        {
            TH1D * bothProjection_deltaCP = numu_deltaCP[i]->ProjectionZ(("fly" + to_string(i)).c_str(), (numuPandizzleCutBin + 1), nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");
            numuProjection_deltaCP->Add(bothProjection_deltaCP, -1.0);
        }
        */

        const double nueLogLikelihood(CalculateMinLogLikelihood(nueProjection_zero, nueProjection_pi, nueProjection_deltaCP));
        //const double numuLogLikelihood(CalculateMinLogLikelihood(numuProjection_zero, numuProjection_pi, numuProjection_deltaCP));
        //const double logLikelihoodSum(nueLogLikelihood + numuLogLikelihood);
        const double logLikelihoodSum(nueLogLikelihood);

        if (logLikelihoodSum < 0.0)
        {
            //std::cout << "nueLogLikelihood: " << nueLogLikelihood << ", numuLogLikelihood: " << numuLogLikelihood << std::endl;
            std::cout << "ISOBEL YOU HAVE A NEGATIVE LOGLIKELIHOOD SUM" << std::endl;
            throw;
        }

        const double sigTotal = std::sqrt(2.0 * logLikelihoodSum);

        if ((previousSigTotal > SIGMA) && (sigTotal > SIGMA))
            totalCoveredRange += deltaCPStepSize;

        previousSigTotal = sigTotal;
    }

    return totalCoveredRange;
}

//------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------------

/*
// Tune on both nue selection and numu selection simultaneously 
void FindSelectionCuts(const NeutrinoEventVector &nuEventVector_full)
{
    // Set up variable ranges
    int pandizzleBins = 200;
    double pandizzleMin = -1.0, pandizzleMax = 1.0;
    double pandizzleStepSize = (pandizzleMax - pandizzleMin) / pandizzleBins;
    double pandizzleCuts[pandizzleBins];

    for (int i = 1; i < pandizzleBins; ++i)
        pandizzleCuts[i] = pandizzleMin + (i * pandizzleStepSize);

    int pandrizzleBins = 200;
    double pandrizzleMin = -1.0, pandrizzleMax = 1.0;
    double pandrizzleStepSize = (pandrizzleMax - pandrizzleMin) / pandrizzleBins;
    double pandrizzleCuts[pandrizzleBins];

    for (int i = 1; i < pandrizzleBins; ++i)
        pandrizzleCuts[i] = pandrizzleMin + (i * pandrizzleStepSize);

    int nDeltaCPValues = 50;
    double minDeltaCP = (-1.0)*TMath::Pi(), maxDeltaCP = TMath::Pi();
    double deltaCPStepSize = (maxDeltaCP - minDeltaCP) / nDeltaCPValues;
    double deltaCPValues[nDeltaCPValues];

    for (int i = 0; i < nDeltaCPValues; ++i)
        deltaCPValues[i] = minDeltaCP + (i * deltaCPStepSize);

    int nEnergyBins = 40;
    double minEnergy = 0.0, maxEnergy = 10.0;

    // Define cut histograms
    TH3D * nue_zero = new TH3D("nue_zero", "nue_zero", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    TH3D * nue_pi = new TH3D("nue_pi", "nue_pi", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    TH3D * numu_zero = new TH3D("numu_zero", "numu_zero", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    TH3D * numu_pi = new TH3D("numu_pi", "numu_pi", pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);
    std::vector<TH3D*> nue_deltaCP, numu_deltaCP;

    for (int i = 0; i < nDeltaCPValues; ++i)
    {
        TH3D * frog =  new TH3D(("nue_" + to_string(i)).c_str(), ("nue_" + to_string(i)).c_str(), pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, 
                                pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);

        TH3D * doggo =  new TH3D(("numu_" + to_string(i)).c_str(), ("numu_" + to_string(i)).c_str(), pandizzleBins, pandizzleMin, pandizzleMax, pandrizzleBins, pandrizzleMin, 
                                pandrizzleMax, nEnergyBins, minEnergy, maxEnergy);

        nue_deltaCP.push_back(frog);
        numu_deltaCP.push_back(doggo);
    }

    // Fill cut histograms
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        const bool isRecoInFV(IsRecoInFiducialVolume(nu));

        if (!isRecoInFV)
            continue;

        const double weight_zero(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, 0.0)));
        nue_zero->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_nueRecoENu, weight_zero);
        numu_zero->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_numuRecoENu, weight_zero);

        const double weight_pi(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, TMath::Pi())));
        nue_pi->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_nueRecoENu, weight_pi);
        numu_pi->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_numuRecoENu, weight_pi);

        for (int i = 0; i < nDeltaCPValues; ++i)
        {
            const double weight_deltaCP(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCPValues[i])));
            nue_deltaCP[i]->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_nueRecoENu, weight_deltaCP);
            numu_deltaCP[i]->Fill(nu.m_selTrackPandizzleScore, nu.m_selShowerPandrizzleScore, nu.m_numuRecoENu, weight_deltaCP);
        }
    }

    double bestNuePandizzle(0.0), bestNumuPandizzle(0.0), bestPandrizzle(0.0);
    double bestSelectionMetric(0.0);
    unsigned int count(0);

    for (int nuePandizzleIndex = 0; nuePandizzleIndex < pandizzleBins; ++nuePandizzleIndex)
    {
        for (int pandrizzleIndex = 0; pandrizzleIndex < pandrizzleBins; ++pandrizzleIndex)
        {
            for (int numuPandizzleIndex = 0; numuPandizzleIndex < pandizzleBins; ++numuPandizzleIndex)
            {
                ++count;
                std::cout << "Iteration: " << count << "/" << pandrizzleBins * pandizzleBins << std::endl;

                const double selectionMetric(ComputeSelectionMetric(nue_zero, nue_pi, nue_deltaCP, pandizzleCuts[nuePandizzleIndex], pandrizzleCuts[pandrizzleIndex],
                                                                    numu_zero, numu_pi, numu_deltaCP, pandizzleCuts[numuPandizzleIndex]));

                if (selectionMetric > bestSelectionMetric)
                {
                    bestNuePandizzle = pandizzleCuts[nuePandizzleIndex];
                    bestNumuPandizzle = pandizzleCuts[numuPandizzleIndex];
                    bestPandrizzle = pandrizzleCuts[pandrizzleIndex];
                    bestSelectionMetric = selectionMetric;
                }
            }
        }
    }
    
    std::cout << "Best selection metric: " << bestSelectionMetric << " obtained with: " << std::endl;
    std::cout << "Nue pandizzle cut: " << bestNuePandizzle << std::endl;
    std::cout << "Pandrizzle cut: " << bestPandrizzle << std::endl;
    std::cout << "Numu pandizzle cut: " << bestNumuPandizzle << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double ComputeSelectionMetric(const TH3D * nue_zero, const TH3D * nue_pi, const std::vector<TH3D*> nue_deltaCP, const double nuePandizzle, const double pandrizzle, 
                              const TH3D * numu_zero, const TH3D * numu_pi, const std::vector<TH3D*> numu_deltaCP, const double numuPandizzle)
{
    // need to have underflow bins included because even if a track isnt reconstructed a shower can be FOOL
    double nuePandizzleCut(nuePandizzle);
    int nuePandizzleMinBin(0), nuePandizzleCutBin(-std::numeric_limits<int>::max()), nuePandizzleMaxBin(nue_zero->GetXaxis()->GetNbins());

    for (int nuePandizzleIndex = 1; nuePandizzleIndex < (nue_zero->GetNbinsX() + 1); nuePandizzleIndex++)
    {
        const double binCenter = nue_zero->GetXaxis()->GetBinCenter(nuePandizzleIndex);

        if ((binCenter < nuePandizzleCut) && (nuePandizzleIndex > nuePandizzleCutBin))
            nuePandizzleCutBin = nuePandizzleIndex;
    }

    if (nuePandizzleCutBin < 0)
    {
        std::cout << "1 couldn't find cut bin" << std::endl;
        throw;
    }

    double numuPandizzleCut(numuPandizzle);
    int numuPandizzleMinBin(0), numuPandizzleCutBin(-std::numeric_limits<int>::max()), numuPandizzleMaxBin(numu_zero->GetXaxis()->GetNbins());

    for (int numuPandizzleIndex = 1; numuPandizzleIndex < (numu_zero->GetNbinsX() + 1); numuPandizzleIndex++)
    {
        const double binCenter = numu_zero->GetXaxis()->GetBinCenter(numuPandizzleIndex);

        if ((binCenter < numuPandizzleCut) && (numuPandizzleIndex > numuPandizzleCutBin))
            numuPandizzleCutBin = numuPandizzleIndex;
    }

    if (numuPandizzleCutBin < 0)
    {
        std::cout << "2 couldn't find cut bin" << std::endl;
        throw;
    }

    double pandrizzleCut(pandrizzle);
    int pandrizzleMinBin(0), pandrizzleCutBin(-std::numeric_limits<int>::max()), pandrizzleMaxBin(nue_zero->GetYaxis()->GetNbins());

    for (int pandrizzleIndex = 1; pandrizzleIndex < (nue_zero->GetNbinsY() + 1); pandrizzleIndex++)
    {
        const double binCenter = nue_zero->GetYaxis()->GetBinCenter(pandrizzleIndex);

        if ((binCenter < pandrizzleCut) && (pandrizzleIndex > pandrizzleCutBin))
            pandrizzleCutBin = pandrizzleIndex;
    }

    if (pandrizzleCutBin < 0)
    {
        std::cout << "3 couldn't find cut bin" << std::endl;
        throw;
    }

    TH1D *nueProjection_zero = nue_zero->ProjectionZ("frog", nuePandizzleMinBin, nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");
    TH1D *nueProjection_pi = nue_pi->ProjectionZ("jam", nuePandizzleMinBin, nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");

    TH1D *numuProjection_zero = numu_zero->ProjectionZ("bug", (numuPandizzleCutBin + 1), numuPandizzleMaxBin, pandrizzleMinBin, pandrizzleMaxBin, "o");
    TH1D *numuProjection_pi = numu_pi->ProjectionZ("janet", (numuPandizzleCutBin + 1), numuPandizzleMaxBin, pandrizzleMinBin, pandrizzleMaxBin, "o");

    // remove nue selected from numu hist 
    if (numuPandizzleCut < nuePandizzle)
    {
        TH1D * bothProjection_zero = numu_zero->ProjectionZ("cactus", (numuPandizzleCutBin + 1), nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");
        TH1D * bothProjection_pi = numu_pi->ProjectionZ("bennie", (numuPandizzleCutBin + 1), nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");

        numuProjection_zero->Add(bothProjection_zero, -1.0);
        numuProjection_pi->Add(bothProjection_pi, -1.0);
    }

    double totalCoveredRange = 0.0;
    double previousSigTotal = -std::numeric_limits<double>::max();
    const double deltaCPStepSize = 1.0 / nue_deltaCP.size();

    for (int i = 0; i < nue_deltaCP.size(); ++i)
    {
        TH1D *nueProjection_deltaCP = nue_deltaCP[i]->ProjectionZ(("sock" + to_string(i)).c_str(), nuePandizzleMinBin, nuePandizzleCutBin, 
                                                                  (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");

        TH1D *numuProjection_deltaCP = numu_deltaCP[i]->ProjectionZ(("kangeroo" + to_string(i)).c_str(), (numuPandizzleCutBin + 1), numuPandizzleMaxBin, 
                                                                    pandrizzleMinBin, pandrizzleMaxBin, "o");

        // remove nue selected from numu hist 
        if (numuPandizzleCut < nuePandizzle)
        {
            TH1D * bothProjection_deltaCP = numu_deltaCP[i]->ProjectionZ(("fly" + to_string(i)).c_str(), (numuPandizzleCutBin + 1), nuePandizzleCutBin, (pandrizzleCutBin + 1), pandrizzleMaxBin, "o");
            numuProjection_deltaCP->Add(bothProjection_deltaCP, -1.0);
        }

        const double nueLogLikelihood(CalculateMinLogLikelihood(nueProjection_zero, nueProjection_pi, nueProjection_deltaCP));
        const double numuLogLikelihood(CalculateMinLogLikelihood(numuProjection_zero, numuProjection_pi, numuProjection_deltaCP));
        const double logLikelihoodSum(nueLogLikelihood + numuLogLikelihood);

        if (logLikelihoodSum < 0.0)
        {
            std::cout << "nueLogLikelihood: " << nueLogLikelihood << ", numuLogLikelihood: " << numuLogLikelihood << std::endl;
            std::cout << "ISOBEL YOU HAVE A NEGATIVE LOGLIKELIHOOD SUM" << std::endl;
            throw;
        }

        const double sigTotal = std::sqrt(2.0 * logLikelihoodSum);

        if ((previousSigTotal > 3.0) && (sigTotal > 3.0))
            totalCoveredRange += deltaCPStepSize;

        previousSigTotal = sigTotal;
    }

    return totalCoveredRange;
}
*/
//------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------------------------------
