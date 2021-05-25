#include "neutrinoEvent.h"

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
    tree->SetBranchAddress("BeamPdg", &nu.m_beamPdg);
    tree->SetBranchAddress("NuPdg", &nu.m_nuPdg);
    tree->SetBranchAddress("ProjectedPOTWeight", &nu.m_projectedPOTWeight);
    tree->SetBranchAddress("TargetZ", &nu.m_target);
    tree->SetBranchAddress("RecoNuVtxNChildren", &nu.m_recoNuVtxNChildren);
    tree->SetBranchAddress("Enu", &nu.m_eNu);
    tree->SetBranchAddress("Q2", &nu.m_qSqr);
    tree->SetBranchAddress("MomLepT", &nu.m_lepMom);
    tree->SetBranchAddress("LepNuAngle", &nu.m_lepNuOpeningAngle);
    tree->SetBranchAddress("NueRecoENu", &nu.m_nueRecoENu);
    tree->SetBranchAddress("NumuRecoENu", &nu.m_numuRecoENu);
    tree->SetBranchAddress("SelTrackRecoContained", &nu.m_selTrackContained);
    tree->SetBranchAddress("SelTrackRecoMomMethod", &nu.m_selTrackMomentumMethod);
    tree->SetBranchAddress("SelTrackRecoEndX", &nu.m_selTrackRecoEndX);
    tree->SetBranchAddress("SelTrackRecoEndY", &nu.m_selTrackRecoEndY);
    tree->SetBranchAddress("SelTrackRecoEndZ", &nu.m_selTrackRecoEndZ);
    tree->SetBranchAddress("SelTrackTrueEndX", &nu.m_selTrackTrueEndX);
    tree->SetBranchAddress("SelTrackTrueEndY", &nu.m_selTrackTrueEndY);
    tree->SetBranchAddress("SelTrackTrueEndZ", &nu.m_selTrackTrueEndZ);
    tree->SetBranchAddress("NuX", &nu.m_nuTrueVertexX);
    tree->SetBranchAddress("NuY", &nu.m_nuTrueVertexY);
    tree->SetBranchAddress("NuZ", &nu.m_nuTrueVertexZ);
    tree->SetBranchAddress("RecoNuVtxX", &nu.m_nuRecoVertexX);
    tree->SetBranchAddress("RecoNuVtxY", &nu.m_nuRecoVertexY);
    tree->SetBranchAddress("RecoNuVtxZ", &nu.m_nuRecoVertexZ);
    tree->SetBranchAddress("SelTrackPandizzleVar", &nu.m_selTrackPandizzleScore);
    tree->SetBranchAddress("SelShowerPandrizzleMVAScore", &nu.m_selShowerPandrizzleScore);
    tree->SetBranchAddress("SelShowerMVAElectron", &nu.m_selShowerElectronScore);
    tree->SetBranchAddress("SelShowerRecoStartX", &nu.m_selShowerRecoStartX);
    tree->SetBranchAddress("SelShowerRecoStartY", &nu.m_selShowerRecoStartY);
    tree->SetBranchAddress("SelShowerRecoStartZ", &nu.m_selShowerRecoStartZ);

    for (Int_t i = 0; i < tree->GetEntries(); ++i)
    {
        tree->GetEntry(i);
        nuEventVector.push_back(nu);
    }
}
