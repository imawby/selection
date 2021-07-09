#include "NeutrinoEvent.C"
#include "Selection.C"
#include "Signal.C"

void DeltaCP(const std::string &inputFileName_full, const std::string &antiInputFileName_full, const std::string &outputFile);
void PlotSensitivity(const NeutrinoEventVector &nuEventVector_full);
void PlotSensitivity(const NeutrinoEventVector &nuEventVector_full, const NeutrinoEventVector &anuEventVector_full, const std::string &outputFile);
void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& nueEnergyDistribution, bool isNu);
void FillNumuEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& energyDistribution, bool isNu);
double GetOscWeight(const NeutrinoEvent &nu, const double deltaCP);
double CalculateMinLogLikelihood(TH1D *& energyDistribution_zero, TH1D *& energyDistribution_pi, TH1D *& energyDistribution);
double CalculateLogLikelihood(const double observed, const double expected);

const bool PERFORM_CVN_SELECTION = false;
const bool CHEAT_ENERGY = false;

void DeltaCP(const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    PlotSensitivity(nuEventVector_full);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DeltaCP(const std::string &inputFileName_full, const std::string &antiInputFileName_full, const std::string &outputFile)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_CVN_SELECTION ? "CVN " : "DIZZLE ") << "\033[31m" << "selection" << "\033[0m" << std::endl;
    std::cout << "\033[31m" << "Using " << "\033[33m" << (CHEAT_ENERGY ? "TRUE " : "RECO ") << "\033[31m" << "energy to fill spectra" << "\033[0m" << std::endl;

    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    NeutrinoEventVector anuEventVector_full;
    ReadFile(antiInputFileName_full, anuEventVector_full);

    PlotSensitivity(nuEventVector_full, anuEventVector_full, outputFile);
}

//------------------------------------------------------------------------------------------------------------------------------------------
// THIS IS JUST FOR FHC!!!
void PlotSensitivity(const NeutrinoEventVector &nuEventVector_full)
{
    int nDeltaCPValues = 50;
    double minFactoredDeltaCP = -1.0, maxFactoredDeltaCP = 1.0;
    double factoredDeltaCP[nDeltaCPValues], sigNue[nDeltaCPValues], sigNumu[nDeltaCPValues], sigTotal[nDeltaCPValues];

    int nEnergyBins = 40;
    double minEnergy = 0.0, maxEnergy = 10.0;

    // No CP violation Histograms
    TH1D * nueEnergyDistribution_zero = new TH1D("nueEnergyDistribution_zero", "nueEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, 0.0, nueEnergyDistribution_zero, true);

    TH1D * nueEnergyDistribution_pi = new TH1D("nueEnergyDistribution_pi", "nueEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, TMath::Pi(), nueEnergyDistribution_pi, true);

    TH1D * numuEnergyDistribution_zero = new TH1D("numuEnergyDistribution_zero", "numuEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(nuEventVector_full, 0.0, numuEnergyDistribution_zero, true);

    TH1D * numuEnergyDistribution_pi = new TH1D("numuEnergyDistribution_pi", "numuEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(nuEventVector_full, TMath::Pi(), numuEnergyDistribution_pi, true);

    double totalCoveredRange = 0;
    double previousSigTotal = -std::numeric_limits<double>::max();

    for (int i = 1; i < numuEnergyDistribution_zero->GetXaxis()->GetNbins(); ++i)
        std::cout << "i: " << i << ", entries: " << numuEnergyDistribution_zero->GetBinContent(i) << std::endl;
    
    for (int i = 0; i < nDeltaCPValues; ++i)
    {
        factoredDeltaCP[i] = minFactoredDeltaCP + ((maxFactoredDeltaCP - minFactoredDeltaCP) * i / nDeltaCPValues);
        double deltaCP = factoredDeltaCP[i] * TMath::Pi();

        std::cout << "deltaCP: " << deltaCP << std::endl;

        TH1D * nueEnergyDistribution = new TH1D(("nueEnergyDistribution" + to_string(deltaCP)).c_str(), ("nueEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNueEnergyDistribution(nuEventVector_full, deltaCP, nueEnergyDistribution, true);

        TH1D * numuEnergyDistribution = new TH1D(("numuEnergyDistribution" + to_string(deltaCP)).c_str(), ("numuEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNumuEnergyDistribution(nuEventVector_full, deltaCP, numuEnergyDistribution, true);

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

        std::cout << "delta CP: " << deltaCP << ", sensitivity: " << sigTotal[i] << std::endl;

        previousSigTotal = sigTotal[i];
    }

    std::cout << "covered range: " << totalCoveredRange / (2.0 * TMath::Pi()) << std::endl;

    TCanvas * test = new TCanvas("test", "test");
    numuEnergyDistribution_zero->Draw("hist");

    TCanvas * cSensitivityNue = new TCanvas("sensitivityNue", "sensitivityNue");
    TGraph * sensitivityNue = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNue);
    sensitivityNue->SetLineColor(kBlue);
    sensitivityNue->SetTitle("Nue;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNue->Draw("AC*");

    TCanvas * cSensitivityNumu = new TCanvas("sensitivityNumu", "sensitivityNumu");
    TGraph * sensitivityNumu = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNumu);
    sensitivityNumu->SetLineColor(kBlue);
    sensitivityNumu->SetTitle("Numu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNumu->Draw("AC*");

    TCanvas * cSensitivityTotal = new TCanvas("sensitivityTotal", "sensitivityTotal");
    TGraph * sensitivityTotal = new TGraph(nDeltaCPValues, factoredDeltaCP, sigTotal);
    sensitivityTotal->SetLineColor(kBlue);
    sensitivityTotal->SetTitle("Total;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityTotal->Draw("AC*");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void PlotSensitivity(const NeutrinoEventVector &nuEventVector_full, const NeutrinoEventVector &anuEventVector_full, const std::string &outputFileName)
{
    TFile * outputFile = new TFile(outputFileName.c_str(), "CREATE");

    //int nDeltaCPValues = 50;
    int nDeltaCPValues = 200;
    double minFactoredDeltaCP = -1.0, maxFactoredDeltaCP = 1.0;
    double factoredDeltaCP[nDeltaCPValues];

    double sigNue[nDeltaCPValues], sigAnue[nDeltaCPValues], sigNumu[nDeltaCPValues], sigAnumu[nDeltaCPValues];

    double sigNuSum[nDeltaCPValues], sigAnuSum[nDeltaCPValues];

    double sigNueSum[nDeltaCPValues], sigNumuSum[nDeltaCPValues];
    double sigNueCombined[nDeltaCPValues], sigNumuCombined[nDeltaCPValues];

    double sigSum[nDeltaCPValues], sigCombined[nDeltaCPValues];

    int nEnergyBins = 40;
    double minEnergy = 0.0, maxEnergy = 10.0;

    // deltaCP = 0 histograms
    TH1D * nueEnergyDistribution_zero = new TH1D("nueEnergyDistribution_zero", "nueEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, 0.0, nueEnergyDistribution_zero, true);

    TH1D * anueEnergyDistribution_zero = new TH1D("anueEnergyDistribution_zero", "anueEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(anuEventVector_full, 0.0, anueEnergyDistribution_zero, false);

    TH1D * numuEnergyDistribution_zero = new TH1D("numuEnergyDistribution_zero", "numuEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(nuEventVector_full, 0.0, numuEnergyDistribution_zero, true);

    TH1D * anumuEnergyDistribution_zero = new TH1D("anumuEnergyDistribution_zero", "anumuEnergyDistribution_zero", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(anuEventVector_full, 0.0, anumuEnergyDistribution_zero, false);

    //TH1D * combinedNueEnergyDistribution_zero = (TH1D*)nueEnergyDistribution_zero->Clone();
    //combinedNueEnergyDistribution_zero->Add(anueEnergyDistribution_zero);

    //TH1D * combinedNumuEnergyDistribution_zero = (TH1D*)numuEnergyDistribution_zero->Clone();
    //combinedNumuEnergyDistribution_zero->Add(anumuEnergyDistribution_zero);

    //TH1D * combinedEnergyDistribution_zero = (TH1D*)nueEnergyDistribution_zero->Clone();
    //combinedEnergyDistribution_zero->Add(anueEnergyDistribution_zero);
    //combinedEnergyDistribution_zero->Add(numuEnergyDistribution_zero);
    //combinedEnergyDistribution_zero->Add(anumuEnergyDistribution_zero);

    // deltaCP = pi histograms
    TH1D * nueEnergyDistribution_pi = new TH1D("nueEnergyDistribution_pi", "nueEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(nuEventVector_full, TMath::Pi(), nueEnergyDistribution_pi, true);

    TH1D * anueEnergyDistribution_pi = new TH1D("anueEnergyDistribution_pi", "anueEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNueEnergyDistribution(anuEventVector_full, TMath::Pi(), anueEnergyDistribution_pi, false);

    TH1D * numuEnergyDistribution_pi = new TH1D("numuEnergyDistribution_pi", "numuEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(nuEventVector_full, TMath::Pi(), numuEnergyDistribution_pi, true);

    TH1D * anumuEnergyDistribution_pi = new TH1D("anumuEnergyDistribution_pi", "anumuEnergyDistribution_pi", nEnergyBins, minEnergy, maxEnergy);
    FillNumuEnergyDistribution(anuEventVector_full, TMath::Pi(), anumuEnergyDistribution_pi, false);

    //TH1D * combinedNueEnergyDistribution_pi = (TH1D*)nueEnergyDistribution_pi->Clone();
    //combinedNueEnergyDistribution_pi->Add(anueEnergyDistribution_pi);

    //TH1D * combinedNumuEnergyDistribution_pi = (TH1D*)numuEnergyDistribution_pi->Clone();
    //combinedNumuEnergyDistribution_pi->Add(anumuEnergyDistribution_pi);

    //TH1D * combinedEnergyDistribution_pi = (TH1D*)nueEnergyDistribution_pi->Clone();
    //combinedEnergyDistribution_pi->Add(anueEnergyDistribution_pi);
    //combinedEnergyDistribution_pi->Add(numuEnergyDistribution_pi);
    //combinedEnergyDistribution_pi->Add(anumuEnergyDistribution_pi);

    double totalCoveredRangeSum_1(0.0), totalCoveredRangeSum_2(0.0), totalCoveredRangeSum_3(0.0), totalCoveredRangeSum_4(0.0), totalCoveredRangeSum_5(0.0);
    double totalCoveredRangeSum = 0.0, totalCoveredRangeCombined = 0.0;
    double previousSigSum = -std::numeric_limits<double>::max(), previousSigCombined = -std::numeric_limits<double>::max();

    for (int i = 0; i < nDeltaCPValues; ++i)
    {
        factoredDeltaCP[i] = minFactoredDeltaCP + ((maxFactoredDeltaCP - minFactoredDeltaCP) * i / nDeltaCPValues);
        double deltaCP = factoredDeltaCP[i] * TMath::Pi();

        TH1D * nueEnergyDistribution = new TH1D(("nueEnergyDistribution" + to_string(deltaCP)).c_str(), ("nueEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNueEnergyDistribution(nuEventVector_full, deltaCP, nueEnergyDistribution, true);

        TH1D * anueEnergyDistribution = new TH1D(("anueEnergyDistribution" + to_string(deltaCP)).c_str(), ("anueEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNueEnergyDistribution(anuEventVector_full, deltaCP, anueEnergyDistribution, false);

        TH1D * numuEnergyDistribution = new TH1D(("numuEnergyDistribution" + to_string(deltaCP)).c_str(), ("numuEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNumuEnergyDistribution(nuEventVector_full, deltaCP, numuEnergyDistribution, true);

        TH1D * anumuEnergyDistribution = new TH1D(("anumuEnergyDistribution" + to_string(deltaCP)).c_str(), ("anumuEnergyDistribution" + to_string(deltaCP)).c_str(), nEnergyBins, minEnergy, maxEnergy);
        FillNumuEnergyDistribution(anuEventVector_full, deltaCP, anumuEnergyDistribution, false);

        //TH1D * combinedNueEnergyDistribution = (TH1D*)nueEnergyDistribution->Clone();
        //combinedNueEnergyDistribution->Add(anueEnergyDistribution);

        //TH1D * combinedNumuEnergyDistribution = (TH1D*)numuEnergyDistribution->Clone();
        //combinedNumuEnergyDistribution->Add(anumuEnergyDistribution);

        //TH1D * combinedEnergyDistribution = (TH1D*)nueEnergyDistribution->Clone();
        //combinedEnergyDistribution->Add(anueEnergyDistribution);
        //combinedEnergyDistribution->Add(numuEnergyDistribution);
        //combinedEnergyDistribution->Add(anumuEnergyDistribution);

        const double nueLogLikelihood(CalculateMinLogLikelihood(nueEnergyDistribution_zero, nueEnergyDistribution_pi, nueEnergyDistribution));
        const double anueLogLikelihood(CalculateMinLogLikelihood(anueEnergyDistribution_zero, anueEnergyDistribution_pi, anueEnergyDistribution));
        const double numuLogLikelihood(CalculateMinLogLikelihood(numuEnergyDistribution_zero, numuEnergyDistribution_pi, numuEnergyDistribution));
        const double anumuLogLikelihood(CalculateMinLogLikelihood(anumuEnergyDistribution_zero, anumuEnergyDistribution_pi, anumuEnergyDistribution));

        const double logLikelihoodNuSum(nueLogLikelihood + numuLogLikelihood);
        const double logLikelihoodAnuSum(anueLogLikelihood + anumuLogLikelihood);

        const double logLikelihoodNueSum(nueLogLikelihood + anueLogLikelihood);
        const double logLikelihoodNumuSum(numuLogLikelihood + anumuLogLikelihood);
        //const double logLikelihoodNueCombined(CalculateMinLogLikelihood(combinedNueEnergyDistribution_zero, combinedNueEnergyDistribution_pi, combinedNueEnergyDistribution));
        //const double loglikelihoodNumuCombined(CalculateMinLogLikelihood(combinedNumuEnergyDistribution_zero, combinedNumuEnergyDistribution_pi, combinedNumuEnergyDistribution));

        const double logLikelihoodSum(logLikelihoodNuSum + logLikelihoodAnuSum);
        //const double logLikelihoodSum(anueLogLikelihood);
        //const double combinedLogLikelihood(CalculateMinLogLikelihood(combinedEnergyDistribution_zero, combinedEnergyDistribution_pi, combinedEnergyDistribution));

        if (logLikelihoodSum < 0.0)
        {
            std::cout << "ISOBEL YOU HAVE A NEGATIVE LOGLIKELIHOOD SUM" << std::endl;
            throw;
        }

        sigNue[i] = std::sqrt(2.0 * nueLogLikelihood);
        sigAnue[i] = std::sqrt(2.0 * anueLogLikelihood);
        sigNumu[i] = std::sqrt(2.0 * numuLogLikelihood);
        sigAnumu[i] = std::sqrt(2.0 * anumuLogLikelihood);

        sigNueSum[i] = std::sqrt(2.0 * logLikelihoodNueSum);
        sigNumuSum[i] = std::sqrt(2.0 * logLikelihoodNumuSum);
        //sigNueCombined[i] = std::sqrt(2.0 * logLikelihoodNueCombined);
        //sigNumuCombined[i] = std::sqrt(2.0 * loglikelihoodNumuCombined);

        sigNuSum[i] = std::sqrt(2.0 * logLikelihoodNuSum);
        sigAnuSum[i] = std::sqrt(2.0 * logLikelihoodAnuSum);

        sigSum[i] = std::sqrt(2.0 * logLikelihoodSum);
        //sigCombined[i] = std::sqrt(2.0 * combinedLogLikelihood);

        if ((sigSum[i] > 1.0) && (previousSigSum > 1.0))
            totalCoveredRangeSum_1 += (2.0 * TMath::Pi()) / nDeltaCPValues;

        if ((sigSum[i] > 2.0) && (previousSigSum > 2.0))
            totalCoveredRangeSum_2 += (2.0 * TMath::Pi()) / nDeltaCPValues;

        if ((sigSum[i] > 3.0) && (previousSigSum > 3.0))
            totalCoveredRangeSum_3 += (2.0 * TMath::Pi()) / nDeltaCPValues;

        if ((sigSum[i] > 4.0) && (previousSigSum > 4.0))
            totalCoveredRangeSum_4 += (2.0 * TMath::Pi()) / nDeltaCPValues;

        if ((sigSum[i] > 5.0) && (previousSigSum > 5.0))
            totalCoveredRangeSum_5 += (2.0 * TMath::Pi()) / nDeltaCPValues;

        //if ((sigCombined[i] > 3.0) && (previousSigCombined > 3.0))
        //totalCoveredRangeCombined += (2.0 * TMath::Pi()) / nDeltaCPValues;

        std::cout << "delta CP: " << deltaCP << ", sensitivity (sum): " << sigSum[i] << std::endl;
        //std::cout << "delta CP: " << deltaCP << ", sensitivity (combined): " << sigCombined[i] << std::endl;

        previousSigSum = sigSum[i];
        //previousSigCombined = sigCombined[i];
    }

    std::cout << "1 sigma covered range (sum): " << totalCoveredRangeSum_1 / (2.0 * TMath::Pi()) << std::endl;
    std::cout << "2 sigma covered range (sum): " << totalCoveredRangeSum_2 / (2.0 * TMath::Pi()) << std::endl;
    std::cout << "3 sigma covered range (sum): " << totalCoveredRangeSum_3 / (2.0 * TMath::Pi()) << std::endl;
    std::cout << "4 sigma covered range (sum): " << totalCoveredRangeSum_4 / (2.0 * TMath::Pi()) << std::endl;
    std::cout << "5 sigma covered range (sum): " << totalCoveredRangeSum_5 / (2.0 * TMath::Pi()) << std::endl;

    //std::cout << "covered range (combined): " << totalCoveredRangeCombined / (2.0 * TMath::Pi()) << std::endl;
    
    TCanvas * cSensitivityNu = new TCanvas("cSensitivityNu", "cSensitivityNu");
    cSensitivityNu->Divide(3, 1);
    cSensitivityNu->cd(1);
    TGraph * sensitivityNue = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNue);
    sensitivityNue->SetLineColor(kBlue);
    sensitivityNue->SetTitle("Nue;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNue->Draw("AC*");
    cSensitivityNu->cd(2);
    TGraph * sensitivityNumu = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNumu);
    sensitivityNumu->SetLineColor(kBlue);
    sensitivityNumu->SetTitle("Numu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNumu->Draw("AC*");
    cSensitivityNu->cd(3);
    TGraph * sensitivityNu = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNuSum);
    sensitivityNu->SetLineColor(kBlue);
    sensitivityNu->SetTitle("Nue + Numu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNu->Draw("AC*");

    TCanvas * cSensitivityAnu = new TCanvas("cSensitivityAnu", "cSensitivityAnu");
    cSensitivityAnu->Divide(3, 1);
    cSensitivityAnu->cd(1);
    TGraph * sensitivityAnue = new TGraph(nDeltaCPValues, factoredDeltaCP, sigAnue);
    sensitivityAnue->SetLineColor(kBlue);
    sensitivityAnue->SetTitle("Anue;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityAnue->Draw("AC*");
    cSensitivityAnu->cd(2);
    TGraph * sensitivityAnumu = new TGraph(nDeltaCPValues, factoredDeltaCP, sigAnumu);
    sensitivityAnumu->SetLineColor(kBlue);
    sensitivityAnumu->SetTitle("Anumu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityAnumu->Draw("AC*");
    cSensitivityAnu->cd(3);
    TGraph * sensitivityAnu = new TGraph(nDeltaCPValues, factoredDeltaCP, sigAnuSum);
    sensitivityAnu->SetLineColor(kBlue);
    sensitivityAnu->SetTitle("Anue + Anumu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityAnu->Draw("AC*");

    TCanvas * cSensitivtyNue = new TCanvas("cSensitivityNue", "cSensitivityNue");
    cSensitivtyNue->Divide(3, 1);
    cSensitivtyNue->cd(1);
    sensitivityNue->SetLineColor(kBlue);
    sensitivityNue->SetTitle("Nue;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNue->Draw("AC*");
    cSensitivtyNue->cd(2);
    sensitivityAnue->SetLineColor(kBlue);
    sensitivityAnue->SetTitle("Anue;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityAnue->Draw("AC*");
    cSensitivtyNue->cd(3);
    TGraph * sensitivityNueSum = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNueSum);
    //TGraph * sensitivityNueCombined = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNueCombined);
    sensitivityNueSum->SetLineColor(kBlue);
    sensitivityNueSum->SetTitle("nue + anue;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNueSum->Draw("AC");
    //sensitivityNueCombined->SetLineColor(kRed);
    //sensitivityNueCombined->Draw("C");
    //auto nueLegend = new TLegend(0.1,0.7,0.48,0.9);
    //nueLegend->AddEntry(sensitivityNueSum, "Spectra fitted then chi2 added", "l");
    //nueLegend->AddEntry(sensitivityNueCombined, "Spectra combined then fitted", "l");
    //nueLegend->Draw("same");

    TCanvas * cSensitivtyNumu = new TCanvas("cSensitivityNumu", "cSensitivityNumu");
    cSensitivtyNumu->Divide(3, 1);
    cSensitivtyNumu->cd(1);
    sensitivityNumu->SetLineColor(kBlue);
    sensitivityNumu->SetTitle("Numu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNumu->Draw("AC*");
    cSensitivtyNumu->cd(2);
    sensitivityAnumu->SetLineColor(kBlue);
    sensitivityAnumu->SetTitle("Anumu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityAnumu->Draw("AC*");
    cSensitivtyNumu->cd(3);
    TGraph * sensitivityNumuSum = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNumuSum);
    //TGraph * sensitivityNumuCombined = new TGraph(nDeltaCPValues, factoredDeltaCP, sigNumuCombined);
    sensitivityNumuSum->SetTitle("numu + anumu;delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivityNumuSum->SetLineColor(kBlue);
    sensitivityNumuSum->Draw("AC");
    //sensitivityNumuCombined->SetLineColor(kRed);
    //sensitivityNumuCombined->Draw("C");
    //auto numuLegend = new TLegend(0.1,0.7,0.48,0.9);
    //numuLegend->AddEntry(sensitivityNumuSum, "Spectra fitted then chi2 added", "l");
    //numuLegend->AddEntry(sensitivityNumuCombined, "Spectra combined then fitted", "l");
    //numuLegend->Draw("same");
    
    TCanvas * cSensitivity = new TCanvas("sensitivity", "sensitivity");
    TGraph * sensitivitySum = new TGraph(nDeltaCPValues, factoredDeltaCP, sigSum);
    sensitivitySum->SetLineColor(kBlue);
    sensitivitySum->SetTitle(";delta_{CP}/#pi; #sqrt{#chi^{2}}");
    sensitivitySum->Draw("AC*");
    //TGraph * sensitivityCombined = new TGraph(nDeltaCPValues, factoredDeltaCP, sigCombined);
    //sensitivityCombined->SetLineColor(kBlack);
    //sensitivityCombined->Draw("C*");

    //auto legend = new TLegend(0.1,0.7,0.48,0.9);
    //legend->AddEntry(sensitivitySum, "Spectra fitted then chi2 added", "f");
    //legend->AddEntry(sensitivityCombined, "Spectra combined then fitted", "f");
    //legend->Draw("same");

    sensitivityNue->Write("sensitivityNue");
    sensitivityAnue->Write("sensitivityAnue");
    sensitivityNumu->Write("sensitivityNumu");
    sensitivityAnumu->Write("sensitivityAnumu");
    sensitivitySum->Write("sensitivitySum");
    outputFile->Close();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillNueEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& nueEnergyDistribution, const bool isNu)
{
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        bool isNueSelected(PERFORM_CVN_SELECTION ? PassCVNNueSelection(nu) : IsNueSelected(nu, isNu));

        if (!isNueSelected)
            continue; 

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCP)));

        nueEnergyDistribution->Fill(CHEAT_ENERGY ? nu.m_eNu : nu.m_nueRecoENu, weight);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void FillNumuEnergyDistribution(const NeutrinoEventVector &nuEventVector_full, const double deltaCP, TH1D *& numuEnergyDistribution, const bool isNu)
{
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        const bool isNumuSelected(PERFORM_CVN_SELECTION ? PassCVNNumuSelection(nu) : IsNumuSelected(nu, isNu));

        if (!isNumuSelected)
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu, deltaCP)));

        numuEnergyDistribution->Fill(CHEAT_ENERGY ? nu.m_eNu : nu.m_numuRecoENu, weight);
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
        return 0;
    //std::cout << "NEGATIVE" << std::endl;

    if (chi2Sum_pi < 0.0)
        return 0;
    //std::cout << "NEGATIVE" << std::endl;

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
