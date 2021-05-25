#include "NeutrinoEvent.C"
#include "Selection.C"
#include "Signal.C"

void WriteDeltaCP(const std::string &inputFileName_full, const std::string &outputFileName);
void WriteSensitivity(const NeutrinoEventVector &nuEventVector_full, const std::string &outputFileName);
void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& nueEnergyDistribution);
void FillNumuEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& energyDistribution);
double GetOscWeight(const NeutrinoEvent &nu, const double deltaCP);
double CalculateMinLogLikelihood(TH1D *& energyDistribution_zero, TH1D *& energyDistribution_pi, TH1D *& energyDistribution);
double CalculateLogLikelihood(const double observed, const double expected);

void WriteDeltaCP(const std::string &inputFileName_full, const std::string &outputFileName)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    WriteSensitivity(nuEventVector_full, outputFileName);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void WriteSensitivity(const NeutrinoEventVector &nuEventVector_full, const std::string &outputFileName)
{
    TFile * outputFile = new TFile(outputFileName.c_str(), "RECREATE");

    int nDeltaCPValues = 50;
    double minFactoredDeltaCP = -1.0, maxFactoredDeltaCP = 1.0;
    double factoredDeltaCP[nDeltaCPValues], sigNue[nDeltaCPValues], sigNumu[nDeltaCPValues], sigTotal[nDeltaCPValues];

    int nEnergyBins = 40;
    double minEnergy = 0.0, maxEnergy = 10.0;

    // No CP violation Histograms
    TH1D * nueEnergyDistribution_zero = new TH1D("nueEnergyDistribution_zero", "nueEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, 0.0, nueEnergyDistribution_zero);

    TH1D * nueEnergyDistribution_pi = new TH1D("nueEnergyDistribution_pi", "nueEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, TMath::Pi(), nueEnergyDistribution_pi);

    TH1D * numuEnergyDistribution_zero = new TH1D("numuEnergyDistribution_zero", "numuEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(nuEventVector_full, 0.0, numuEnergyDistribution_zero);

    TH1D * numuEnergyDistribution_pi = new TH1D("numuEnergyDistribution_pi", "numuEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(nuEventVector_full, TMath::Pi(), numuEnergyDistribution_pi);

    double totalCoveredRange = 0;
    double previousSigTotal = -std::numeric_limits<double>::max();

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
        const double logLikelihoodSum(nueLogLikelihood + numuLogLikelihood);

        if (logLikelihoodSum < 0.0)
        {
            std::cout << "ISOBEL YOU HAVE A NEGATIVE LOGLIKELIHOOD SUM" << std::endl;
            throw;
        }

        sigNue[i] = std::sqrt(2.0 * nueLogLikelihood);
        sigNumu[i] = std::sqrt(2.0 * numuLogLikelihood);
        sigTotal[i] = std::sqrt(2.0 * logLikelihoodSum);

        if ((sigTotal[i] > 3.0) && (previousSigTotal > 3.0))
            totalCoveredRange += (2.0 * TMath::Pi()) / nDeltaCPValues;

        previousSigTotal = sigTotal[i];
    }

    std::cout << "3#sigma covered range: " << totalCoveredRange / (2.0 * TMath::Pi()) << std::endl;

    TCanvas * cSensitivityNue = new TCanvas("sensitivityNue", "sensitivityNue");
    TGraph * sensitivityNue = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNue);
    sensitivityNue->SetLineColor(kBlue);
    sensitivityNue->SetTitle("Nue;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNue->Write("sensitivityNue");
    sensitivityNue->Draw("AC*");

    TCanvas * cSensitivityNumu = new TCanvas("sensitivityNumu", "sensitivityNumu");
    TGraph * sensitivityNumu = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNumu);
    sensitivityNumu->SetLineColor(kBlue);
    sensitivityNumu->SetTitle("Numu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNumu->Write("sensitivityNumu");
    sensitivityNumu->Draw("AC*");

    TCanvas * cSensitivityTotal = new TCanvas("sensitivityTotal", "sensitivityTotal");
    TGraph * sensitivityTotal = new TGraph(nDeltaCPValues, factoredDeltaCP, sigTotal);
    sensitivityTotal->SetLineColor(kBlue);
    sensitivityTotal->SetTitle("Total;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityTotal->Write("sensitivityTotal");
    sensitivityTotal->Draw("AC*");




}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& nueEnergyDistribution)
{
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsNueSelected(nu))
            continue; 

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCP)));

        nueEnergyDistribution->Fill(nu.m_nueRecoENu, weight);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillNumuEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& numuEnergyDistribution)
{
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsNumuSelected(nu))
            continue;
        
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCP)));

        numuEnergyDistribution->Fill(nu.m_numuRecoENu, weight);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu, const double deltaCP)
{
    /*
    // TO MATCH CAFANA
    double DEF_SIN2THETA12 = 0.27926180; //0.310;
    double DEF_SIN2THETA13 = 0.021799994; //0.02240;
    double DEF_SIN2THETA23 = 0.51199968; //0.582;
    double DEF_DLITTLEM2 = 7.53e-5; //7.39e-5;
    double DEF_DBIGM2 = 0.002444; //2.525e-3;
    */
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

        double chi2_zero = CalculateLogLikelihood(observed_zero, expected);
        double chi2_pi = CalculateLogLikelihood(observed_pi, expected);

        chi2Sum_zero += chi2_zero;
        chi2Sum_pi += chi2_pi;
    }

    if (chi2Sum_zero < 0.0)
    {
        std::cout << "NEGATIVE" << std::endl;
        return 0;
    }


    if (chi2Sum_pi < 0.0)
    {
        std::cout << "NEGATIVE" << std::endl;
        return 0;
    }

    return std::min(chi2Sum_zero, chi2Sum_pi);
}

//------------------------------------------------------------------------------------------------------------------------------------------

double CalculateLogLikelihood(const double observed, const double expected)
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
