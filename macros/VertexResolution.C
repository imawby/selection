#include "NeutrinoEvent.C"
#include "Selection.C"

void VertexResolution(const std::string &inputFileName);
void DrawHistograms(NeutrinoEventVector &nuEventVector_full, ElectronValidation &electronValidation);

void VertexResolution(const std::string &inputFileName_full)
{
    ElectronValidation electronValidation;
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full, electronValidation);

    DrawHistograms(nuEventVector_full, electronValidation);    
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawHistograms(NeutrinoEventVector &nuEventVector_full, ElectronValidation &electronValidation)
{
    const int pandrizzleBins = 25, vertexBins = 9;
    double pandrizzleMin = -1.0, vertexMin = 0.0;
    double pandrizzleMax = 1.0, vertexMax = 100.0;
    double vertexBinLimits[vertexBins] = {0.0,1.0,2.0,3.0,4.0,5.0,10.0,20.0,100.0};

    TH1D * vertexSel = new TH1D("vertexSel", "vertexSel", vertexBins, vertexBinLimits);
    TH2D * vertexPandrizzleSel = new TH2D("vertexPandrizzleSel", "vertexPandrizzleSel", pandrizzleBins, pandrizzleMin, pandrizzleMax, 
        vertexBins, vertexBinLimits);
    TH1D * countPandrizzleSel = new TH1D("countPandrizzleSel", "countPandrizzleSel", vertexBins, vertexBinLimits);
    TH1D * avPandrizzleSel = new TH1D("avPandrizzleSel", "avPandrizzleSel", vertexBins, vertexBinLimits);


    TH1D * vertexAll = new TH1D("vertexAll", "vertexAll", vertexBins, vertexBinLimits);
    TH2D * vertexPandrizzleAll = new TH2D("vertexPandrizzleAll", "vertexPandrizzleAll", pandrizzleBins, pandrizzleMin, pandrizzleMax, 
        vertexBins, vertexBinLimits);
    TH1D * countPandrizzleAll = new TH1D("countPandrizzleAll", "countPandrizzleAll", vertexBins, vertexBinLimits);
    TH1D * avPandrizzleAll = new TH1D("avPandrizzleAll", "avPandrizzleAll", vertexBins, vertexBinLimits);

    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        // Fill selected shower histograms
        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            double sigmaX = nu.m_selShowerTrueStartX - nu.m_selShowerStartX; 
            double sigmaY = nu.m_selShowerTrueStartY - nu.m_selShowerStartY;
            double sigmaZ = nu.m_selShowerTrueStartZ - nu.m_selShowerStartZ;
            double resolution = std::sqrt((sigmaX * sigmaX) + (sigmaY * sigmaY) + (sigmaZ * sigmaZ));

            vertexSel->Fill(resolution, 1.0);
            vertexPandrizzleSel->Fill(nu.m_selShowerPandrizzleScore, resolution, 1.0);
            countPandrizzleSel->Fill(resolution, 1.0);
            avPandrizzleSel->Fill(resolution, nu.m_selShowerPandrizzleScore);
        }
    }

        for (unsigned int i = 0; i < electronValidation.m_allShowerTrueStartX.size(); ++i)
        {
            double sigmaX = electronValidation.m_allShowerTrueStartX[i] - electronValidation.m_allShowerStartX[i];
            double sigmaY = electronValidation.m_allShowerTrueStartY[i] - electronValidation.m_allShowerStartY[i];
            double sigmaZ = electronValidation.m_allShowerTrueStartZ[i] - electronValidation.m_allShowerStartZ[i];
            double resolution = std::sqrt((sigmaX * sigmaX) + (sigmaY * sigmaY) + (sigmaZ * sigmaZ));

            vertexAll->Fill(resolution, 1.0);
            vertexPandrizzleAll->Fill(electronValidation.m_allShowerPandrizzleScore[i], resolution, 1.0);
            countPandrizzleAll->Fill(resolution, 1.0);
            avPandrizzleAll->Fill(resolution, electronValidation.m_allShowerPandrizzleScore[i]);
        }

    // Obtain average pandrizzle scores
    avPandrizzleSel->Divide(countPandrizzleSel);
    avPandrizzleAll->Divide(countPandrizzleAll);

    gStyle->SetOptStat(0);

    TCanvas * cAvPandrizzleSel = new TCanvas("AvPandrizzleSel", "AvPandrizzleSel");
    cAvPandrizzleSel->Divide(2,1);
    cAvPandrizzleSel->cd(1);
    avPandrizzleSel->SetTitle("av. Pandrizzle score - Selected True Electrons;Vertex;av. Pandrizzle Score");
    avPandrizzleSel->Draw("hist");
    cAvPandrizzleSel->cd(2);
    countPandrizzleSel->SetTitle("Entries;Vertex;nEvents");
    countPandrizzleSel->Draw("hist");

    TCanvas * cVertexSel = new TCanvas("VertexSel", "VertexSel");
    vertexSel->SetTitle("Vertex - Selected True Electrons;Vertex;nElectrons");
    vertexSel->Draw("hist");

    TCanvas * cVertexPandrizzleSel = new TCanvas("VertexPandrizzleSel", "VertexPandrizzleSel");
    vertexPandrizzleSel->SetTitle(";Pandrizzle Score;Vertex");
    vertexPandrizzleSel->Draw("colz");



    TCanvas * cAvPandrizzleAll = new TCanvas("AvPandrizzleAll", "AvPandrizzleAll");
    cAvPandrizzleAll->Divide(2,1);
    cAvPandrizzleAll->cd(1);
    avPandrizzleAll->SetTitle("av. Pandrizzle score - All True Electrons;Vertex;av. Pandrizzle Score");
    avPandrizzleAll->Draw("hist");
    cAvPandrizzleAll->cd(2);
    countPandrizzleAll->SetTitle("Entries;Vertex;av. Pandrizzle Score");
    countPandrizzleAll->Draw("hist");

    TCanvas * cVertexAll = new TCanvas("VertexAll", "VertexAll");
    vertexAll->SetTitle("Vertex - All True Electrons;Vertex;nElectrons");
    vertexAll->Draw("hist");

    TCanvas * cVertexPandrizzleAll = new TCanvas("VertexPandrizzleAll", "VertexPandrizzleAll");
    vertexPandrizzleAll->SetTitle(";Pandrizzle Score;Vertex");
    vertexPandrizzleAll->Draw("colz");



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
