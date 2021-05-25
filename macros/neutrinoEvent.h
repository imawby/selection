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
    int m_recoNuVtxNChildren;
    double m_eNu;
    double m_qSqr;
    double m_lepMom;
    double m_lepNuOpeningAngle;

    int m_selTrackContained;
    int m_selTrackMomentumMethod;

    double m_selTrackRecoEndX;
    double m_selTrackRecoEndY;
    double m_selTrackRecoEndZ;

    double m_selTrackTrueEndX;
    double m_selTrackTrueEndY;
    double m_selTrackTrueEndZ;

    double m_nueRecoENu;
    double m_numuRecoENu;

    double m_nuTrueVertexX;
    double m_nuTrueVertexY;
    double m_nuTrueVertexZ;

    double m_nuRecoVertexX;
    double m_nuRecoVertexY;
    double m_nuRecoVertexZ;

    double m_selTrackPandizzleScore;
    double m_selShowerPandrizzleScore;
    double m_selShowerElectronScore;

    double m_selShowerRecoStartX;
    double m_selShowerRecoStartY;
    double m_selShowerRecoStartZ;
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
    m_selTrackContained(-2),
    m_selTrackMomentumMethod(-2),
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
    m_selShowerRecoStartZ(kDefDouble)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ReadFile(const std::string &inputFileName, NeutrinoEventVector &nuEventVector);

#endif
