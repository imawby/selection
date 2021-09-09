#include "NeutrinoEvent.C"
#include "Selection.C"

void DirectionResolution(const std::string &inputFileName);
void DrawHistograms(NeutrinoEventVector &nuEventVector_full, ElectronValidation &electronValidation);

void DirectionResolution(const std::string &inputFileName_full)
{
    ElectronValidation electronValidation;
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full, electronValidation);

    DrawHistograms(nuEventVector_full, electronValidation);    
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawHistograms(NeutrinoEventVector &nuEventVector_full, ElectronValidation &electronValidation)
{
    const int pandrizzleBins = 25, directionOpeningAngleBins = 50;
    double pandrizzleMin = -1.0, directionOpeningAngleMin = 0.0;
    double pandrizzleMax = 1.0, directionOpeningAngleMax = 180.0;
    double directionOpeningAngleBinLimits[directionOpeningAngleBins] = {0.0,1.0,2.0,3.0,4.0,5.0,10.0,20.0,100.0};

    TH1D * directionOpeningAngleSel = new TH1D("directionOpeningAngleSel", "directionOpeningAngleSel", directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);
    TH2D * directionOpeningAnglePandrizzleSel = new TH2D("directionOpeningAnglePandrizzleSel", "directionOpeningAnglePandrizzleSel", pandrizzleBins, pandrizzleMin, pandrizzleMax, 
        directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);
    TH1D * countPandrizzleSel = new TH1D("countPandrizzleSel", "countPandrizzleSel", directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);
    TH1D * avPandrizzleSel = new TH1D("avPandrizzleSel", "avPandrizzleSel", directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);

    TH1D * directionOpeningAngleAll = new TH1D("directionOpeningAngleAll", "directionOpeningAngleAll", directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);
    TH2D * directionOpeningAnglePandrizzleAll = new TH2D("directionOpeningAnglePandrizzleAll", "directionOpeningAnglePandrizzleAll", pandrizzleBins, pandrizzleMin, pandrizzleMax, 
        directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);
    TH1D * countPandrizzleAll = new TH1D("countPandrizzleAll", "countPandrizzleAll", directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);
    TH1D * avPandrizzleAll = new TH1D("avPandrizzleAll", "avPandrizzleAll", directionOpeningAngleBins, directionOpeningAngleMin, directionOpeningAngleMax);

    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        // Fill selected shower histograms
        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            double dotProduct = (nu.m_selShowerTrueMomX * nu.m_selShowerDirectionX) + (nu.m_selShowerTrueMomY * nu.m_selShowerDirectionY) + 
                (nu.m_selShowerTrueMomZ * nu.m_selShowerDirectionZ); 
            double trueMag = std::sqrt((nu.m_selShowerTrueMomX * nu.m_selShowerTrueMomX) + (nu.m_selShowerTrueMomY * nu.m_selShowerTrueMomY) + 
                (nu.m_selShowerTrueMomZ * nu.m_selShowerTrueMomZ));
            double recoMag = std::sqrt((nu.m_selShowerDirectionX * nu.m_selShowerDirectionX) + (nu.m_selShowerDirectionY * nu.m_selShowerDirectionY) + 
                (nu.m_selShowerDirectionZ * nu.m_selShowerDirectionZ));
            double directionOpeningAngle = std::acos(dotProduct / (trueMag * recoMag)) * 180 / TMath::Pi();

            directionOpeningAngleSel->Fill(directionOpeningAngle, 1.0);
            directionOpeningAnglePandrizzleSel->Fill(nu.m_selShowerPandrizzleScore, directionOpeningAngle, 1.0);
            countPandrizzleSel->Fill(directionOpeningAngle, 1.0);
            avPandrizzleSel->Fill(directionOpeningAngle, nu.m_selShowerPandrizzleScore);
        }
    }

    for (unsigned int i = 0; i < electronValidation.m_allShowerTrueStartX.size(); ++i)
    {
        double dotProduct = (electronValidation.m_allShowerTrueMomX[i] * electronValidation.m_allShowerDirectionX[i]) + 
            (electronValidation.m_allShowerTrueMomY[i] * electronValidation.m_allShowerDirectionY[i]) + 
            (electronValidation.m_allShowerTrueMomZ[i] * electronValidation.m_allShowerDirectionZ[i]); 
        double trueMag = std::sqrt((electronValidation.m_allShowerTrueMomX[i] * electronValidation.m_allShowerTrueMomX[i]) + 
            (electronValidation.m_allShowerTrueMomY[i] * electronValidation.m_allShowerTrueMomY[i]) + 
            (electronValidation.m_allShowerTrueMomZ[i] * electronValidation.m_allShowerTrueMomZ[i]));
        double recoMag = std::sqrt((electronValidation.m_allShowerDirectionX[i] * electronValidation.m_allShowerDirectionX[i]) + 
            (electronValidation.m_allShowerDirectionY[i] * electronValidation.m_allShowerDirectionY[i]) + 
            (electronValidation.m_allShowerDirectionZ[i] * electronValidation.m_allShowerDirectionZ[i]));
        double directionOpeningAngle = std::acos(dotProduct / (trueMag * recoMag)) * 180 / TMath::Pi();

        directionOpeningAngleAll->Fill(directionOpeningAngle, 1.0);
        directionOpeningAnglePandrizzleAll->Fill(electronValidation.m_allShowerPandrizzleScore[i], directionOpeningAngle, 1.0);
        countPandrizzleAll->Fill(directionOpeningAngle, 1.0);
        avPandrizzleAll->Fill(directionOpeningAngle, electronValidation.m_allShowerPandrizzleScore[i]);
    }

    // Obtain average pandrizzle scores
    avPandrizzleSel->Divide(countPandrizzleSel);
    avPandrizzleAll->Divide(countPandrizzleAll);

    gStyle->SetOptStat(0);

    TCanvas * cAvPandrizzleSel = new TCanvas("AvPandrizzleSel", "AvPandrizzleSel");
    cAvPandrizzleSel->Divide(2,1);
    cAvPandrizzleSel->cd(1);
    avPandrizzleSel->SetTitle("av. Pandrizzle score - Selected True Electrons;DirectionOpeningAngle;av. Pandrizzle Score");
    avPandrizzleSel->Draw("hist");
    cAvPandrizzleSel->cd(2);
    countPandrizzleSel->SetTitle("Entries;DirectionOpeningAngle;nEvents");
    countPandrizzleSel->Draw("hist");

    TCanvas * cDirectionOpeningAngleSel = new TCanvas("DirectionOpeningAngleSel", "DirectionOpeningAngleSel");
    directionOpeningAngleSel->SetTitle("DirectionOpeningAngle - Selected True Electrons;DirectionOpeningAngle;nElectrons");
    directionOpeningAngleSel->Draw("hist");

    TCanvas * cDirectionOpeningAnglePandrizzleSel = new TCanvas("DirectionOpeningAnglePandrizzleSel", "DirectionOpeningAnglePandrizzleSel");
    directionOpeningAnglePandrizzleSel->SetTitle(";Pandrizzle Score;DirectionOpeningAngle");
    directionOpeningAnglePandrizzleSel->Draw("colz");

    TCanvas * cAvPandrizzleAll = new TCanvas("AvPandrizzleAll", "AvPandrizzleAll");
    cAvPandrizzleAll->Divide(2,1);
    cAvPandrizzleAll->cd(1);
    avPandrizzleAll->SetTitle("av. Pandrizzle score - All True Electrons;DirectionOpeningAngle;av. Pandrizzle Score");
    avPandrizzleAll->Draw("hist");
    cAvPandrizzleAll->cd(2);
    countPandrizzleAll->SetTitle("Entries;DirectionOpeningAngle;av. Pandrizzle Score");
    countPandrizzleAll->Draw("hist");

    TCanvas * cDirectionOpeningAngleAll = new TCanvas("DirectionOpeningAngleAll", "DirectionOpeningAngleAll");
    directionOpeningAngleAll->SetTitle("DirectionOpeningAngle - All True Electrons;DirectionOpeningAngle;nElectrons");
    directionOpeningAngleAll->Draw("hist");

    TCanvas * cDirectionOpeningAnglePandrizzleAll = new TCanvas("DirectionOpeningAnglePandrizzleAll", "DirectionOpeningAnglePandrizzleAll");
    directionOpeningAnglePandrizzleAll->SetTitle(";Pandrizzle Score;DirectionOpeningAngle");
    directionOpeningAnglePandrizzleAll->Draw("colz");

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
