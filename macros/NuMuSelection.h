#include "TH1D.h" 

constexpr int kDefInt = -9999;
constexpr int kDefDouble = (double)(kDefInt);

//NuFit Nov2018 with SK atmospheric
double DEF_SIN2THETA12 = 0.310;
double DEF_SIN2THETA13 = 0.02240;
double DEF_SIN2THETA23 = 0.582;
double DEF_DLITTLEM2 = 7.39e-5;
double DEF_DBIGM2 = 2.525e-3;
double DEF_DELTA = 0;

class TH1D;

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

    double m_nueRecoENu;
    double m_numuRecoENu;

    double m_nuRecoVertexX;
    double m_nuRecoVertexY;
    double m_nuRecoVertexZ;

    double m_selTrackPandizzleScore;
    double m_selShowerElectronScore;

    double m_selShowerRecoStartX;
    double m_selShowerRecoStartY;
    double m_selShowerRecoStartZ;
};

typedef std::vector<NeutrinoEvent> NeutrinoEventVector;

//------------------------------------------------------------------------------------------------------------------------------------------

class NuSelectionHistograms
{
public:
    NuSelectionHistograms();

    TH1D *m_signal;
    TH1D *m_background;
    TH1D *m_selected;
    TH1D *m_signalSelected;
    TH1D *m_backgroundRejected;
    TH1D *m_efficiency;
    TH1D *m_purity;
    TH1D *m_backgroundRejection;
};

//------------------------------------------------------------------------------------------------------------------------------------------

class NuSelectionHistogramCollection
{
public:
    NuSelectionHistogramCollection();

    NuSelectionHistograms m_eNuHists;
    NuSelectionHistograms m_qSqrHists;
    NuSelectionHistograms m_lepMomHists;
    NuSelectionHistograms m_lepNuOpeningAngleHists;
};

//------------------------------------------------------------------------------------------------------------------------------------------

void NuSelection(const std::string &inputFile);
void ReadFile(const std::string &inputFile, NeutrinoEventVector &neutrinoEventVector);
void InitialiseNuSelectionHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix);
void InitialiseNuSelectionHistograms(NuSelectionHistograms &nuSelectionHistograms, const std::string &histPrefix, const unsigned int nBins, const float xMin, const float xMax);
bool IsInFiducialVolume(const NeutrinoEvent &neutrinoEvent);
bool IsNueCCSignal(const NeutrinoEvent &nu);
bool PassNueSelection(const NeutrinoEvent &nu);
bool IsNumuCCSignal(const NeutrinoEvent &nu);
bool PassNumuSelection(const NeutrinoEvent &nu);
void PerformSelection(const NeutrinoEventVector &nuVector, NuSelectionHistogramCollection &nueSelectionHistogramCollection, NuSelectionHistogramCollection &numuSelectionHistogramCollection);
void ProcessHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection);
void ProcessHistograms(NuSelectionHistograms &nuSelectionHistograms);
double GetOscWeight(const NeutrinoEvent &nu);

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
    m_eNu(kDefDouble),
    m_qSqr(kDefDouble),
    m_lepMom(kDefDouble),
    m_lepNuOpeningAngle(kDefDouble),
    m_nueRecoENu(kDefDouble),
    m_numuRecoENu(kDefDouble),
    m_nuRecoVertexX(kDefDouble),
    m_nuRecoVertexY(kDefDouble),
    m_nuRecoVertexZ(kDefDouble),
    m_selTrackPandizzleScore(kDefDouble),
    m_selShowerElectronScore(kDefDouble),
    m_selShowerRecoStartX(kDefDouble),
    m_selShowerRecoStartY(kDefDouble),
    m_selShowerRecoStartZ(kDefDouble)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

NuSelectionHistograms::NuSelectionHistograms() :
    m_signal(nullptr),
    m_background(nullptr),
    m_selected(nullptr),
    m_signalSelected(nullptr),
    m_backgroundRejected(nullptr),
    m_efficiency(nullptr),
    m_purity(nullptr),
    m_backgroundRejection(nullptr)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

NuSelectionHistogramCollection::NuSelectionHistogramCollection() :
    m_eNuHists(NuSelectionHistograms()),
    m_qSqrHists(NuSelectionHistograms()),
    m_lepMomHists(NuSelectionHistograms()),
    m_lepNuOpeningAngleHists(NuSelectionHistograms())
{
}
