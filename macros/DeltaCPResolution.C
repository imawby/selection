#include "NeutrinoEvent.C"
#include "Selection.C"
#include "Signal.C"

void DeltaCPResolution(const std::string &inputFileName);
void PlotResolution(const NeutrinoEventVector &nuEventVector);
void CreateObservedData(const NeutrinoEventVector &nuEventVector, const double deltaCP, int &sin2Theta23_minimaIndex, TH1D *& nueEnergyDistribution);
void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const bool isNu, const double sin2Theta12, const double sin2Theta13, 
    const double sin2Theta23, const double dLittleM2, const double dBigM2, const double deltaCP, TH1D *& nueEnergyDistribution);
double GetOscWeight(const NeutrinoEvent &nu, const double sin2Theta12, const double sin2Theta13, const double sin2Theta23, const double dLittleM2, 
    const double dBigM2, const double deltaCP);
void FillChiSquaredProfile(const NeutrinoEventVector &nuEventVector, TH1D *& nueEnergyDistribution_observed, const int deltaCPIndex, 
    const int sin2Theta23Index, TTree *& chiSquaredProfileTree);
bool FillChiSquaredProfileTree(const NeutrinoEventVector &nuEventVector, TH1D *& nueEnergyDistribution_observed, const int deltaCPIndex, 
    const int sin2Theta23Index, TTree *& chiSquaredProfileTree);
double CalculateChiSquared(TH1D *& energyDistribution_observed, TH1D *& energyDistribution_expected);
double CalculateChiSquared(const double observed, const double expected);

const bool PERFORM_CVN_SELECTION = true;
const bool CHEAT_ENERGY = false;
const bool IS_FHC = true;

const int N_ENERGY_BINS = 16;
const double MIN_ENERGY = 0.0, MAX_ENERGY = 8.0;
const double ENEGRY_BIN_LIMITS[17] = {0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0, 2.25, 2.50, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 8.0};

const int N_DELTA_CP_VALUES = 50;
const double DELTA_CP_MIN = -1.0 * M_PI;
const double DELTA_CP_MAX = 1.0 * M_PI;
const double DELTA_CP_STEP_SIZE = (DELTA_CP_MAX - DELTA_CP_MIN) / N_DELTA_CP_VALUES;

const int N_SIN2_THETA_23_VALUES = 50;
const double SIN2_THETA_23_MIN = 0.4;
const double SIN2_THETA_23_MAX = 0.6;
const double SIN2_THETA_23_STEP_SIZE = (SIN2_THETA_23_MAX - SIN2_THETA_23_MIN) / N_SIN2_THETA_23_VALUES;

// NU_FIT 4.0 VALUES
const double DEF_SIN2THETA12 = 0.310;
const double DEF_SIN2THETA13 = 0.02240;
const double DEF_SIN2THETA23 = 0.582;
const double DEF_DLITTLEM2 = 7.39e-5; 
const double DEF_DBIGM2 = 2.525e-3; // +ve for NO -ve for IO

// Tree variables - yes they're global variables - sue me
double deltaCP_tree = 9999;
int deltaCPIndex_tree = 9999;
double sin2Theta23_tree = 9999;
int sin2Theta23Index_tree = 9999; 
double chiSquared_tree = 9999;

//------------------------------------------------------------------------------------------------------------------------------------------

void DeltaCPResolution(const std::string &inputFileName)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_CVN_SELECTION ? "CVN" : "DIZZLE") << "\033[31m" << " selection" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "Using " << "\033[33m" << (CHEAT_ENERGY ? "TRUE" : "RECO") << "\033[31m" << " energy to fill spectra" << "\033[0m" << std::endl;
    std::cout << "\033[33m" << "Assuming file is: "<< "\033[33m" << (IS_FHC ? "FHC" : "RHC") << "\033[31m" << " is that correct?" << "\033[0m" << std::endl;

    NeutrinoEventVector nuEventVector;
    ReadFile(inputFileName, nuEventVector);

    PlotResolution(nuEventVector);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void PlotResolution(const NeutrinoEventVector &nuEventVector)
{
    const int nDeltaCPValues = N_DELTA_CP_VALUES + 1;
    const int nTheta23Values = N_SIN2_THETA_23_VALUES + 1;

    TTree * chiSquaredProfileTree = new TTree("T", "ChiSquaredProfileTree");
    chiSquaredProfileTree->Branch("DeltaCP", &deltaCP_tree, "DeltaCP/D");
    chiSquaredProfileTree->Branch("DeltaCPIndex", &deltaCPIndex_tree, "DeltaCPIndex/I");
    chiSquaredProfileTree->Branch("Sin2Theta23", &sin2Theta23_tree, "Sin2Theta23/D");
    chiSquaredProfileTree->Branch("Sin2Theta23Index", &sin2Theta23Index_tree, "Sin2Theta23Index/I");
    chiSquaredProfileTree->Branch("ChiSquared", &chiSquared_tree, "ChiSquared/D");

    // eventually turn this into a loop <-- BUT NOT TODAY!!
    const double deltaCP = 0.1 * M_PI; 

    // Create expected spectra with this delta CP value - obtain used values
    int sin2Theta23_minimaIndex = 9999, deltaCP_minimaIndex = std::floor((deltaCP - DELTA_CP_MIN) / DELTA_CP_STEP_SIZE);

    TH1D * nueEnergyDistribution_observed = new TH1D("nueEnergyDistribution_observed", "nueEnergyDistribution_observed", N_ENERGY_BINS, ENEGRY_BIN_LIMITS);
    CreateObservedData(nuEventVector, deltaCP, sin2Theta23_minimaIndex, nueEnergyDistribution_observed);

    //std::cout << "sin2Theta23_minimaIndex: " << sin2Theta23_minimaIndex << std::endl;
    //std::cout << "deltaCP_minimaIndex: " << deltaCP_minimaIndex << std::endl;

    double checkedValuesTable[nDeltaCPValues][nTheta23Values][1];
    FillChiSquaredProfile(nuEventVector, nueEnergyDistribution_observed, deltaCP_minimaIndex, sin2Theta23_minimaIndex, chiSquaredProfileTree);

    //std::cout << "chiSquaredProfileTree->GetEntries(): " << chiSquaredProfileTree->GetEntries() << std::endl;

    // now lets draw the chi2 profile
    TH2D * hist = new TH2D("jam", "jam", N_DELTA_CP_VALUES + 1, DELTA_CP_MIN-0.0000001, DELTA_CP_MAX+0.0000001, N_SIN2_THETA_23_VALUES + 1, SIN2_THETA_23_MIN-0.0000001, SIN2_THETA_23_MAX+0.0000001);

    float bestAngle(0.0), bestDeltaCP(0.0), lowestChi(10000);

    float lowestDeltaCP(1000), highestDeltaCP(-1000);

    for (int i = 0; i < chiSquaredProfileTree->GetEntries(); ++i)
    {
        chiSquaredProfileTree->GetEntry(i);

        if (chiSquared_tree < 1.0)
        {
            if (deltaCP_tree > highestDeltaCP)
                highestDeltaCP = deltaCP_tree;

            if (deltaCP_tree < lowestDeltaCP)
                lowestDeltaCP = deltaCP_tree;
        }

        if (chiSquared_tree < lowestChi)
        {
            lowestChi = chiSquared_tree;
            bestDeltaCP = deltaCP_tree;
            bestAngle = sin2Theta23_tree;
        }

        hist->Fill(deltaCP_tree, sin2Theta23_tree, chiSquared_tree);
    }

    std::cout << "bestAngle: " << bestAngle << std::endl;
    std::cout << "bestDeltaCP: " << bestDeltaCP << std::endl;
    std::cout << "lowestChi: " << lowestChi << std::endl;

    std::cout << "//////////////////////////" << std::endl;
    std::cout << "lowestDeltaCP: " << lowestDeltaCP << std::endl;
    std::cout << "highestDeltaCP: " << highestDeltaCP << std::endl;
    std::cout << "CP resolution: " << (highestDeltaCP - lowestDeltaCP) << " radians, " << ((highestDeltaCP - lowestDeltaCP) * 180.0 / M_PI) << " degrees" << std::endl;
    std::cout << "//////////////////////////" << std::endl;

    TCanvas * c1 = new TCanvas("c1", "c1");
    hist->SetTitle(";deltaCP;sin2theta23");
    hist->Draw("COLZ"); //text
}

//------------------------------------------------------------------------------------------------------------------------------------------

void CreateObservedData(const NeutrinoEventVector &nuEventVector, const double deltaCP, int &sin2Theta23_minimaIndex, TH1D *& nueEnergyDistribution)
{
    // would do some fancy choosing of oscillation params in the future but for now use NuFit 4.0 params

    FillNueEnergyDistribution(nuEventVector, IS_FHC, DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, deltaCP, 
        nueEnergyDistribution);

    sin2Theta23_minimaIndex = std::floor((DEF_SIN2THETA23 - SIN2_THETA_23_MIN) / SIN2_THETA_23_STEP_SIZE);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const bool isNu, const double sin2Theta12, const double sin2Theta13, 
    const double sin2Theta23, const double dLittleM2, const double dBigM2, const double deltaCP, TH1D *& nueEnergyDistribution)
{
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        bool isNueSelected(PERFORM_CVN_SELECTION ? PassCVNNueSelection(nu) : IsNueSelected(nu, isNu));

        if (!isNueSelected)
            continue; 

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, sin2Theta12, sin2Theta13, sin2Theta23, dLittleM2, dBigM2, deltaCP)));

        nueEnergyDistribution->Fill(CHEAT_ENERGY ? nu.m_eNu : nu.m_nueRecoENu, weight);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu, const double sin2Theta12, const double sin2Theta13, const double sin2Theta23, const double dLittleM2, 
    const double dBigM2, const double deltaCP)
{
    double weight = getProb3ppWeights(sin2Theta12, sin2Theta13, sin2Theta23, dLittleM2, dBigM2, deltaCP, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillChiSquaredProfile(const NeutrinoEventVector &nuEventVector, TH1D *& nueEnergyDistribution_observed, const int deltaCPIndex, 
    const int sin2Theta23Index, TTree *& chiSquaredProfileTree)
{
    for (int deltaCPOffset = -1; deltaCPOffset <= 1; ++deltaCPOffset)
    {
        const int neighbourDeltaCPIndex = deltaCPIndex + deltaCPOffset;

        if ((neighbourDeltaCPIndex < 0) || (neighbourDeltaCPIndex > N_DELTA_CP_VALUES))
            continue;

        for (int sin2Theta23Offset = -1; sin2Theta23Offset <= 1; ++sin2Theta23Offset)
        {
            const int neighbourSin2Theta23Index = sin2Theta23Index + sin2Theta23Offset;

            if ((neighbourSin2Theta23Index < 0) || (neighbourSin2Theta23Index > N_SIN2_THETA_23_VALUES))
                continue;

            // if have already been checked - look in the tree (i hate root)

            TCut deltaCPCut = ("DeltaCPIndex == " + to_string(neighbourDeltaCPIndex)).c_str();
            TCut sin2Theta23Cut = ("Sin2Theta23Index == " + to_string(neighbourSin2Theta23Index)).c_str();

            const int nEntries = chiSquaredProfileTree->Draw("ChiSquared>>hist", deltaCPCut && sin2Theta23Cut,"goff");

            if (nEntries != 0)
                continue;

            // get chi-squared
            if (FillChiSquaredProfileTree(nuEventVector, nueEnergyDistribution_observed, neighbourDeltaCPIndex, neighbourSin2Theta23Index, chiSquaredProfileTree))
                FillChiSquaredProfile(nuEventVector, nueEnergyDistribution_observed, neighbourDeltaCPIndex, neighbourSin2Theta23Index, chiSquaredProfileTree);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool FillChiSquaredProfileTree(const NeutrinoEventVector &nuEventVector, TH1D *& nueEnergyDistribution_observed, const int deltaCPIndex, 
    const int sin2Theta23Index, TTree *& chiSquaredProfileTree)
{
    const double deltaCP = (deltaCPIndex * DELTA_CP_STEP_SIZE) + DELTA_CP_MIN;
    const double sin2Theta23 = (sin2Theta23Index * SIN2_THETA_23_STEP_SIZE) + SIN2_THETA_23_MIN;

    TH1D * nueEnergyDistribution_expected = new TH1D("nueEnergyDistribution_expected", "nueEnergyDistribution_expected", N_ENERGY_BINS, ENEGRY_BIN_LIMITS);
    
    FillNueEnergyDistribution(nuEventVector, IS_FHC, DEF_SIN2THETA12, DEF_SIN2THETA13, sin2Theta23, DEF_DLITTLEM2, DEF_DBIGM2, deltaCP,
        nueEnergyDistribution_expected);

    const double chiSquared = CalculateChiSquared(nueEnergyDistribution_observed, nueEnergyDistribution_expected);

    delete nueEnergyDistribution_expected;


    if (chiSquared > 1.f)
        return false;

    // Fill tree
    deltaCP_tree = deltaCP;
    deltaCPIndex_tree = deltaCPIndex;
    sin2Theta23_tree = sin2Theta23;
    sin2Theta23Index_tree = sin2Theta23Index;
    chiSquared_tree = chiSquared;
    chiSquaredProfileTree->Fill();

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double CalculateChiSquared(TH1D *& energyDistribution_observed, TH1D *& energyDistribution_expected)
{
    double chi2Sum(0.0);

    for (int bin = 1; bin <= energyDistribution_observed->GetXaxis()->GetNbins(); ++bin)
    {
        const double observed(energyDistribution_observed->GetBinContent(bin));
        const double expected(energyDistribution_expected->GetBinContent(bin));

        double chi2 = CalculateChiSquared(observed, expected);

        chi2Sum += chi2;
    }

    return (chi2Sum < 0.0 ? 0.0 : chi2Sum);
}

//------------------------------------------------------------------------------------------------------------------------------------------

double CalculateChiSquared(const double observed, const double expected)
{
    if (expected < numeric_limits<double>::epsilon())
        return 0.0;

    const double chi2 = ((expected - observed + (observed * log(observed / expected))));

    if (TMath::IsNaN(chi2))
    {
        std::cout << "ISOBEL YOU HAVE A NAN" << std::endl;
        throw;
    }

    return chi2;
}

//------------------------------------------------------------------------------------------------------------------------------------------
