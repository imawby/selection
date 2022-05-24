#include "neutrinoEvent.h"

void ReadFile(const std::string &inputFileName, NeutrinoEventVector &nuEventVector);
void ReadFile(const std::string &inputFileName, ElectronValidation &electronValidation);

void ReadFile(const std::string &inputFileName, NeutrinoEventVector &nuEventVector)
{
    TFile *inputFile = new TFile(inputFileName.c_str());
    TTree *tree = (TTree*)inputFile->Get("ccnuselection/ccnusel");

    std::cout << "tree->GetEntries(): " << tree->GetEntries() << std::endl;

    NeutrinoEvent nu;

    tree->SetBranchAddress("Run", &nu.m_run);
    tree->SetBranchAddress("SubRun", &nu.m_subrun);
    tree->SetBranchAddress("Event", &nu.m_event);
    tree->SetBranchAddress("NC", &nu.m_isNC);
    tree->SetBranchAddress("Mode", &nu.m_intType);
    tree->SetBranchAddress("BeamPdg", &nu.m_beamPdg);
    tree->SetBranchAddress("NuPdg", &nu.m_nuPdg);
    tree->SetBranchAddress("ProjectedPOTWeight", &nu.m_projectedPOTWeight);
    tree->SetBranchAddress("TargetZ", &nu.m_target);
    tree->SetBranchAddress("Enu", &nu.m_eNu);
    tree->SetBranchAddress("Q2", &nu.m_qSqr);
    tree->SetBranchAddress("MomLepT", &nu.m_lepMom);
    tree->SetBranchAddress("LepNuAngle", &nu.m_lepNuOpeningAngle);
    tree->SetBranchAddress("RecoShowerRecoNHits", &nu.m_nPFPHits);
    tree->SetBranchAddress("NueRecoENu", &nu.m_nueRecoENu);
    tree->SetBranchAddress("NumuRecoENu", &nu.m_numuRecoENu);
    tree->SetBranchAddress("SelTrackTruePDG", &nu.m_selTrackTruePdg);
    tree->SetBranchAddress("SelShowerTruePDG", &nu.m_selShowerTruePdg);
    tree->SetBranchAddress("SelTrackRecoContained", &nu.m_selTrackContained);
    tree->SetBranchAddress("SelTrackRecoMomMethod", &nu.m_selTrackMomentumMethod);
    tree->SetBranchAddress("SelTrackRecoEndX", &nu.m_selTrackRecoEndX);
    tree->SetBranchAddress("SelTrackRecoEndY", &nu.m_selTrackRecoEndY);
    tree->SetBranchAddress("SelTrackRecoEndZ", &nu.m_selTrackRecoEndZ);
    tree->SetBranchAddress("SelTrackTrueStartX", &nu.m_selTrackTrueStartX);
    tree->SetBranchAddress("SelTrackTrueStartY", &nu.m_selTrackTrueStartY);
    tree->SetBranchAddress("SelTrackTrueStartZ", &nu.m_selTrackTrueStartZ);
    tree->SetBranchAddress("SelTrackTrueEndX", &nu.m_selTrackTrueEndX);
    tree->SetBranchAddress("SelTrackTrueEndY", &nu.m_selTrackTrueEndY);
    tree->SetBranchAddress("SelTrackTrueEndZ", &nu.m_selTrackTrueEndZ);
    tree->SetBranchAddress("NuX", &nu.m_nuTrueVertexX);
    tree->SetBranchAddress("NuY", &nu.m_nuTrueVertexY);
    tree->SetBranchAddress("NuZ", &nu.m_nuTrueVertexZ);
    tree->SetBranchAddress("RecoNuVtxX", &nu.m_nuRecoVertexX);
    tree->SetBranchAddress("RecoNuVtxY", &nu.m_nuRecoVertexY);
    tree->SetBranchAddress("RecoNuVtxZ", &nu.m_nuRecoVertexZ);
    tree->SetBranchAddress("RecoNuVtxNChildren", &nu.m_recoNuVtxNChildren);
    tree->SetBranchAddress("RecoNuVtxNTracks", &nu.m_recoNuVtxNTracks);
    tree->SetBranchAddress("RecoNuVtxNShowers", &nu.m_recoNuVtxNShowers);
    tree->SetBranchAddress("SelTrackPandizzleVar", &nu.m_selTrackPandizzleScore);
    tree->SetBranchAddress("SelShowerJamPandrizzleMVAScore", &nu.m_selShowerJamPandrizzleScore);
    tree->SetBranchAddress("SelShowerPandrizzleMVAScore", &nu.m_selShowerPandrizzleScore);
    tree->SetBranchAddress("SelShowerMVAElectron", &nu.m_selShowerElectronScore);
    tree->SetBranchAddress("SelShowerTrueStartX", &nu.m_selShowerTrueStartX);
    tree->SetBranchAddress("SelShowerTrueStartY", &nu.m_selShowerTrueStartY);
    tree->SetBranchAddress("SelShowerTrueStartZ", &nu.m_selShowerTrueStartZ);
    tree->SetBranchAddress("SelShowerRecoStartX", &nu.m_selShowerStartX);
    tree->SetBranchAddress("SelShowerRecoStartY", &nu.m_selShowerStartY);
    tree->SetBranchAddress("SelShowerRecoStartZ", &nu.m_selShowerStartZ);
    tree->SetBranchAddress("SelShowerRecoCompleteness", &nu.m_selShowerCompleteness);
    tree->SetBranchAddress("SelShowerRecoHitPurity", &nu.m_selShowerPurity);
    tree->SetBranchAddress("SelShowerTrueMomX", &nu.m_selShowerTrueMomX);
    tree->SetBranchAddress("SelShowerTrueMomY", &nu.m_selShowerTrueMomY);
    tree->SetBranchAddress("SelShowerTrueMomZ", &nu.m_selShowerTrueMomZ);
    tree->SetBranchAddress("SelShowerRecoDirX", &nu.m_selShowerDirectionX);
    tree->SetBranchAddress("SelShowerRecoDirY", &nu.m_selShowerDirectionY);
    tree->SetBranchAddress("SelShowerRecoDirZ", &nu.m_selShowerDirectionZ);    

    tree->SetBranchAddress("SelShowerPandrizzleDisplacement", &nu.m_selShowerDisplacement);
    tree->SetBranchAddress("SelShowerPandrizzleDCA", &nu.m_selShowerDCA);

    tree->SetBranchAddress("NRecoTracks",&nu.m_nRecoTracks);
    tree->SetBranchAddress("NRecoShowers",&nu.m_nRecoShowers);

    //tree->SetBranchAddress("RecoTrackRecoEndClosestToVertexX", &nu.m_recoTrackRecoEndClosestToVertexX);
    //tree->SetBranchAddress("RecoTrackRecoEndClosestToVertexY", &nu.m_recoTrackRecoEndClosestToVertexY);
    //tree->SetBranchAddress("RecoTrackRecoEndClosestToVertexZ", &nu.m_recoTrackRecoEndClosestToVertexZ);


    tree->SetBranchAddress("CVNResultNue", &nu.m_cvnResultNue);
    tree->SetBranchAddress("CVNResultNumu", &nu.m_cvnResultNumu);
    tree->SetBranchAddress("CVNResultNutau", &nu.m_cvnResultNutau);
    tree->SetBranchAddress("CVNResultNC", &nu.m_cvnResultNC);

    
    // might have to shimmy these down to only store the selected electron ones? 
    /*
    tree->SetBranchAddress("RecoShowerTrueMomX", &allShowerTrueMomX);
    tree->SetBranchAddress("RecoShowerTrueMomY", &allShowerTrueMomY);
    tree->SetBranchAddress("RecoShowerTrueMomZ", &allShowerTrueMomZ);
    tree->SetBranchAddress("RecoShowerTrueMomT", &allShowerTrueMomT);

    tree->SetBranchAddress("RecoShowerRecoDirX", &allShowerDirectionX);
    tree->SetBranchAddress("RecoShowerRecoDirY", &allShowerDirectionY);
    tree->SetBranchAddress("RecoShowerRecoDirZ", &allShowerDirectionZ);    

    */

    std::cout << "Reading neutrino events... " << std::endl;
    for (Int_t i = 0; i < tree->GetEntries(); ++i)
    {
        //std::cout << "i: " << i << std::endl;
        tree->GetEntry(i);

        nuEventVector.push_back(nu);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

void ReadFile(const std::string &inputFileName, ElectronValidation &electronValidation)
{
    TFile *inputFile = new TFile(inputFileName.c_str());
    TTree *tree = (TTree*)inputFile->Get("ccnuselection/ccnusel");

    std::cout << "tree->GetEntries(): " << tree->GetEntries() << std::endl;

    NeutrinoEvent nu;

    tree->SetBranchAddress("Run", &nu.m_run);
    tree->SetBranchAddress("SubRun", &nu.m_subrun);
    tree->SetBranchAddress("Event", &nu.m_event);
    tree->SetBranchAddress("NC", &nu.m_isNC);
    tree->SetBranchAddress("BeamPdg", &nu.m_beamPdg);
    tree->SetBranchAddress("NuPdg", &nu.m_nuPdg);
    tree->SetBranchAddress("ProjectedPOTWeight", &nu.m_projectedPOTWeight);
    tree->SetBranchAddress("TargetZ", &nu.m_target);
    tree->SetBranchAddress("Enu", &nu.m_eNu);
    tree->SetBranchAddress("Q2", &nu.m_qSqr);
    tree->SetBranchAddress("MomLepT", &nu.m_lepMom);
    tree->SetBranchAddress("LepNuAngle", &nu.m_lepNuOpeningAngle);
    tree->SetBranchAddress("NueRecoENu", &nu.m_nueRecoENu);
    tree->SetBranchAddress("NumuRecoENu", &nu.m_numuRecoENu);
    tree->SetBranchAddress("SelTrackTruePDG", &nu.m_selTrackTruePdg);
    tree->SetBranchAddress("SelShowerTruePDG", &nu.m_selShowerTruePdg);
    tree->SetBranchAddress("SelTrackRecoContained", &nu.m_selTrackContained);
    tree->SetBranchAddress("SelTrackRecoMomMethod", &nu.m_selTrackMomentumMethod);
    tree->SetBranchAddress("SelTrackRecoEndX", &nu.m_selTrackRecoEndX);
    tree->SetBranchAddress("SelTrackRecoEndY", &nu.m_selTrackRecoEndY);
    tree->SetBranchAddress("SelTrackRecoEndZ", &nu.m_selTrackRecoEndZ);
    tree->SetBranchAddress("SelTrackTrueStartX", &nu.m_selTrackTrueStartX);
    tree->SetBranchAddress("SelTrackTrueStartY", &nu.m_selTrackTrueStartY);
    tree->SetBranchAddress("SelTrackTrueStartZ", &nu.m_selTrackTrueStartZ);
    tree->SetBranchAddress("SelTrackTrueEndX", &nu.m_selTrackTrueEndX);
    tree->SetBranchAddress("SelTrackTrueEndY", &nu.m_selTrackTrueEndY);
    tree->SetBranchAddress("SelTrackTrueEndZ", &nu.m_selTrackTrueEndZ);
    tree->SetBranchAddress("NuX", &nu.m_nuTrueVertexX);
    tree->SetBranchAddress("NuY", &nu.m_nuTrueVertexY);
    tree->SetBranchAddress("NuZ", &nu.m_nuTrueVertexZ);
    tree->SetBranchAddress("RecoNuVtxX", &nu.m_nuRecoVertexX);
    tree->SetBranchAddress("RecoNuVtxY", &nu.m_nuRecoVertexY);
    tree->SetBranchAddress("RecoNuVtxZ", &nu.m_nuRecoVertexZ);
    tree->SetBranchAddress("RecoNuVtxNChildren", &nu.m_recoNuVtxNChildren);
    tree->SetBranchAddress("RecoNuVtxNTracks", &nu.m_recoNuVtxNTracks);
    tree->SetBranchAddress("RecoNuVtxNShowers", &nu.m_recoNuVtxNShowers);
    tree->SetBranchAddress("SelTrackPandizzleVar", &nu.m_selTrackPandizzleScore);
    tree->SetBranchAddress("SelShowerJamPandrizzleMVAScore", &nu.m_selShowerJamPandrizzleScore);
    tree->SetBranchAddress("SelShowerPandrizzleMVAScore", &nu.m_selShowerPandrizzleScore);
    tree->SetBranchAddress("SelShowerMVAElectron", &nu.m_selShowerElectronScore);
    tree->SetBranchAddress("SelShowerTrueStartX", &nu.m_selShowerTrueStartX);
    tree->SetBranchAddress("SelShowerTrueStartY", &nu.m_selShowerTrueStartY);
    tree->SetBranchAddress("SelShowerTrueStartZ", &nu.m_selShowerTrueStartZ);
    tree->SetBranchAddress("SelShowerRecoStartX", &nu.m_selShowerStartX);
    tree->SetBranchAddress("SelShowerRecoStartY", &nu.m_selShowerStartY);
    tree->SetBranchAddress("SelShowerRecoStartZ", &nu.m_selShowerStartZ);
    tree->SetBranchAddress("SelShowerRecoCompleteness", &nu.m_selShowerCompleteness);
    tree->SetBranchAddress("SelShowerRecoHitPurity", &nu.m_selShowerPurity);
    tree->SetBranchAddress("SelShowerTrueMomX", &nu.m_selShowerTrueMomX);
    tree->SetBranchAddress("SelShowerTrueMomY", &nu.m_selShowerTrueMomY);
    tree->SetBranchAddress("SelShowerTrueMomZ", &nu.m_selShowerTrueMomZ);
    tree->SetBranchAddress("SelShowerRecoDirX", &nu.m_selShowerDirectionX);
    tree->SetBranchAddress("SelShowerRecoDirY", &nu.m_selShowerDirectionY);
    tree->SetBranchAddress("SelShowerRecoDirZ", &nu.m_selShowerDirectionZ);    

    tree->SetBranchAddress("SelShowerPandrizzleDisplacement", &nu.m_selShowerDisplacement);
    tree->SetBranchAddress("SelShowerPandrizzleDCA", &nu.m_selShowerDCA);

    tree->SetBranchAddress("NRecoTracks",&nu.m_nRecoTracks);
    tree->SetBranchAddress("NRecoShowers",&nu.m_nRecoShowers);
    tree->SetBranchAddress("CVNResultNue", &nu.m_cvnResultNue);
    tree->SetBranchAddress("CVNResultNumu", &nu.m_cvnResultNumu);
    tree->SetBranchAddress("CVNResultNutau", &nu.m_cvnResultNutau);
    tree->SetBranchAddress("CVNResultNC", &nu.m_cvnResultNC);
    
    // group things
    
    bool allShowerIsPrimary[1000];
    int allShowerTruePdg[1000];
    float allShowerRecoEnergy[1000][3];
    int allShowerNHits[1000];

    tree->SetBranchAddress("RecoShowerRecoIsPrimaryPFPDaughter", &allShowerIsPrimary); // is reconstructed primary of neutrino
    tree->SetBranchAddress("RecoShowerTruePDG", &allShowerTruePdg);
    tree->SetBranchAddress("RecoShowerRecoEnergy", &allShowerRecoEnergy);
    tree->SetBranchAddress("RecoShowerRecoNHits", &allShowerNHits);

    std::cout << "Reading neutrino events... " << std::endl;

    for (Int_t i = 0; i < tree->GetEntries(); ++i)
    {
        tree->GetEntry(i);

        if (nu.m_nuPdg != 12)
            continue;

        if (nu.m_isNC)
            continue;

        if (nu.m_target != 18)
            continue;

        const double minX(-360.0 + 50.0), maxX(360.0 - 50.0);
        const double minY(-600.0 + 50.0), maxY(600.0 - 50.0);
        const double minZ(50.0), maxZ(1394.0 - 150.0);

        if ((nu.m_nuTrueVertexX < minX) || (nu.m_nuTrueVertexX > maxX))
            continue;

        if ((nu.m_nuTrueVertexY < minY) || (nu.m_nuTrueVertexY > maxY))
            continue;

        if ((nu.m_nuTrueVertexZ < minZ) || (nu.m_nuTrueVertexZ > maxZ))
            continue;

        int nPFPHits(-1);

        float highestEnergyShower(-std::numeric_limits<float>::max());

        for (unsigned int i = 0; i < nu.m_nRecoShowers; ++i)
        {
            if (allShowerIsPrimary[i] && (std::abs(allShowerTruePdg[i]) == 11))
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (allShowerRecoEnergy[i][j] > highestEnergyShower)
                    {
                        highestEnergyShower = allShowerRecoEnergy[i][j];
                        nPFPHits = allShowerNHits[i];
                    }
                }
            }
        }

        electronValidation.m_nPFPHits.push_back(nPFPHits);
        electronValidation.m_trueNueEnergy.push_back(nu.m_eNu); // can't really do a reconstructed nue energy with anything other than the selected shower... 
        electronValidation.m_beamPdg.push_back(nu.m_beamPdg);
        electronValidation.m_nuPdg.push_back(nu.m_nuPdg);
        electronValidation.m_POTWeight.push_back(nu.m_projectedPOTWeight);
    }
}
