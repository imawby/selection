#include "NeutrinoEvent.C"
#include "Selection.C"

void EnergyDistribution(const std::string &inputFileName);
void PlotEnergyDistribution(NeutrinoEventVector &nuEventVector_beam, NeutrinoEventVector &nuEventVector_full);
double GetOscWeight(const NeutrinoEvent &nu);

void EnergyDistribution(const std::string &inputFileName_beam, const std::string &inputFileName_full)
{
    NeutrinoEventVector nuEventVector_beam;
    ReadFile(inputFileName_beam, nuEventVector_beam);

    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full);

    PlotEnergyDistribution(nuEventVector_beam, nuEventVector_full);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void PlotEnergyDistribution(NeutrinoEventVector &nuEventVector_beam, NeutrinoEventVector &nuEventVector_full)
{
    int nBins = 40;
    double xMin = 0.0;
    double xMax = 10.0;

    TH1D * beam_noOscillations = new TH1D("beam_noOscillations", "beam_noOscillations", nBins, xMin, xMax);
    TH1D * full_noOscillations = new TH1D("full_oOscillations", "full_noOscillations", nBins, xMin, xMax);
    TH1D * full_oscillations = new TH1D("full_oscillations", "full_oscillations", nBins, xMin, xMax);

    TH1D * pot = new TH1D("pot", "pot", nBins, xMin, xMax);

    // Beam
    /*
    for (const NeutrinoEvent &nu : nuEventVector_beam)
    {
        
        if (!nu.m_isNC)
            continue;
        
        if (nu.m_selTrackPandizzleScore < -1.0)
            continue;

        
        if (nu.m_isNC)
            continue;
        
        if (nu.m_nuPdg != 14)
            continue;
        
        const double plottingEnergy = nu.m_numuRecoENu < 0.0 ? 0.0 : nu.m_numuRecoENu;
        //const double plottingEnergy = nu.m_eNu;
        const double weight(nu.m_projectedPOTWeight);

        //beam_noOscillations->Fill(nu.m_eNu, weight);
        beam_noOscillations->Fill(plottingEnergy, weight);
    }
    */

    // Full
    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        /*
        if (!nu.m_isNC)
            continue;
        
              
        if (nu.m_isNC)
            continue;
        
        if (nu.m_nuPdg != 14)
            continue;
        */

        if (!IsNumuSelected(nu))
            continue;

        pot->Fill(nu.m_projectedPOTWeight);

        const double plottingEnergy = nu.m_numuRecoENu < 0.0 ? 0.0 : nu.m_numuRecoENu;
        //const double plottingEnergy = nu.m_eNu;

        if (nu.m_numuRecoENu < 0.0)
            std::cout << "ISOBEL HERE" << std::endl;

        const double noOscillationsWeight(nu.m_projectedPOTWeight);
        full_noOscillations->Fill(plottingEnergy, noOscillationsWeight);

        const double oscillationsWeight(nu.m_projectedPOTWeight * GetOscWeight(nu));
        full_oscillations->Fill(plottingEnergy, oscillationsWeight);
    }


    //beam_noOscillations->SetLineColor(kBlack);
    full_noOscillations->SetLineColor(kBlue);
    full_oscillations->SetLineColor(kRed);

    full_noOscillations->SetTitle("NC;TrueNeutrinoEnergy [GeV]; nEvents");
    //beam_noOscillations->Draw("hist same");
    full_noOscillations->Draw("hist same");
    full_oscillations->Draw("hist same");
   
    auto pandrizzleLegend = new TLegend(0.1,0.7,0.48,0.9);
    //pandrizzleLegend->AddEntry(beam_noOscillations, "beam_noOscillations", "l");
    pandrizzleLegend->AddEntry(full_noOscillations, "full_noOscillations", "l");
    pandrizzleLegend->AddEntry(full_oscillations, "full_oscillations", "l");
    pandrizzleLegend->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu)
{
    double DEF_SIN2THETA12 = 0.310;
    double DEF_SIN2THETA13 = 0.02240;
    double DEF_SIN2THETA23 = 0.582;
    double DEF_DLITTLEM2 = 7.39e-5;
    double DEF_DBIGM2 = 2.525e-3;
    double DEF_DELTA = 0;

    double weight = getProb3ppWeights(DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, DEF_DELTA, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}

//------------------------------------------------------------------------------------------------------------------------------------------
