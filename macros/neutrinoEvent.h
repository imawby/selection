#ifndef NEUTRINO_EVENT_H
#define NEUTRINO_EVENT_H 1

constexpr int kDefInt = -9999;
constexpr int kDefDouble = (double)(kDefInt);

class NeutrinoEvent
{
public:
    NeutrinoEvent();

    int m_run;
    int m_subrun;
    int m_event;
    int m_isNC;
    int m_beamPdg;
    int m_nuPdg;
    double m_projectedPOTWeight;
    int m_target;
    double m_eNu;
    double m_qSqr;
    double m_lepMom;
    double m_lepNuOpeningAngle;

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

    double m_recoTrackRecoEndClosestToVertexX[500];
    double m_recoTrackRecoEndClosestToVertexY[500];
    double m_recoTrackRecoEndClosestToVertexZ[500];

    double m_selShowerPandrizzleScore;
    double m_selShowerElectronScore;

    double m_selShowerRecoStartX;
    double m_selShowerRecoStartY;
    double m_selShowerRecoStartZ;
    int m_selShowerTruePdg;

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
    m_beamPdg(kDefInt),
    m_nuPdg(kDefInt),
    m_target(kDefInt),
    m_recoNuVtxNChildren(kDefInt),
    m_eNu(kDefDouble),
    m_qSqr(kDefDouble),
    m_lepMom(kDefDouble),
    m_lepNuOpeningAngle(kDefDouble),
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
    m_selShowerPandrizzleScore(kDefDouble),
    m_selShowerElectronScore(kDefDouble),
    m_selShowerRecoStartX(kDefDouble),
    m_selShowerRecoStartY(kDefDouble),
    m_selShowerRecoStartZ(kDefDouble),
    m_cvnResultNue(kDefDouble),
    m_cvnResultNumu(kDefDouble),
    m_cvnResultNutau(kDefDouble),
    m_cvnResultNC(kDefDouble)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ReadFile(const std::string &inputFileName, NeutrinoEventVector &nuEventVector);

#endif
