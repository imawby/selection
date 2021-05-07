#include "NeutrinoEvent.C"
#include "Selection.C"
#include "Signal.C"

void DeltaCP(const std::string &inputFileName_full);
void PlotSensitivity(const NeutrinoEventVector &nuEventVector_full);
void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& nueEnergyDistribution);
void FillNumuEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& energyDistribution);
double GetOscWeight(const NeutrinoEvent &nu, const double deltaCP);
double CalculateMinLogLikelihood(TH1D *& energyDistribution_zero, TH1D *& energyDistribution_pi, TH1D *& energyDistribution);
double CalculateLogLikelihood(const double observed, const double expected);

void DeltaCP(const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    PlotSensitivity(nuEventVector_full);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void PlotSensitivity(const NeutrinoEventVector &nuEventVector_full)
{
    int nDeltaCPValues = 51;
    double minFactoredDeltaCP = -1.0, maxFactoredDeltaCP = 1.0;
    double factoredDeltaCP[nDeltaCPValues], chi2[nDeltaCPValues];

    int nEnergyBins = 40;
    double minEnergy = -1.0, maxEnergy = 1.0;

    // No CP violation Histograms
    TH1D * nueEnergyDistribution_zero = new TH1D("nueEnergyDistribution_zero", "nueEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, 0.0, nueEnergyDistribution_zero);

    TH1D * nueEnergyDistribution_pi = new TH1D("nueEnergyDistribution_pi", "nueEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, TMath::Pi(), nueEnergyDistribution_pi);

    TH1D * numuEnergyDistribution_zero = new TH1D("numuEnergyDistribution_zero", "numuEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(nuEventVector_full, 0.0, numuEnergyDistribution_zero);

    //TH1D * numuEnergyDistribution_pi = new TH1D("numuEnergyDistribution_pi", "numuEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    //FillNumuEnergyDistribution(nuEventVector_full, TMath::Pi(), numuEnergyDistribution_pi);

    /*
    TCanvas * nue = new TCanvas("nue", "nue");
    nueEnergyDistribution_pi->SetLineColor(kBlue);
    nueEnergyDistribution_zero->Draw("hist");
    nueEnergyDistribution_pi->Draw("hist same");

    TCanvas * numu = new TCanvas("numu", "numu");
    numuEnergyDistribution_pi->SetLineColor(kBlue);
    numuEnergyDistribution_zero->Draw("hist");
    numuEnergyDistribution_pi->Draw("hist same");
    */
    /*
    for (int i = 0; i < nDeltaCPValues; ++i)
    {
        factoredDeltaCP[i] = minFactoredDeltaCP + ((maxFactoredDeltaCP - minFactoredDeltaCP) * i / nDeltaCPValues);
        double deltaCP = factoredDeltaCP[i] * TMath::Pi();

        std::cout << "deltaCP: " << deltaCP << std::endl;

        TH1D * nueEnergyDistribution = new TH1D(("nueEnergyDistribution" + to_string(deltaCP)).c_str(), ("nueEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNueEnergyDistribution(nuEventVector_full, deltaCP, nueEnergyDistribution);

        TH1D * numuEnergyDistribution = new TH1D(("numuEnergyDistribution" + to_string(deltaCP)).c_str(), ("numuEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNumuEnergyDistribution(nuEventVector_full, deltaCP, numuEnergyDistribution);

        const double nueLogLikelihood(CalculateMinLogLikelihood(nueEnergyDistribution_zero, nueEnergyDistribution_pi, nueEnergyDistribution));
        const double numuLogLikelihood(CalculateMinLogLikelihood(numuEnergyDistribution_zero, numuEnergyDistribution_pi, numuEnergyDistribution));

        double jam = std::sqrt(2.0 * (nueLogLikelihood + numuLogLikelihood));

        std::cout << "chi2: " << jam << std::endl;

        chi2[i] = jam;
    }

    TGraph * sensitivityGraph = new TGraph(nDeltaCPValues, factoredDeltaCP, chi2);

    TCanvas * sensitivity = new TCanvas("sensitivity", "sensitivity");
    sensitivityGraph->SetLineColor(kBlue);
    sensitivityGraph->SetTitle(";delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityGraph->Draw("AC*");
    */
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& nueEnergyDistribution)
{
    std::cout << nuEventVector_full.size() << std::endl;
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        /*
        if (!IsNueSelected(nu))
            continue; 
        */

        if (!IsNueCCSignal(nu))
            continue;
        
        //const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCP)) * 2.0);
        const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu, deltaCP)); //<-- WRONG WEIGHTING


        nueEnergyDistribution->Fill((nu.m_nueRecoENu - nu.m_eNu), weight);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillNumuEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& numuEnergyDistribution)
{
    std::cout << nuEventVector_full.size() << std::endl;

    const double minX(-360.0 + 20.0), maxX(360.0 - 20.0);
    const double minY(-600.0 + 20.0), maxY(600.0 - 20.0);
    const double minZ(20.0), maxZ(1394.0 - 120.0);

    TH3D * trueEnd = new TH3D("j", "j", 100, -720, 720, 100, -1200, 1200, 100, 0, 2800);
    TH3D * recoEnd = new TH3D("t", "t", 100, -720, 720, 100, -1200, 1200, 100, 0, 2800);

    TH2D * res = new TH2D("n", "n", 100, 0, 100, 100, -1.0, 1.0);


    int count (0);
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        /*
        if (!IsNumuSelected(nu))
            continue;
        */
        if (!IsNumuCCSignal(nu))
            continue;
        
        if (!nu.m_selTrackContained)
            continue;
        
        //const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCP) * 2.0));
        const double weight(nu.m_projectedPOTWeight * GetOscWeight(nu, deltaCP)); //<-- WRONG WEIGHTING

        std::cout << "////////////////////////" << std::endl;
        std::cout << "nu.m_projectedPOTWeight: "<< nu.m_projectedPOTWeight << std::endl;
        std::cout << "GetOscWeight(nu, deltaCP): " << GetOscWeight(nu, deltaCP) << std::endl;
        std::cout << "nu.m_projectedPOTWeight * GetOscWeight(nu, deltaCP): " << weight << std::endl;

        float xDiff(nu.m_selTrackTrueEndX - nu.m_selTrackRecoEndX);
        float yDiff(nu.m_selTrackTrueEndY - nu.m_selTrackRecoEndY);
        float zDiff(nu.m_selTrackTrueEndZ - nu.m_selTrackRecoEndZ);

        double diff = std::sqrt(xDiff*xDiff + yDiff*yDiff + zDiff*zDiff);

        numuEnergyDistribution->Fill((nu.m_numuRecoENu - nu.m_eNu) / nu.m_eNu, weight);



    if ((nu.m_selTrackRecoEndX < minX) || (nu.m_selTrackRecoEndX > maxX))
        recoEnd->Fill(nu.m_selTrackRecoEndX, nu.m_selTrackRecoEndY, nu.m_selTrackRecoEndZ);

    else if ((nu.m_selTrackRecoEndY < minY) || (nu.m_selTrackRecoEndY > maxY))
        recoEnd->Fill(nu.m_selTrackRecoEndX, nu.m_selTrackRecoEndY, nu.m_selTrackRecoEndZ);

    else if ((nu.m_selTrackRecoEndZ < minZ) || (nu.m_selTrackRecoEndZ > maxZ))
        recoEnd->Fill(nu.m_selTrackRecoEndX, nu.m_selTrackRecoEndY, nu.m_selTrackRecoEndZ);
    else
        ++count;

        trueEnd->Fill(nu.m_selTrackTrueEndX, nu.m_selTrackTrueEndY, nu.m_selTrackTrueEndZ);
        res->Fill(diff, (nu.m_numuRecoENu - nu.m_eNu) / nu.m_eNu);
    }

    std::cout << "count: " << count << std::endl;

    //trueEnd->Draw();
    recoEnd->SetLineColor(kRed);
    recoEnd->Draw();
    //res->Draw("colz");
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu, const double deltaCP)
{
    double DEF_SIN2THETA12 = 0.310;
    double DEF_SIN2THETA13 = 0.02240;
    double DEF_SIN2THETA23 = 0.582;
    double DEF_DLITTLEM2 = 7.39e-5;
    double DEF_DBIGM2 = 2.525e-3;

    double weight = getProb3ppWeights(DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, deltaCP, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}

//------------------------------------------------------------------------------------------------------------------------------------------

double CalculateMinLogLikelihood(TH1D *& energyDistribution_zero, TH1D *& energyDistribution_pi, TH1D *& energyDistribution)
{
    double chi2Sum_zero(0.0);
    double chi2Sum_pi(0.0);

    for (int i = 1; i <= energyDistribution_zero->GetXaxis()->GetNbins(); ++i)
    {
        const unsigned int bin(i);
        const double observed_zero(energyDistribution_zero->GetBinContent(bin));
        const double observed_pi(energyDistribution_pi->GetBinContent(bin));
        const double expected(energyDistribution->GetBinContent(bin));

        /*
        std::cout << "observed_zero: " << observed_zero << std::endl;
        std::cout << "observed_pi: " << observed_pi << std::endl;
        std::cout << "expected: " << expected << std::endl;
        */

        double chi2_zero = CalculateLogLikelihood(observed_zero, expected);
        double chi2_pi = CalculateLogLikelihood(observed_pi, expected);

        chi2Sum_zero += chi2_zero;
        chi2Sum_pi += chi2_pi;
    }

    if (chi2Sum_zero < 0.0)
        return 0.0;

    if (chi2Sum_pi < 0.0)
        return 0.0;


    return std::min(chi2Sum_zero, chi2Sum_pi);
}

//------------------------------------------------------------------------------------------------------------------------------------------

double CalculateLogLikelihood(const double observed, const double expected)
{
    if (expected < numeric_limits<double>::epsilon())
        return 0.0;

    const double chi2 = ((expected - observed + (observed * log(observed / expected))));

    if (TMath::IsNaN(chi2))
        std::cout << "ISOBEL YOU HAVE A NAN" << std::endl;

    return chi2;
}

//------------------------------------------------------------------------------------------------------------------------------------------
