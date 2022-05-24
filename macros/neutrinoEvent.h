#ifndef NEUTRINO_EVENT_H
#define NEUTRINO_EVENT_H 1

constexpr int kDefInt = -9999;
constexpr int kDefDouble = (double)(kDefInt);

class ElectronValidation
{
public:
    ElectronValidation();

    std::vector<int> m_nPFPHits;
    std::vector<double> m_trueNueEnergy;
    std::vector<int> m_beamPdg;
    std::vector<int> m_nuPdg;
    std::vector<double> m_POTWeight;
};

///////////////////////////////////////////////////////////////////////////////////////////

class NeutrinoEvent
{
public:
    NeutrinoEvent();

    int m_run;
    int m_subrun;
    int m_event;
    int m_isNC;
    int m_intType;
    int m_beamPdg;
    int m_nuPdg;
    double m_projectedPOTWeight;
    int m_target;
    double m_eNu;
    double m_qSqr;
    double m_lepMom;
    double m_lepNuOpeningAngle;
    int m_nPFPHits;

    // selected track
    int m_selTrackTruePdg;
    int m_selTrackContained;
    int m_selTrackMomentumMethod;
    double m_selTrackRecoEndX;
    double m_selTrackRecoEndY;
    double m_selTrackRecoEndZ;
    double m_selTrackTrueStartX;
    double m_selTrackTrueStartY;
    double m_selTrackTrueStartZ;
    double m_selTrackTrueEndX;
    double m_selTrackTrueEndY;
    double m_selTrackTrueEndZ;
    double m_selTrackPandizzleScore;

    double m_nueRecoENu;
    double m_numuRecoENu;

    double m_nuTrueVertexX;
    double m_nuTrueVertexY;
    double m_nuTrueVertexZ;

    double m_nuRecoVertexX;
    double m_nuRecoVertexY;
    double m_nuRecoVertexZ;

    int m_nRecoTracks;
    int m_recoNuVtxNChildren;
    int m_recoNuVtxNTracks;
    int m_recoNuVtxNShowers;

    //double m_recoTrackRecoEndClosestToVertexX[1000];
    //double m_recoTrackRecoEndClosestToVertexY[1000];
    //double m_recoTrackRecoEndClosestToVertexZ[1000];


    // Selected Shower Stuff
    double m_selShowerJamPandrizzleScore;
    double m_selShowerPandrizzleScore;
    double m_selShowerElectronScore;
    double m_selShowerStartX;
    double m_selShowerStartY;
    double m_selShowerStartZ;
    int m_selShowerTruePdg;
    double m_selShowerCompleteness;
    double m_selShowerPurity;
    double m_selShowerTrueStartX;
    double m_selShowerTrueStartY;
    double m_selShowerTrueStartZ;
    double m_selShowerTrueMomX;
    double m_selShowerTrueMomY;
    double m_selShowerTrueMomZ;
    double m_selShowerDirectionX;
    double m_selShowerDirectionY;
    double m_selShowerDirectionZ;
    double m_selShowerDisplacement;
    double m_selShowerDCA;
    double m_selShowerEvalRatio;
    double m_selShowerConcentration;
    double m_selShowerCoreHaloRatio;
    double m_selShowerConicalness;
    double m_selShowerdEdxBestPlane;
    double m_selShowerWideness;
    double m_selShowerEnergyDensity;

    // All Shower Stuff
    int m_nRecoShowers;
   

    /*
    double m_allShowerTrueMomX[1000];
    double m_allShowerTrueMomY[1000];
    double m_allShowerTrueMomZ[1000];
    double m_allShowerTrueMomT[1000];

    double m_allShowerDirectionX[1000];
    double m_allShowerDirectionY[1000];
    double m_allShowerDirectionZ[1000];

    */

    // CVN scores
    double m_cvnResultNue;
    double m_cvnResultNumu;
    double m_cvnResultNutau;
    double m_cvnResultNC;
};

typedef std::vector<NeutrinoEvent> NeutrinoEventVector;

//------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

NeutrinoEvent::NeutrinoEvent() :
   m_run(kDefInt),
    m_subrun(kDefInt),
    m_event(kDefInt),
    m_isNC(false),
    m_intType(kDefInt),
    m_beamPdg(kDefInt),
    m_nuPdg(kDefInt),
    m_target(kDefInt),
    m_recoNuVtxNChildren(kDefInt),
    m_eNu(kDefDouble),
    m_qSqr(kDefDouble),
    m_lepMom(kDefDouble),
    m_lepNuOpeningAngle(kDefDouble),
    m_nPFPHits(0),
    m_selTrackTruePdg(0),
    m_selTrackContained(-2),
    m_selTrackMomentumMethod(-2),
    m_selShowerTruePdg(0),
    m_nueRecoENu(kDefDouble),
    m_numuRecoENu(kDefDouble),
    m_selTrackRecoEndX(kDefDouble),
    m_selTrackRecoEndY(kDefDouble),
    m_selTrackRecoEndZ(kDefDouble),
    m_selTrackTrueEndX(kDefDouble),
    m_selTrackTrueEndY(kDefDouble),
    m_selTrackTrueEndZ(kDefDouble),
    m_nuTrueVertexX(kDefDouble),
    m_nuTrueVertexY(kDefDouble),
    m_nuTrueVertexZ(kDefDouble),
    m_nuRecoVertexX(kDefDouble),
    m_nuRecoVertexY(kDefDouble),
    m_nuRecoVertexZ(kDefDouble),
    m_selTrackPandizzleScore(kDefDouble),
    m_selShowerJamPandrizzleScore(kDefDouble),
    m_selShowerPandrizzleScore(kDefDouble),
    m_selShowerElectronScore(kDefDouble),
    m_selShowerStartX(kDefDouble),
    m_selShowerStartY(kDefDouble),
    m_selShowerStartZ(kDefDouble),
    m_selShowerCompleteness(kDefDouble),
    m_selShowerPurity(kDefDouble),
    m_selShowerTrueMomX(kDefDouble),
    m_selShowerTrueMomY(kDefDouble),
    m_selShowerTrueMomZ(kDefDouble),
    m_selShowerDirectionX(kDefDouble),
    m_selShowerDirectionY(kDefDouble),
    m_selShowerDirectionZ(kDefDouble),
    m_selShowerDisplacement(kDefDouble),
    m_selShowerDCA(kDefDouble),
    m_selShowerEvalRatio(kDefDouble),
    m_selShowerConcentration(kDefDouble),
    m_selShowerCoreHaloRatio(kDefDouble),
    m_selShowerConicalness(kDefDouble),
    m_selShowerdEdxBestPlane(kDefDouble),
    m_selShowerWideness(kDefDouble),
    m_selShowerEnergyDensity(kDefDouble),
    m_nRecoShowers(0),
    m_cvnResultNue(kDefDouble),
    m_cvnResultNumu(kDefDouble),
    m_cvnResultNutau(kDefDouble),
    m_cvnResultNC(kDefDouble)
{
}

///////////////////////////////////////////////////////////////////////////////////////////

ElectronValidation::ElectronValidation() :
    m_nPFPHits(std::vector<int>()),
    m_trueNueEnergy(std::vector<double>()),
    m_beamPdg(std::vector<int>()),
    m_nuPdg(std::vector<int>()),
    m_POTWeight(std::vector<double>())
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ReadFile(const std::string &inputFileName, NeutrinoEventVector &nuEventVector);

#endif
