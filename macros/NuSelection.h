#include "TH1D.h" 

//NuFit Nov2018 with SK atmospheric
double DEF_SIN2THETA12 = 0.310;
double DEF_SIN2THETA13 = 0.02240;
double DEF_SIN2THETA23 = 0.582;
double DEF_DLITTLEM2 = 7.39e-5;
double DEF_DBIGM2 = 2.525e-3;
double DEF_DELTA = 0;

class TH1D;

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
void InitialiseNuSelectionHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix);
void InitialiseNuSelectionHistograms(NuSelectionHistograms &nuSelectionHistograms, const std::string &histPrefix, const unsigned int nBins, const float xMin, const float xMax);
void PerformSelection(const NeutrinoEventVector &nuVector, NuSelectionHistogramCollection &nueSelectionHistogramCollection, NuSelectionHistogramCollection &numuSelectionHistogramCollection);
void ProcessHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection);
void ProcessHistograms(NuSelectionHistograms &nuSelectionHistograms);
void DrawHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix);
double GetOscWeight(const NeutrinoEvent &nu);

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
