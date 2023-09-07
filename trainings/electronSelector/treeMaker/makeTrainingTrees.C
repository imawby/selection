//STIL
#include <iostream>
#include <cstdlib>
#include <vector>

//ROOT
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TVector3.h"

const double COMPLETENESS_CUT(0.5);
const double PURITY_CUT(0.9);

void MakeTrainingTrees(TString inputFile)
{
    TFile *inFile(new TFile(inputFile,"READ"));
    TTree *inTree((TTree*) inFile->Get("ccnuselection/ccnusel"));

    int truePDGIn;
    double trueEnergyIn;
    double evalRatioIn;
    double concentrationIn;
    double coreHaloRatioIn;
    double conicalnessIn;
    double dEdxStartIn;
    double dEdxEndIn;
    double dEdxEndRatioIn;
    double dEdxIn[3];
    int bestPlaneIn;
    double nuXIn;
    double nuYIn;
    double nuZIn;
    double showerXIn;
    double showerYIn;
    double showerZIn;
    double showerDirXIn;
    double showerDirYIn;
    double showerDirZIn;
    double openingAngleIn;
    double lengthIn;
    double energyIn[3];

    int nPFPHitsIn;
    int nRecoTracksIn;
    int recoTrackHitCountIn[1000];

    int nRecoShowers;
    double allShowerCompleteness[1000];
    double allShowerPurity[1000];

    inTree->SetBranchAddress("NRecoTracks", &nRecoTracksIn);
    inTree->SetBranchAddress("SelShowerRecoNHits", &nPFPHitsIn);
    inTree->SetBranchAddress("RecoTrackRecoNHits", &recoTrackHitCountIn);

    inTree->SetBranchAddress("NRecoShowers",&nRecoShowers);
    inTree->SetBranchAddress("RecoShowerRecoCompleteness", &allShowerCompleteness);
    inTree->SetBranchAddress("RecoShowerRecoHitPurity", &allShowerPurity);

    inTree->SetBranchAddress("SelShowerTruePDG",&truePDGIn);
    inTree->SetBranchAddress("SelShowerTrueMomT",&trueEnergyIn);
    inTree->SetBranchAddress("SelShowerMVAEvalRatio",&evalRatioIn);
    inTree->SetBranchAddress("SelShowerMVAConcentration",&concentrationIn);
    inTree->SetBranchAddress("SelShowerMVACoreHaloRatio",&coreHaloRatioIn);
    inTree->SetBranchAddress("SelShowerMVAConicalness",&conicalnessIn);
    inTree->SetBranchAddress("SelShowerMVAdEdxStart",&dEdxStartIn);
    inTree->SetBranchAddress("SelShowerMVAdEdxEnd",&dEdxEndIn);
    inTree->SetBranchAddress("SelShowerMVAdEdxEndRatio",&dEdxEndRatioIn);
    inTree->SetBranchAddress("SelShowerRecodEdx",&dEdxIn);
    inTree->SetBranchAddress("SelShowerRecoBestPlane",&bestPlaneIn);
    inTree->SetBranchAddress("RecoNuVtxX",&nuXIn);
    inTree->SetBranchAddress("RecoNuVtxY",&nuYIn);
    inTree->SetBranchAddress("RecoNuVtxZ",&nuZIn);
    inTree->SetBranchAddress("SelShowerRecoStartX",&showerXIn);
    inTree->SetBranchAddress("SelShowerRecoStartY",&showerYIn);
    inTree->SetBranchAddress("SelShowerRecoStartZ",&showerZIn);
    inTree->SetBranchAddress("SelShowerRecoDirX",&showerDirXIn);
    inTree->SetBranchAddress("SelShowerRecoDirY",&showerDirYIn);
    inTree->SetBranchAddress("SelShowerRecoDirZ",&showerDirZIn);
    inTree->SetBranchAddress("SelShowerRecoOpeningAngle",&openingAngleIn);
    inTree->SetBranchAddress("SelShowerRecoLength",&lengthIn);
    inTree->SetBranchAddress("SelShowerRecoEnergy",&energyIn);

    TFile *outFile(new TFile("outputFile_recoopflux.root","RECREATE"));
    TTree *sigTree(new TTree("sigtree","signal tree"));
    TTree *bgTree(new TTree("bgtree","bgnal tree"));
    std::vector<TTree*> trees;
    trees.emplace_back(sigTree);
    trees.emplace_back(bgTree);

    int truePDGOut;
    float trueEnergyOut;
    float evalRatioOut;
    float concentrationOut;
    float coreHaloRatioOut;
    float conicalnessOut;
    float dEdxStartOut;
    float dEdxEndOut;
    float dEdxEndRatioOut;
    float dEdxColPlaneOut;
    float dEdxBestPlaneOut;
    float displacementOut;
    float dcaOut;
    float widenessOut;
    float energyDensityOut;
    float lengthOut;
    float openingAngleOut;

    int nPFPHits;
    int nRecoTracks;
    int nHighestTrackHitCount;


    for (unsigned int iTree = 0; iTree < trees.size(); ++iTree)
    {
        trees[iTree]->Branch("PFPNHits",&nPFPHits);
        trees[iTree]->Branch("NRecoTracks",&nRecoTracks);
        trees[iTree]->Branch("NHighestTrackHitCount",&nHighestTrackHitCount);
        trees[iTree]->Branch("TruePDG",&truePDGOut);
        trees[iTree]->Branch("TrueEnergy",&trueEnergyOut);
        trees[iTree]->Branch("EvalRatio",&evalRatioOut);
        trees[iTree]->Branch("Concentration",&concentrationOut);
        trees[iTree]->Branch("CoreHaloRatio",&coreHaloRatioOut);
        trees[iTree]->Branch("Conicalness",&conicalnessOut);
        trees[iTree]->Branch("dEdxStart",&dEdxStartOut);
        trees[iTree]->Branch("dEdxEnd",&dEdxEndOut);
        trees[iTree]->Branch("dEdxEndRatio",&dEdxEndRatioOut);
        trees[iTree]->Branch("dEdxColPlane",&dEdxColPlaneOut);
        trees[iTree]->Branch("dEdxBestPlane",&dEdxBestPlaneOut);
        trees[iTree]->Branch("Displacement",&displacementOut);
        trees[iTree]->Branch("DCA",&dcaOut);
        trees[iTree]->Branch("Wideness",&widenessOut);
        trees[iTree]->Branch("EnergyDensity",&energyDensityOut);
        trees[iTree]->Branch("OpeningAngle",&openingAngleOut);
        trees[iTree]->Branch("Length",&lengthOut);
    }

    bool passCut = false;

    UInt_t nEvents(inTree->GetEntries());
    for (UInt_t iEvent = 0; iEvent < nEvents; ++iEvent)
    {
        passCut = true;

        inTree->GetEntry(iEvent);

        if (truePDGIn == -9999)
            continue;

        for (int i = 0; i < nRecoShowers; ++i)
        {
            if ((allShowerCompleteness[i] < COMPLETENESS_CUT) || (allShowerPurity[i] < PURITY_CUT))
            {
                passCut = false;
                break;
            }
        }

        if (!passCut)
            continue;

        nPFPHits = nPFPHitsIn;
        nRecoTracks = nRecoTracksIn;

        nHighestTrackHitCount = 0;
        for (int i = 0; i < nRecoTracks; ++i)
        {
            if (recoTrackHitCountIn[i] > nHighestTrackHitCount)
                nHighestTrackHitCount = recoTrackHitCountIn[i];
        }

        truePDGOut = truePDGIn;
        trueEnergyOut = static_cast<float>(trueEnergyIn);
        if (isnan(evalRatioIn))
            evalRatioOut = -0.5f;
        else
            evalRatioOut = static_cast<float>(evalRatioIn);
        if (isnan(concentrationIn))
            concentrationOut = -2.f;
        else 
            concentrationOut = std::min(static_cast<float>(concentrationIn), 50.f);
        coreHaloRatioOut = static_cast<float>(coreHaloRatioIn);
        conicalnessOut = std::min(static_cast<float>(conicalnessIn), 100.f);
        dEdxStartOut = static_cast<float>(dEdxStartIn);
        dEdxEndOut = static_cast<float>(dEdxEndIn);
        dEdxEndRatioOut = static_cast<float>(dEdxEndRatioIn);
        dEdxColPlaneOut = std::max(std::min(static_cast<float>(dEdxIn[2]),20.f),-2.f);
        dEdxBestPlaneOut = std::max(std::min(static_cast<float>(dEdxIn[bestPlaneIn]),20.f),-2.f);

        TVector3 showerPos(showerXIn, showerYIn, showerZIn);
        TVector3 nuPos(nuXIn, nuYIn, nuZIn);
        displacementOut = std::min(static_cast<float>((showerPos-nuPos).Mag()), 100.f);

        TVector3 showerDir(showerDirXIn, showerDirYIn, showerDirZIn);
        double alpha((showerPos-nuPos).Dot(showerDir));
        TVector3 r(showerPos+alpha*showerDir);
        dcaOut = std::min(static_cast<float>((r-nuPos).Mag()), 50.f);

        widenessOut = std::min(static_cast<float>(openingAngleIn)/static_cast<float>(lengthIn),0.1f);
        if (isnan(widenessOut))
            widenessOut = -0.01f;

        float volume(3.14159265359*static_cast<float>(lengthIn)*static_cast<float>(lengthIn)*static_cast<float>(lengthIn)*static_cast<float>(std::tan(openingAngleIn))/3);
        energyDensityOut = std::min(std::max(static_cast<float>(energyIn[2])/volume,-0.1f),5.f);
        if (isnan(energyDensityOut))
            energyDensityOut = -0.1f;

        lengthOut = static_cast<float>(lengthIn);
        openingAngleOut = static_cast<float>(openingAngleIn);

        if (std::abs(truePDGIn) == 11)
            sigTree->Fill();
        else
            bgTree->Fill();
    }
    inFile->Close();
    outFile->Write();
    return;
}
