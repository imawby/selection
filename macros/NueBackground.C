#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"

void NueBackground(const std::string &inputFileName);
void SelectedBackgroundPlots(const NeutrinoEventVector &nuVector);
void SelectedNumuBackgroundPlots(const NeutrinoEventVector &nuVector);
void SelectedNCBackgroundPlots(const NeutrinoEventVector &nuVector);
void RejectedSignalPlots(const NeutrinoEventVector &nuVector);
void SelectedSignalPlots(const NeutrinoEventVector &nuVector);
void SelectedEnergyPlots(const NeutrinoEventVector &nuVector);
double GetOscWeight(const NeutrinoEvent &nu);
void SelectedPlots(const NeutrinoEventVector &nuVector);

bool PERFORM_OLD_NUE_SELECTION = false;
bool IS_NEUTRINO = true;
bool USE_TRUE_ENERGY = false;
bool IS_JAM_SELECTION = true;

void NueBackground(const std::string &inputFileName)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_OLD_NUE_SELECTION ? "old " : "new ") << "\033[31m" << "nue selection" << "\033[0m" << std::endl; 
    std::cout << "\033[31m" << "For " << "\033[33m" << (IS_NEUTRINO ? "F" : "R") << "\033[31m" << "HC" << "\033[0m" << std::endl;

    NeutrinoEventVector neutrinoEventVector;
    ReadFile(inputFileName, neutrinoEventVector);

    SelectedPlots(neutrinoEventVector);
    //SelectedBackgroundPlots(neutrinoEventVector);
    //SelectedNumuBackgroundPlots(neutrinoEventVector);
    //SelectedNCBackgroundPlots(neutrinoEventVector);
    //RejectedSignalPlots(neutrinoEventVector);
    //SelectedSignalPlots(neutrinoEventVector);
    //SelectedEnergyPlots(neutrinoEventVector);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void SelectedPlots(const NeutrinoEventVector &nuVector)
{
    // Flavour background
    int nEnergyBins(32);
    double minEnergy(0.0), maxEnergy(8.0);

    double nueFVCount(0.0), numuCount(0.0), nutauCount(0.0), ncCount(0.0);
    double selectedSignalQE(0.0), selectedSignalRes(0.0), selectedSignalDIS(0.0), selectedSignalCoh(0.0), selectedSignalOther(0.0);
    double rejectedSignalQE(0.0), rejectedSignalRes(0.0), rejectedSignalDIS(0.0), rejectedSignalCoh(0.0), rejectedSignalOther(0.0);
    double selectedBackgroundQE(0.0), selectedBackgroundRes(0.0), selectedBackgroundDIS(0.0), selectedBackgroundCoh(0.0), selectedBackgroundOther(0.0);
    double rejectedBackgroundQE(0.0), rejectedBackgroundRes(0.0), rejectedBackgroundDIS(0.0), rejectedBackgroundCoh(0.0), rejectedBackgroundOther(0.0);

    double signalQECount(0.0), signalResCount(0.0), signalDISCount(0.0), signalCohCount(0.0), signalOtherCount(0.0);
    double backgroundQECount(0.0), backgroundResCount(0.0), backgroundDISCount(0.0), backgroundCohCount(0.0), backgroundOtherCount(0.0);
    double selected(0.0), rejectedFlavourSignal(0.0), selectedFlavourSignal(0.0), selectedBackground(0.0), rejectedBackground(0.0);

    TH1D * nueSpectrum_True = new TH1D("nueSpectrum_True", "nueSpectrum_True", nEnergyBins, minEnergy, maxEnergy);

    TH1D * selectedSignalQE_True = new TH1D("selectedSignalQE_True", "selectedSignalQE_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalRes_True = new TH1D("selectedSignalRes_True", "selectedSignalRes_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalDIS_True = new TH1D("selectedSignalDIS_True", "selectedSignalDIS_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalCoh_True = new TH1D("selectedSignalCoh_True", "selectedSignalCoh_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalOther_True = new TH1D("selectedSignalOther_True", "selectedSignalOther_True", nEnergyBins, minEnergy, maxEnergy);

    TH1D * selectedSignalQE_Reco = new TH1D("selectedSignalQE_Reco", "selectedSignalQE_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalRes_Reco = new TH1D("selectedSignalRes_Reco", "selectedSignalRes_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalDIS_Reco = new TH1D("selectedSignalDIS_Reco", "selectedSignalDIS_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalCoh_Reco = new TH1D("selectedSignalCoh_Reco", "selectedSignalCoh_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedSignalOther_Reco = new TH1D("selectedSignalOther_Reco", "selectedSignalOther_Reco", nEnergyBins, minEnergy, maxEnergy);

    TH1D * rejectedSignalQE_True = new TH1D("rejectedSignalQE_True", "rejectedSignalQE_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalRes_True = new TH1D("rejectedSignalRes_True", "rejectedSignalRes_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalDIS_True = new TH1D("rejectedSignalDIS_True", "rejectedSignalDIS_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalCoh_True = new TH1D("rejectedSignalCoh_True", "rejectedSignalCoh_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalOther_True = new TH1D("rejectedSignalOther_True", "rejectedSignalOther_True", nEnergyBins, minEnergy, maxEnergy);

    TH1D * rejectedSignalQE_Reco = new TH1D("rejectedSignalQE_Reco", "rejectedSignalQE_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalRes_Reco = new TH1D("rejectedSignalRes_Reco", "rejectedSignalRes_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalDIS_Reco = new TH1D("rejectedSignalDIS_Reco", "rejectedSignalDIS_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalCoh_Reco = new TH1D("rejectedSignalCoh_Reco", "rejectedSignalCoh_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedSignalOther_Reco = new TH1D("rejectedSignalOther_Reco", "rejectedSignalOther_Reco", nEnergyBins, minEnergy, maxEnergy);

    TH1D * selectedBackgroundQE_True = new TH1D("selectedBackgroundQE_True", "selectedBackgroundQE_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundRes_True = new TH1D("selectedBackgroundRes_True", "selectedBackgroundRes_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundDIS_True = new TH1D("selectedBackgroundDIS_True", "selectedBackgroundDIS_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundCoh_True = new TH1D("selectedBackgroundCoh_True", "selectedBackgroundCoh_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundOther_True = new TH1D("selectedBackgroundOther_True", "selectedBackgroundOther_True", nEnergyBins, minEnergy, maxEnergy);

    TH1D * selectedBackgroundQE_Reco = new TH1D("selectedBackgroundQE_Reco", "selectedBackgroundQE_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundRes_Reco = new TH1D("selectedBackgroundRes_Reco", "selectedBackgroundRes_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundDIS_Reco = new TH1D("selectedBackgroundDIS_Reco", "selectedBackgroundDIS_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundCoh_Reco = new TH1D("selectedBackgroundCoh_Reco", "selectedBackgroundCoh_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * selectedBackgroundOther_Reco = new TH1D("selectedBackgroundOther_Reco", "selectedBackgroundOther_Reco", nEnergyBins, minEnergy, maxEnergy);

    TH1D * rejectedBackgroundQE_True = new TH1D("rejectedBackgroundQE_True", "rejectedBackgroundQE_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundRes_True = new TH1D("rejectedBackgroundRes_True", "rejectedBackgroundRes_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundDIS_True = new TH1D("rejectedBackgroundDIS_True", "rejectedBackgroundDIS_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundCoh_True = new TH1D("rejectedBackgroundCoh_True", "rejectedBackgroundCoh_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundOther_True = new TH1D("rejectedBackgroundOther_True", "rejectedBackgroundOther_True", nEnergyBins, minEnergy, maxEnergy);

    TH1D * rejectedBackgroundQE_Reco = new TH1D("rejectedBackgroundQE_Reco", "rejectedBackgroundQE_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundRes_Reco = new TH1D("rejectedBackgroundRes_Reco", "rejectedBackgroundRes_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundDIS_Reco = new TH1D("rejectedBackgroundDIS_Reco", "rejectedBackgroundDIS_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundCoh_Reco = new TH1D("rejectedBackgroundCoh_Reco", "rejectedBackgroundCoh_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * rejectedBackgroundOther_Reco = new TH1D("rejectedBackgroundOther_Reco", "rejectedBackgroundOther_Reco", nEnergyBins, minEnergy, maxEnergy);

    TH1D * nueSelected_Reco = new TH1D("nueSelected_Reco", "nueSelected_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nueFVSelected_Reco = new TH1D("nueFVSelected_Reco", "nueFVSelected_Reco", nEnergyBins, minEnergy, maxEnergy); // out of FV nue
    TH1D * numuSelected_Reco = new TH1D("numuSelected_Reco", "numuSelected_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nutauSelected_Reco = new TH1D("nutauSelected_Reco", "nutauSelected_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * ncSelected_Reco = new TH1D("ncSelected_Reco", "ncSelected_Reco", nEnergyBins, minEnergy, maxEnergy);

    TH1D * nueSelected_True = new TH1D("nueSelected_True", "nueSelected_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nueFVSelected_True = new TH1D("nueFVSelected_True", "nueFVSelected_True", nEnergyBins, minEnergy, maxEnergy); // out of FV nue
    TH1D * numuSelected_True = new TH1D("numuSelected_True", "numuSelected_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nutauSelected_True = new TH1D("nutauSelected_True", "nutauSelected_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * ncSelected_True = new TH1D("ncSelected_True", "ncSelected_True", nEnergyBins, minEnergy, maxEnergy);

    for (const NeutrinoEvent &nu : nuVector)
    {
        double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));

        // The true spectrum
        if (IsNueFlavourCCSignal(nu))
        {
            nueSpectrum_True->Fill(nu.m_eNu, weight);

            if (nu.m_intType == 0)
            {
                signalQECount += weight;
            }
            else if (nu.m_intType == 1)
            {
                signalResCount += weight;
            }
            else if (nu.m_intType == 2)
            {
                signalDISCount += weight;
            }
            else if (nu.m_intType == 3)
            {
                signalCohCount += weight;
            }
            else
            {
                signalOtherCount += weight;
            }
        }
        else
        {
            if (nu.m_intType == 0)
            {
                backgroundQECount += weight;
            }
            else if (nu.m_intType == 1)
            {
                backgroundResCount += weight;
            }
            else if (nu.m_intType == 2)
            {
                backgroundDISCount += weight;
            }
            else if (nu.m_intType == 3)
            {
                backgroundCohCount += weight;
            }
            else
            {
                backgroundOtherCount += weight;
            }
        }

        // Apply Selection
        if (IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION))
        {
            selected += weight;

            if (IsNueFlavourCCSignal(nu))
            {
                selectedFlavourSignal += weight;
                nueSelected_Reco->Fill(nu.m_nueRecoENu, weight);
                nueSelected_True->Fill(nu.m_eNu, weight);

                if (nu.m_intType == 0)
                {
                    selectedSignalQE += weight;
                    selectedSignalQE_True->Fill(nu.m_eNu, weight);
                    selectedSignalQE_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 1)
                {
                    selectedSignalRes += weight;
                    selectedSignalRes_True->Fill(nu.m_eNu, weight);
                    selectedSignalRes_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 2)
                {
                    selectedSignalDIS += weight;
                    selectedSignalDIS_True->Fill(nu.m_eNu, weight);
                    selectedSignalDIS_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 3)
                {
                    selectedSignalCoh += weight;
                    selectedSignalCoh_True->Fill(nu.m_eNu, weight);
                    selectedSignalCoh_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else
                {
                    selectedSignalOther += weight;
                    selectedSignalOther_True->Fill(nu.m_eNu, weight);
                    selectedSignalOther_Reco->Fill(nu.m_nueRecoENu, weight);
                }
            }
            else
            {
                selectedBackground += weight;

                if (nu.m_intType == 0)
                {
                    selectedBackgroundQE += weight;
                    selectedBackgroundQE_True->Fill(nu.m_eNu, weight);
                    selectedBackgroundQE_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 1)
                {
                    selectedBackgroundRes += weight;
                    selectedBackgroundRes_True->Fill(nu.m_eNu, weight);
                    selectedBackgroundRes_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 2)
                {
                    selectedBackgroundDIS += weight;
                    selectedBackgroundDIS_True->Fill(nu.m_eNu, weight);
                    selectedBackgroundDIS_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 3)
                {
                    selectedBackgroundCoh += weight;
                    selectedBackgroundCoh_True->Fill(nu.m_eNu, weight);
                    selectedBackgroundCoh_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else
                {
                    selectedBackgroundOther += weight;
                    selectedBackgroundOther_True->Fill(nu.m_eNu, weight);
                    selectedBackgroundOther_Reco->Fill(nu.m_nueRecoENu, weight);
                }

                if (nu.m_isNC)
                {
                    ncCount += weight;
                    ncSelected_Reco->Fill(nu.m_nueRecoENu, weight);
                    ncSelected_True->Fill(nu.m_eNu, weight);
                }
                else if (std::abs(nu.m_nuPdg) == 12)
                {
                    nueFVCount += weight;
                    nueFVSelected_Reco->Fill(nu.m_nueRecoENu, weight);
                    nueFVSelected_True->Fill(nu.m_eNu, weight);
                }
                else if (std::abs(nu.m_nuPdg) == 14)
                {
                    numuCount += weight;
                    numuSelected_Reco->Fill(nu.m_nueRecoENu, weight);
                    numuSelected_True->Fill(nu.m_eNu, weight);
                }
                else if (std::abs(nu.m_nuPdg) == 16)
                {
                    nutauCount += weight;
                    nutauSelected_Reco->Fill(nu.m_nueRecoENu, weight);
                    nutauSelected_True->Fill(nu.m_eNu, weight);
                }
                else
                {
                    std::cout << "what is this" << std::endl;
                    throw;
                }
            }
        }
        else
        {
            if (IsNueFlavourCCSignal(nu))
            {
                rejectedFlavourSignal += weight;

                if (nu.m_intType == 0)
                {
                    rejectedSignalQE += weight;
                    rejectedSignalQE_True->Fill(nu.m_eNu, weight);
                    rejectedSignalQE_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 1)
                {
                    rejectedSignalRes += weight;
                    rejectedSignalRes_True->Fill(nu.m_eNu, weight);
                    rejectedSignalRes_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 2)
                {
                    rejectedSignalDIS += weight;
                    rejectedSignalDIS_True->Fill(nu.m_eNu, weight);
                    rejectedSignalDIS_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 3)
                {
                    rejectedSignalCoh += weight;
                    rejectedSignalCoh_True->Fill(nu.m_eNu, weight);
                    rejectedSignalCoh_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else
                {
                    rejectedSignalOther += weight;
                    rejectedSignalOther_True->Fill(nu.m_eNu, weight);
                    rejectedSignalOther_Reco->Fill(nu.m_nueRecoENu, weight);
                }
            }
            else
            {
                rejectedBackground += weight;

                if (nu.m_intType == 0)
                {
                    rejectedBackgroundQE += weight;
                    rejectedBackgroundQE_True->Fill(nu.m_eNu, weight);
                    rejectedBackgroundQE_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 1)
                {
                    rejectedBackgroundRes += weight;
                    rejectedBackgroundRes_True->Fill(nu.m_eNu, weight);
                    rejectedBackgroundRes_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 2)
                {
                    rejectedBackgroundDIS += weight;
                    rejectedBackgroundDIS_True->Fill(nu.m_eNu, weight);
                    rejectedBackgroundDIS_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else if (nu.m_intType == 3)
                {
                    rejectedBackgroundCoh += weight;
                    rejectedBackgroundCoh_True->Fill(nu.m_eNu, weight);
                    rejectedBackgroundCoh_Reco->Fill(nu.m_nueRecoENu, weight);
                }
                else
                {
                    rejectedBackgroundOther += weight;
                    rejectedBackgroundOther_True->Fill(nu.m_eNu, weight);
                    rejectedBackgroundOther_Reco->Fill(nu.m_nueRecoENu, weight);
                }
            }
        }
    }

    // Integrated flavour metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "Selected signal: " << selectedFlavourSignal * 100/selected << "%" << std::endl;
    std::cout << "Selected background: " << selectedBackground/selected << "%" << std::endl;
    std::cout << "Out of FV anue/nue: " << nueFVCount * 100/selected << "%" << std::endl;
    std::cout << "numu/anumu: " << numuCount * 100/selected << "%" << std::endl;
    std::cout << "nutau/anutau: " << nutauCount * 100/selected << "%" << std::endl;
    std::cout << "NC: " << ncCount * 100/selected << "%" << std::endl;

    // Integrated interaction type metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "IN SELECTED SIGNAL" << std::endl;
    std::cout << "QE Proportion: " << selectedSignalQE * 100/selectedFlavourSignal << "%" << std::endl; 
    std::cout << "Res Proportion: " << selectedSignalRes * 100/selectedFlavourSignal << "%" << std::endl;
    std::cout << "DIS Proportion: " << selectedSignalDIS * 100/selectedFlavourSignal << "%" << std::endl;
    std::cout << "Coh Proportion: " << selectedSignalCoh * 100/selectedFlavourSignal << "%" << std::endl;
    std::cout << "Other Proportion: " << selectedSignalOther * 100/selectedFlavourSignal << "%" << std::endl;

    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "IN REJECTED SIGNAL" << std::endl;
    std::cout << "QE Proportion: " << rejectedSignalQE * 100/rejectedFlavourSignal << "%" << std::endl; 
    std::cout << "Res Proportion: " << rejectedSignalRes * 100/rejectedFlavourSignal << "%" << std::endl;
    std::cout << "DIS Proportion: " << rejectedSignalDIS * 100/rejectedFlavourSignal << "%" << std::endl;
    std::cout << "Coh Proportion: " << rejectedSignalCoh * 100/rejectedFlavourSignal << "%" << std::endl;
    std::cout << "Other Proportion: " << rejectedSignalOther * 100/rejectedFlavourSignal << "%" << std::endl;

    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "IN SELECTED BACKGROUND" << std::endl;
    std::cout << "QE Proportion: " << selectedBackgroundQE * 100/selectedBackground << "%" << std::endl; 
    std::cout << "Res Proportion: " << selectedBackgroundRes * 100/selectedBackground << "%" << std::endl;
    std::cout << "DIS Proportion: " << selectedBackgroundDIS * 100/selectedBackground << "%" << std::endl;
    std::cout << "Coh Proportion: " << selectedBackgroundCoh * 100/selectedBackground << "%" << std::endl;
    std::cout << "Other Proportion: " << selectedBackgroundOther * 100/selectedBackground << "%" << std::endl;

    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "IN REJECTED BACKGROUND" << std::endl;
    std::cout << "QE Proportion: " << rejectedBackgroundQE * 100/rejectedBackground << "%" << std::endl; 
    std::cout << "Res Proportion: " << rejectedBackgroundRes * 100/rejectedBackground << "%" << std::endl;
    std::cout << "DIS Proportion: " << rejectedBackgroundDIS * 100/rejectedBackground << "%" << std::endl;
    std::cout << "Coh Proportion: " << rejectedBackgroundCoh * 100/rejectedBackground << "%" << std::endl;
    std::cout << "Other Proportion: " << rejectedBackgroundOther * 100/rejectedBackground << "%" << std::endl;

    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "IN REJECTED BACKGROUND" << std::endl;
    std::cout << "QE Proportion: " << rejectedBackgroundQE * 100/rejectedBackground << "%" << std::endl; 
    std::cout << "Res Proportion: " << rejectedBackgroundRes * 100/rejectedBackground << "%" << std::endl;
    std::cout << "DIS Proportion: " << rejectedBackgroundDIS * 100/rejectedBackground << "%" << std::endl;
    std::cout << "Coh Proportion: " << rejectedBackgroundCoh * 100/rejectedBackground << "%" << std::endl;
    std::cout << "Other Proportion: " << rejectedBackgroundOther * 100/rejectedBackground << "%" << std::endl;

    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "SIGNAL INTERACTION TYPE" << std::endl;
    std::cout << "Signal QE: " << selectedSignalQE * 100/signalQECount << "% selected, " << rejectedSignalQE * 100/signalQECount << "% rejected" << std::endl;
    std::cout << "Signal Res: " << selectedSignalRes * 100/signalResCount << "% selected, " << rejectedSignalRes * 100/signalResCount << "% rejected" << std::endl;
    std::cout << "Signal DIS: " << selectedSignalDIS * 100/signalDISCount << "% selected, " << rejectedSignalDIS * 100/signalDISCount << "% rejected" << std::endl;
    std::cout << "Signal Coh: " << selectedSignalCoh * 100/signalCohCount << "% selected, " << rejectedSignalCoh * 100/signalCohCount << "% rejected" << std::endl;
    std::cout << "Signal Other: " << selectedSignalOther * 100/signalOtherCount << "% selected, " << rejectedSignalOther * 100/signalOtherCount << "% rejected" << std::endl;

    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "BACKGROUND INTERACTION TYPE" << std::endl;
    std::cout << "Background QE: " << selectedBackgroundQE * 100/backgroundQECount << "% selected, " << rejectedBackgroundQE * 100/backgroundQECount << "% rejected" << std::endl;
    std::cout << "Background Res: " << selectedBackgroundRes * 100/backgroundResCount << "% selected, " << rejectedBackgroundRes * 100/backgroundResCount << "% rejected" << std::endl;
    std::cout << "Background DIS: " << selectedBackgroundDIS * 100/backgroundDISCount << "% selected, " << rejectedBackgroundDIS * 100/backgroundDISCount << "% rejected" << std::endl;
    std::cout << "Background Coh: " << selectedBackgroundCoh * 100/backgroundCohCount << "% selected, " << rejectedBackgroundCoh * 100/backgroundCohCount << "% rejected" << std::endl;
    std::cout << "Background Other: " << selectedBackgroundOther * 100/backgroundOtherCount << "% selected, " << rejectedBackgroundOther * 100/backgroundOtherCount << "% rejected" << std::endl;


    // Draw those histograms
    TCanvas * c1 = new TCanvas("Background_True", "Background_True");
    THStack * flavourStack_True = new THStack("flavourStack_True", ";True Neutrino Energy [GeV]; nEvents");

    nueSelected_True->SetFillColor(kBlack);
    nueFVSelected_True->SetFillColor(kGray);
    numuSelected_True->SetFillColor(kBlue);
    nutauSelected_True->SetFillColor(kGreen);
    ncSelected_True->SetFillColor(kRed);

    nueSelected_True->SetLineColor(kBlack);
    nueFVSelected_True->SetLineColor(kGray);
    numuSelected_True->SetLineColor(kBlue);
    nutauSelected_True->SetLineColor(kGreen);
    ncSelected_True->SetLineColor(kRed);

    nueSelected_True->SetFillStyle(3144);
    nueFVSelected_True->SetFillStyle(3144);
    numuSelected_True->SetFillStyle(3144);
    nutauSelected_True->SetFillStyle(3144);
    ncSelected_True->SetFillStyle(3144);

    flavourStack_True->Add(numuSelected_True);
    flavourStack_True->Add(nutauSelected_True);
    flavourStack_True->Add(ncSelected_True);
    flavourStack_True->Add(nueFVSelected_True);
    flavourStack_True->Add(nueSelected_True);
    flavourStack_True->Draw("hist");

    auto legend_True = new TLegend(0.1,0.7,0.48,0.9);
    legend_True->AddEntry(nueSelected_True, "CC #nu_{e} + #bar{#nu_{e}}", "f");
    legend_True->AddEntry(numuSelected_True, "CC #nu_{#mu} + #bar{#nu_{#mu}}", "f");
    legend_True->AddEntry(nueFVSelected_True, "CC #nu_{e} + CC #bar{#nu_{e}} out of FV", "f"); // and not interacting on LAr
    legend_True->AddEntry(nutauSelected_True, "CC #nu_{#tau} + #bar{#nu_{#tau}}", "f"); 
    legend_True->AddEntry(ncSelected_True, "NC", "f");
    legend_True->Draw("same");

    TCanvas * c1b = new TCanvas("Background_Reco", "Background_Reco");
    THStack * flavourStack_Reco = new THStack("flavourStack_Reco", ";Reconstructed Neutrino Energy [GeV]; nEvents");

    nueSelected_Reco->SetFillColor(kBlack);
    nueFVSelected_Reco->SetFillColor(kGray);
    numuSelected_Reco->SetFillColor(kBlue);
    nutauSelected_Reco->SetFillColor(kGreen);
    ncSelected_Reco->SetFillColor(kRed);

    nueSelected_Reco->SetLineColor(kBlack);
    nueFVSelected_Reco->SetLineColor(kGray);
    numuSelected_Reco->SetLineColor(kBlue);
    nutauSelected_Reco->SetLineColor(kGreen);
    ncSelected_Reco->SetLineColor(kRed);

    nueSelected_Reco->SetFillStyle(3144);
    nueFVSelected_Reco->SetFillStyle(3144);
    numuSelected_Reco->SetFillStyle(3144);
    nutauSelected_Reco->SetFillStyle(3144);
    ncSelected_Reco->SetFillStyle(3144);

    flavourStack_Reco->Add(numuSelected_Reco);
    flavourStack_Reco->Add(nutauSelected_Reco);
    flavourStack_Reco->Add(ncSelected_Reco);
    flavourStack_Reco->Add(nueFVSelected_Reco);
    flavourStack_Reco->Add(nueSelected_Reco);
    flavourStack_Reco->Draw("hist");

    auto legend_Reco = new TLegend(0.1,0.7,0.48,0.9);
    legend_Reco->AddEntry(nueSelected_Reco, "CC #nu_{e} + #bar{#nu_{e}}", "f");
    legend_Reco->AddEntry(numuSelected_Reco, "CC #nu_{#mu} + #bar{#nu_{#mu}}", "f");
    legend_Reco->AddEntry(nueFVSelected_Reco, "CC #nu_{e} + CC #bar{#nu_{e}} out of FV", "f"); // and not interacting on LAr
    legend_Reco->AddEntry(nutauSelected_Reco, "CC #nu_{#tau} + #bar{#nu_{#tau}}", "f"); 
    legend_Reco->AddEntry(ncSelected_Reco, "NC", "f");
    legend_Reco->Draw("same");

    TCanvas * c2 = new TCanvas("True Nue Spectrum", "True Nue Spectrum");
    nueSpectrum_True->Draw("hist");

    TCanvas * c3 = new TCanvas("Selected Signal Interaction Type", "Selected Signal Interaction Type");

    c3->Divide(2,1);
    c3->cd(1);

    THStack * selectedSignalIntTypeStack_True = new THStack("selectedSignalIntTypeStack_True", "Selected Signal Interaction Type Decomposition;True Neutrino Energy [GeV]; nEvents");

    selectedSignalQE_True->SetFillColor(kBlack);
    selectedSignalRes_True->SetFillColor(kGray);
    selectedSignalDIS_True->SetFillColor(kBlue);
    selectedSignalCoh_True->SetFillColor(kGreen);
    selectedSignalOther_True->SetFillColor(kRed);

    selectedSignalQE_True->SetLineColor(kBlack);
    selectedSignalRes_True->SetLineColor(kGray);
    selectedSignalDIS_True->SetLineColor(kBlue);
    selectedSignalCoh_True->SetLineColor(kGreen);
    selectedSignalOther_True->SetLineColor(kRed);

    selectedSignalQE_True->SetFillStyle(3144);
    selectedSignalRes_True->SetFillStyle(3144);
    selectedSignalDIS_True->SetFillStyle(3144);
    selectedSignalCoh_True->SetFillStyle(3144);
    selectedSignalOther_True->SetFillStyle(3144);

    selectedSignalIntTypeStack_True->Add(selectedSignalOther_True);
    selectedSignalIntTypeStack_True->Add(selectedSignalCoh_True);
    selectedSignalIntTypeStack_True->Add(selectedSignalDIS_True);
    selectedSignalIntTypeStack_True->Add(selectedSignalRes_True);
    selectedSignalIntTypeStack_True->Add(selectedSignalQE_True);
    selectedSignalIntTypeStack_True->Draw("hist");

    auto legendSelectedSignalIntType_True = new TLegend(0.1,0.7,0.48,0.9);
    legendSelectedSignalIntType_True->AddEntry(selectedSignalQE_True, "Selected Signal QE Component", "f");
    legendSelectedSignalIntType_True->AddEntry(selectedSignalRes_True, "Selected Signal Res Component", "f");
    legendSelectedSignalIntType_True->AddEntry(selectedSignalDIS_True, "Selected Signal DIS Component", "f");
    legendSelectedSignalIntType_True->AddEntry(selectedSignalCoh_True, "Selected Signal Coh Component", "f");
    legendSelectedSignalIntType_True->AddEntry(selectedSignalOther_True, "Selected Signal Other Component", "f");
    legendSelectedSignalIntType_True->Draw("same");

    c3->cd(2);

    THStack * selectedSignalIntTypeStack_Reco = new THStack("selectedSignalIntTypeStack_Reco", "Selected Signal Interaction Type Decomposition;Reco Neutrino Energy [GeV]; nEvents");

    selectedSignalQE_Reco->SetFillColor(kBlack);
    selectedSignalRes_Reco->SetFillColor(kGray);
    selectedSignalDIS_Reco->SetFillColor(kBlue);
    selectedSignalCoh_Reco->SetFillColor(kGreen);
    selectedSignalOther_Reco->SetFillColor(kRed);

    selectedSignalQE_Reco->SetLineColor(kBlack);
    selectedSignalRes_Reco->SetLineColor(kGray);
    selectedSignalDIS_Reco->SetLineColor(kBlue);
    selectedSignalCoh_Reco->SetLineColor(kGreen);
    selectedSignalOther_Reco->SetLineColor(kRed);

    selectedSignalQE_Reco->SetFillStyle(3144);
    selectedSignalRes_Reco->SetFillStyle(3144);
    selectedSignalDIS_Reco->SetFillStyle(3144);
    selectedSignalCoh_Reco->SetFillStyle(3144);
    selectedSignalOther_Reco->SetFillStyle(3144);

    selectedSignalIntTypeStack_Reco->Add(selectedSignalOther_Reco);
    selectedSignalIntTypeStack_Reco->Add(selectedSignalCoh_Reco);
    selectedSignalIntTypeStack_Reco->Add(selectedSignalDIS_Reco);
    selectedSignalIntTypeStack_Reco->Add(selectedSignalRes_Reco);
    selectedSignalIntTypeStack_Reco->Add(selectedSignalQE_Reco);
    selectedSignalIntTypeStack_Reco->Draw("hist");

    auto legendSelectedSignalIntType_Reco = new TLegend(0.1,0.7,0.48,0.9);
    legendSelectedSignalIntType_Reco->AddEntry(selectedSignalQE_Reco, "Selected Signal QE Component", "f");
    legendSelectedSignalIntType_Reco->AddEntry(selectedSignalRes_Reco, "Selected Signal Res Component", "f");
    legendSelectedSignalIntType_Reco->AddEntry(selectedSignalDIS_Reco, "Selected Signal DIS Component", "f");
    legendSelectedSignalIntType_Reco->AddEntry(selectedSignalCoh_Reco, "Selected Signal Coh Component", "f");
    legendSelectedSignalIntType_Reco->AddEntry(selectedSignalOther_Reco, "Selected Signal Other Component", "f");
    legendSelectedSignalIntType_Reco->Draw("same");

    TCanvas * c4 = new TCanvas("Rejected Signal Interaction Type", "Rejected Signal Interaction Type");

    c4->Divide(2,1);
    c4->cd(1);

    THStack * rejectedSignalIntTypeStack_True = new THStack("rejectedSignalIntTypeStack_True", "Rejected Signal Interaction Type Decomposition;True Neutrino Energy [GeV]; nEvents");

    rejectedSignalQE_True->SetFillColor(kBlack);
    rejectedSignalRes_True->SetFillColor(kGray);
    rejectedSignalDIS_True->SetFillColor(kBlue);
    rejectedSignalCoh_True->SetFillColor(kGreen);
    rejectedSignalOther_True->SetFillColor(kRed);

    rejectedSignalQE_True->SetLineColor(kBlack);
    rejectedSignalRes_True->SetLineColor(kGray);
    rejectedSignalDIS_True->SetLineColor(kBlue);
    rejectedSignalCoh_True->SetLineColor(kGreen);
    rejectedSignalOther_True->SetLineColor(kRed);

    rejectedSignalQE_True->SetFillStyle(3144);
    rejectedSignalRes_True->SetFillStyle(3144);
    rejectedSignalDIS_True->SetFillStyle(3144);
    rejectedSignalCoh_True->SetFillStyle(3144);
    rejectedSignalOther_True->SetFillStyle(3144);

    rejectedSignalIntTypeStack_True->Add(rejectedSignalOther_True);
    rejectedSignalIntTypeStack_True->Add(rejectedSignalCoh_True);
    rejectedSignalIntTypeStack_True->Add(rejectedSignalDIS_True);
    rejectedSignalIntTypeStack_True->Add(rejectedSignalRes_True);
    rejectedSignalIntTypeStack_True->Add(rejectedSignalQE_True);
    rejectedSignalIntTypeStack_True->Draw("hist");

    auto legendRejectedSignalIntType_True = new TLegend(0.1,0.7,0.48,0.9);
    legendRejectedSignalIntType_True->AddEntry(rejectedSignalQE_True, "Rejected Signal QE Component", "f");
    legendRejectedSignalIntType_True->AddEntry(rejectedSignalRes_True, "Rejected Signal Res Component", "f");
    legendRejectedSignalIntType_True->AddEntry(rejectedSignalDIS_True, "Rejected Signal DIS Component", "f");
    legendRejectedSignalIntType_True->AddEntry(rejectedSignalCoh_True, "Rejected Signal Coh Component", "f");
    legendRejectedSignalIntType_True->AddEntry(rejectedSignalOther_True, "Rejected Signal Other Component", "f");
    legendRejectedSignalIntType_True->Draw("same");

    c4->cd(2);

    THStack * rejectedSignalIntTypeStack_Reco = new THStack("rejectedSignalIntTypeStack_Reco", "Rejected Signal Interaction Type Decomposition;Reco Neutrino Energy [GeV]; nEvents");

    rejectedSignalQE_Reco->SetFillColor(kBlack);
    rejectedSignalRes_Reco->SetFillColor(kGray);
    rejectedSignalDIS_Reco->SetFillColor(kBlue);
    rejectedSignalCoh_Reco->SetFillColor(kGreen);
    rejectedSignalOther_Reco->SetFillColor(kRed);

    rejectedSignalQE_Reco->SetLineColor(kBlack);
    rejectedSignalRes_Reco->SetLineColor(kGray);
    rejectedSignalDIS_Reco->SetLineColor(kBlue);
    rejectedSignalCoh_Reco->SetLineColor(kGreen);
    rejectedSignalOther_Reco->SetLineColor(kRed);

    rejectedSignalQE_Reco->SetFillStyle(3144);
    rejectedSignalRes_Reco->SetFillStyle(3144);
    rejectedSignalDIS_Reco->SetFillStyle(3144);
    rejectedSignalCoh_Reco->SetFillStyle(3144);
    rejectedSignalOther_Reco->SetFillStyle(3144);

    rejectedSignalIntTypeStack_Reco->Add(rejectedSignalOther_Reco);
    rejectedSignalIntTypeStack_Reco->Add(rejectedSignalCoh_Reco);
    rejectedSignalIntTypeStack_Reco->Add(rejectedSignalDIS_Reco);
    rejectedSignalIntTypeStack_Reco->Add(rejectedSignalRes_Reco);
    rejectedSignalIntTypeStack_Reco->Add(rejectedSignalQE_Reco);
    rejectedSignalIntTypeStack_Reco->Draw("hist");

    auto legendRejectedSignalIntType_Reco = new TLegend(0.1,0.7,0.48,0.9);
    legendRejectedSignalIntType_Reco->AddEntry(rejectedSignalQE_Reco, "Rejected Signal QE Component", "f");
    legendRejectedSignalIntType_Reco->AddEntry(rejectedSignalRes_Reco, "Rejected Signal Res Component", "f");
    legendRejectedSignalIntType_Reco->AddEntry(rejectedSignalDIS_Reco, "Rejected Signal DIS Component", "f");
    legendRejectedSignalIntType_Reco->AddEntry(rejectedSignalCoh_Reco, "Rejected Signal Coh Component", "f");
    legendRejectedSignalIntType_Reco->AddEntry(rejectedSignalOther_Reco, "Rejected Signal Other Component", "f");
    legendRejectedSignalIntType_Reco->Draw("same");

    TCanvas * c5 = new TCanvas("Selected Background Interaction Type", "Selected Background Interaction Type");

    c5->Divide(2,1);
    c5->cd(1);

    THStack * selectedBackgroundIntTypeStack_True = new THStack("selectedBackgroundIntTypeStack_True", "Selected Background Interaction Type Decomposition;True Neutrino Energy [GeV]; nEvents");

    selectedBackgroundQE_True->SetFillColor(kBlack);
    selectedBackgroundRes_True->SetFillColor(kGray);
    selectedBackgroundDIS_True->SetFillColor(kBlue);
    selectedBackgroundCoh_True->SetFillColor(kGreen);
    selectedBackgroundOther_True->SetFillColor(kRed);

    selectedBackgroundQE_True->SetLineColor(kBlack);
    selectedBackgroundRes_True->SetLineColor(kGray);
    selectedBackgroundDIS_True->SetLineColor(kBlue);
    selectedBackgroundCoh_True->SetLineColor(kGreen);
    selectedBackgroundOther_True->SetLineColor(kRed);

    selectedBackgroundQE_True->SetFillStyle(3144);
    selectedBackgroundRes_True->SetFillStyle(3144);
    selectedBackgroundDIS_True->SetFillStyle(3144);
    selectedBackgroundCoh_True->SetFillStyle(3144);
    selectedBackgroundOther_True->SetFillStyle(3144);

    selectedBackgroundIntTypeStack_True->Add(selectedBackgroundOther_True);
    selectedBackgroundIntTypeStack_True->Add(selectedBackgroundCoh_True);
    selectedBackgroundIntTypeStack_True->Add(selectedBackgroundDIS_True);
    selectedBackgroundIntTypeStack_True->Add(selectedBackgroundRes_True);
    selectedBackgroundIntTypeStack_True->Add(selectedBackgroundQE_True);
    selectedBackgroundIntTypeStack_True->Draw("hist");

    auto legendSelectedBackgroundIntType_True = new TLegend(0.1,0.7,0.48,0.9);
    legendSelectedBackgroundIntType_True->AddEntry(selectedBackgroundQE_True, "Selected Background QE Component", "f");
    legendSelectedBackgroundIntType_True->AddEntry(selectedBackgroundRes_True, "Selected Background Res Component", "f");
    legendSelectedBackgroundIntType_True->AddEntry(selectedBackgroundDIS_True, "Selected Background DIS Component", "f");
    legendSelectedBackgroundIntType_True->AddEntry(selectedBackgroundCoh_True, "Selected Background Coh Component", "f");
    legendSelectedBackgroundIntType_True->AddEntry(selectedBackgroundOther_True, "Selected Background Other Component", "f");
    legendSelectedBackgroundIntType_True->Draw("same");

    c5->cd(2);

    THStack * selectedBackgroundIntTypeStack_Reco = new THStack("selectedBackgroundIntTypeStack_Reco", "Selected Background Interaction Type Decomposition;Reco Neutrino Energy [GeV]; nEvents");

    selectedBackgroundQE_Reco->SetFillColor(kBlack);
    selectedBackgroundRes_Reco->SetFillColor(kGray);
    selectedBackgroundDIS_Reco->SetFillColor(kBlue);
    selectedBackgroundCoh_Reco->SetFillColor(kGreen);
    selectedBackgroundOther_Reco->SetFillColor(kRed);

    selectedBackgroundQE_Reco->SetLineColor(kBlack);
    selectedBackgroundRes_Reco->SetLineColor(kGray);
    selectedBackgroundDIS_Reco->SetLineColor(kBlue);
    selectedBackgroundCoh_Reco->SetLineColor(kGreen);
    selectedBackgroundOther_Reco->SetLineColor(kRed);

    selectedBackgroundQE_Reco->SetFillStyle(3144);
    selectedBackgroundRes_Reco->SetFillStyle(3144);
    selectedBackgroundDIS_Reco->SetFillStyle(3144);
    selectedBackgroundCoh_Reco->SetFillStyle(3144);
    selectedBackgroundOther_Reco->SetFillStyle(3144);

    selectedBackgroundIntTypeStack_Reco->Add(selectedBackgroundOther_Reco);
    selectedBackgroundIntTypeStack_Reco->Add(selectedBackgroundCoh_Reco);
    selectedBackgroundIntTypeStack_Reco->Add(selectedBackgroundDIS_Reco);
    selectedBackgroundIntTypeStack_Reco->Add(selectedBackgroundRes_Reco);
    selectedBackgroundIntTypeStack_Reco->Add(selectedBackgroundQE_Reco);
    selectedBackgroundIntTypeStack_Reco->Draw("hist");

    auto legendSelectedBackgroundIntType_Reco = new TLegend(0.1,0.7,0.48,0.9);
    legendSelectedBackgroundIntType_Reco->AddEntry(selectedBackgroundQE_Reco, "Selected Background QE Component", "f");
    legendSelectedBackgroundIntType_Reco->AddEntry(selectedBackgroundRes_Reco, "Selected Background Res Component", "f");
    legendSelectedBackgroundIntType_Reco->AddEntry(selectedBackgroundDIS_Reco, "Selected Background DIS Component", "f");
    legendSelectedBackgroundIntType_Reco->AddEntry(selectedBackgroundCoh_Reco, "Selected Background Coh Component", "f");
    legendSelectedBackgroundIntType_Reco->AddEntry(selectedBackgroundOther_Reco, "Selected Background Other Component", "f");
    legendSelectedBackgroundIntType_Reco->Draw("same");

    TCanvas * c6 = new TCanvas("Rejected Background Interaction Type", "Rejected Background Interaction Type");

    c6->Divide(2,1);
    c6->cd(1);

    THStack * rejectedBackgroundIntTypeStack_True = new THStack("rejectedBackgroundIntTypeStack_True", "Rejected Background Interaction Type Decomposition;True Neutrino Energy [GeV]; nEvents");

    rejectedBackgroundQE_True->SetFillColor(kBlack);
    rejectedBackgroundRes_True->SetFillColor(kGray);
    rejectedBackgroundDIS_True->SetFillColor(kBlue);
    rejectedBackgroundCoh_True->SetFillColor(kGreen);
    rejectedBackgroundOther_True->SetFillColor(kRed);

    rejectedBackgroundQE_True->SetLineColor(kBlack);
    rejectedBackgroundRes_True->SetLineColor(kGray);
    rejectedBackgroundDIS_True->SetLineColor(kBlue);
    rejectedBackgroundCoh_True->SetLineColor(kGreen);
    rejectedBackgroundOther_True->SetLineColor(kRed);

    rejectedBackgroundQE_True->SetFillStyle(3144);
    rejectedBackgroundRes_True->SetFillStyle(3144);
    rejectedBackgroundDIS_True->SetFillStyle(3144);
    rejectedBackgroundCoh_True->SetFillStyle(3144);
    rejectedBackgroundOther_True->SetFillStyle(3144);

    rejectedBackgroundIntTypeStack_True->Add(rejectedBackgroundOther_True);
    rejectedBackgroundIntTypeStack_True->Add(rejectedBackgroundCoh_True);
    rejectedBackgroundIntTypeStack_True->Add(rejectedBackgroundDIS_True);
    rejectedBackgroundIntTypeStack_True->Add(rejectedBackgroundRes_True);
    rejectedBackgroundIntTypeStack_True->Add(rejectedBackgroundQE_True);
    rejectedBackgroundIntTypeStack_True->Draw("hist");

    auto legendRejectedBackgroundIntType_True = new TLegend(0.1,0.7,0.48,0.9);
    legendRejectedBackgroundIntType_True->AddEntry(rejectedBackgroundQE_True, "Rejected Background QE Component", "f");
    legendRejectedBackgroundIntType_True->AddEntry(rejectedBackgroundRes_True, "Rejected Background Res Component", "f");
    legendRejectedBackgroundIntType_True->AddEntry(rejectedBackgroundDIS_True, "Rejected Background DIS Component", "f");
    legendRejectedBackgroundIntType_True->AddEntry(rejectedBackgroundCoh_True, "Rejected Background Coh Component", "f");
    legendRejectedBackgroundIntType_True->AddEntry(rejectedBackgroundOther_True, "Rejected Background Other Component", "f");
    legendRejectedBackgroundIntType_True->Draw("same");

    c6->cd(2);

    THStack * rejectedBackgroundIntTypeStack_Reco = new THStack("rejectedBackgroundIntTypeStack_Reco", "Rejected Background Interaction Type Decomposition;Reco Neutrino Energy [GeV]; nEvents");

    rejectedBackgroundQE_Reco->SetFillColor(kBlack);
    rejectedBackgroundRes_Reco->SetFillColor(kGray);
    rejectedBackgroundDIS_Reco->SetFillColor(kBlue);
    rejectedBackgroundCoh_Reco->SetFillColor(kGreen);
    rejectedBackgroundOther_Reco->SetFillColor(kRed);

    rejectedBackgroundQE_Reco->SetLineColor(kBlack);
    rejectedBackgroundRes_Reco->SetLineColor(kGray);
    rejectedBackgroundDIS_Reco->SetLineColor(kBlue);
    rejectedBackgroundCoh_Reco->SetLineColor(kGreen);
    rejectedBackgroundOther_Reco->SetLineColor(kRed);

    rejectedBackgroundQE_Reco->SetFillStyle(3144);
    rejectedBackgroundRes_Reco->SetFillStyle(3144);
    rejectedBackgroundDIS_Reco->SetFillStyle(3144);
    rejectedBackgroundCoh_Reco->SetFillStyle(3144);
    rejectedBackgroundOther_Reco->SetFillStyle(3144);

    rejectedBackgroundIntTypeStack_Reco->Add(rejectedBackgroundOther_Reco);
    rejectedBackgroundIntTypeStack_Reco->Add(rejectedBackgroundCoh_Reco);
    rejectedBackgroundIntTypeStack_Reco->Add(rejectedBackgroundDIS_Reco);
    rejectedBackgroundIntTypeStack_Reco->Add(rejectedBackgroundRes_Reco);
    rejectedBackgroundIntTypeStack_Reco->Add(rejectedBackgroundQE_Reco);
    rejectedBackgroundIntTypeStack_Reco->Draw("hist");

    auto legendRejectedBackgroundIntType_Reco = new TLegend(0.1,0.7,0.48,0.9);
    legendRejectedBackgroundIntType_Reco->AddEntry(rejectedBackgroundQE_Reco, "Rejected Background QE Component", "f");
    legendRejectedBackgroundIntType_Reco->AddEntry(rejectedBackgroundRes_Reco, "Rejected Background Res Component", "f");
    legendRejectedBackgroundIntType_Reco->AddEntry(rejectedBackgroundDIS_Reco, "Rejected Background DIS Component", "f");
    legendRejectedBackgroundIntType_Reco->AddEntry(rejectedBackgroundCoh_Reco, "Rejected Background Coh Component", "f");
    legendRejectedBackgroundIntType_Reco->AddEntry(rejectedBackgroundOther_Reco, "Rejected Background Other Component", "f");
    legendRejectedBackgroundIntType_Reco->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void SelectedBackgroundPlots(const NeutrinoEventVector &nuVector)
{
    // Flavour background
    int nEnergyBins(32);
    double minEnergy(0.0), maxEnergy(8.0);

    double nueCount(0.0), nueFVCount(0.0), numuCount(0.0), nutauCount(0.0), ncCount(0.0);
    TH1D * nueFVSelected = new TH1D("nueFVSelected", "nueFVSelected", nEnergyBins, minEnergy, maxEnergy); // out of FV nue
    TH1D * numuSelected = new TH1D("numuSelected", "numuSelected", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nutauSelected = new TH1D("nutauSelected", "nutauSelected", nEnergyBins, minEnergy, maxEnergy);
    TH1D * ncSelected = new TH1D("ncSelected", "ncSelected", nEnergyBins, minEnergy, maxEnergy);

    // Selected shower PDG
    int nPandrizzleBins(50);
    double minPandrizzle(-1.0), maxPandrizzle(1.0);

    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0), noShowerCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * muon = new TH1D("muon", "muon", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * photon = new TH1D("photon", "photon", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * pion = new TH1D("pion", "pion", nPandrizzleBins, minPandrizzle, maxPandrizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * other = new TH1D("other", "other", nPandrizzleBins, minPandrizzle, maxPandrizzle);

    double selectedBackground(0);

    double pion_NC(0), pion_numu(0), pion_nutau(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Look at background (everything that is not a nue + anue CC event)
        if (IsNueFlavourCCSignal(nu))
            continue;

        // Apply Selection
        if ((!PERFORM_OLD_NUE_SELECTION && IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)))
        {
            const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
            selectedBackground += weight;

            if (nu.m_isNC)
            {
                ncCount += weight;
                ncSelected->Fill(nu.m_eNu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 12)
            {
                nueFVCount += weight;
                nueFVSelected->Fill(nu.m_eNu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 14)
            {
                numuCount += weight;
                numuSelected->Fill(nu.m_eNu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 16)
            {
                nutauCount += weight;
                nutauSelected->Fill(nu.m_eNu, weight);
            }
            else
            {
                std::cout << "what is this" << std::endl;
                throw;
            }

            if (std::abs(nu.m_selShowerTruePdg) == 11)
            {
                electronCount += weight;
                electron->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (std::abs(nu.m_selShowerTruePdg) == 13)
            {
                muonCount += weight;
                muon->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (nu.m_selShowerTruePdg == 22)
            {
                photonCount += weight;
                photon->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (std::abs(nu.m_selShowerTruePdg) == 211)
            {
                pionCount += weight;
                pion->Fill(nu.m_selShowerPandrizzleScore, weight);

                if (nu.m_isNC)
                    pion_NC += weight;
                else if (std::abs(nu.m_nuPdg) == 14)
                    pion_numu += weight;
                else if (std::abs(nu.m_nuPdg) == 16)
                    pion_nutau += weight;

            }
            else if (nu.m_selShowerTruePdg == 2212)
            {
                protonCount += weight;
                proton->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (nu.m_selShowerTruePdg == -9999)
            {
                noShowerCount += weight;
            }
            else
            {
                otherCount += weight;
                other->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
        }
    }

    // Integrated flavour metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "Out of FV anue/nue: " << nueFVCount * 100/selectedBackground << "%" << std::endl;
    std::cout << "numu/anumu: " << numuCount * 100/selectedBackground << "%" << std::endl;
    std::cout << "nutau/anutau: " << nutauCount * 100/selectedBackground << "%" << std::endl;
    std::cout << "NC: " << ncCount * 100/selectedBackground << "%" << std::endl;

    // Integrated pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron: " << electronCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "muon: " << muonCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "photon: " << photonCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "pion: " << pionCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "proton: " << protonCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "no shower:" << noShowerCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "other: " << otherCount * 100.0 / selectedBackground << "%" << std::endl;

    std::cout << "pion_numu: " << pion_numu / pionCount * 100.00 << "%" << std::endl;
    std::cout << "pion_nutau: " << pion_nutau / pionCount * 100.00 << "%" << std::endl;
    std::cout << "pion_NC: " << pion_NC / pionCount * 100.00 << "%" << std::endl;


    // Draw those histograms
    TCanvas * c1 = new TCanvas("flavour decomposition", "flavour decomposition");
    THStack * flavourStack = new THStack("flavourStack", "Selected Background Decomposition;True Neutrino Energy [GeV]; nEvents");

    nueFVSelected->SetFillColor(kGray);
    numuSelected->SetFillColor(kBlue);
    nutauSelected->SetFillColor(kGreen);
    ncSelected->SetFillColor(kRed);

    nueFVSelected->SetFillStyle(3144);
    numuSelected->SetFillStyle(3144);
    nutauSelected->SetFillStyle(3144);
    ncSelected->SetFillStyle(3144);

    flavourStack->Add(numuSelected);
    flavourStack->Add(nutauSelected);
    flavourStack->Add(ncSelected);
    flavourStack->Add(nueFVSelected);
    flavourStack->Draw("hist");

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(numuSelected, "CC #nu_{#mu} + #bar{#nu_{#mu}}", "f");
    legend->AddEntry(nueFVSelected, "CC #nu_{e} + CC #bar{#nu_{e}} out of FV", "f"); // and not interacting on LAr
    legend->AddEntry(nutauSelected, "CC #nu_{#tau} + #bar{#nu_{#tau}}", "f"); 
    legend->AddEntry(ncSelected, "NC", "f");
    legend->Draw("same");

    TCanvas * c2 = new TCanvas("pdg decomposition", "pdg decomposition");
    THStack * pdgStack = new THStack("pdgStack", "Selected Background Decomposition;Pandrizzle Score;nEvents");

    electron->SetFillColor(kGray);
    muon->SetFillColor(kBlack);
    photon->SetFillColor(kBlue);
    pion->SetFillColor(kGreen);
    proton->SetFillColor(kRed);
    other->SetFillColor(kBlack);

    electron->SetFillStyle(3144);
    muon->SetFillStyle(3144);
    photon->SetFillStyle(3144);
    pion->SetFillStyle(3144);
    proton->SetFillStyle(3144);
    other->SetFillStyle(3144);

    pdgStack->Add(electron);
    pdgStack->Add(muon);
    pdgStack->Add(photon);
    pdgStack->Add(pion);
    pdgStack->Add(proton);
    pdgStack->Add(other);

    pdgStack->Draw("hist");

    auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
    legend2->AddEntry(electron, "e^{#pm}", "f");
    legend2->AddEntry(muon, "#mu^{#pm}", "f");
    legend2->AddEntry(photon, "#gamma", "f");
    legend2->AddEntry(pion, "#pi^{#pm}", "f");
    legend2->AddEntry(proton, "proton", "f");
    legend2->AddEntry(other, "other", "f");

    legend2->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void SelectedNumuBackgroundPlots(const NeutrinoEventVector &nuVector)
{
    // Numu selected shower PDG
    int nPandrizzleBins(50);
    double minPandrizzle(-1.0), maxPandrizzle(1.0);

    double electronCount_shower(0.0), muonCount_shower(0.0), photonCount_shower(0.0), pionCount_shower(0.0), protonCount_shower(0.0), otherCount_shower(0.0);
    TH1D * electron_shower = new TH1D("electron_shower", "electron_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * muon_shower = new TH1D("muon_shower", "muon_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * photon_shower = new TH1D("photon_shower", "photon_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * pion_shower = new TH1D("pion_shower", "pion_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton_shower = new TH1D("proton_shower", "proton_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * other_shower = new TH1D("other_shower", "other_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);

    // Numu selected track PDG
    int nPandizzleBins(50);
    double minPandizzle(-1.0), maxPandizzle(1.0);

    double electronCount_track(0.0), muonCount_track(0.0), photonCount_track(0.0), pionCount_track(0.0), protonCount_track(0.0), otherCount_track(0.0), noTrackCount(0.0);
    TH1D * electron_track = new TH1D("electron_track", "electron_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * muon_track = new TH1D("muon_track", "muon_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * photon_track = new TH1D("photon_track", "photon_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * pion_track = new TH1D("pion_track", "pion_track", nPandizzleBins, minPandizzle, maxPandizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton_track = new TH1D("proton_track", "proton_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * other_track = new TH1D("other_track", "other_track", nPandizzleBins, minPandizzle, maxPandizzle);

    double selectedBackground(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Look at background
        if (IsNueFlavourCCSignal(nu))
            continue;

        // Look at anumu/numu CC background
        if ((std::abs(nu.m_nuPdg) != 14) || nu.m_isNC)
            continue;

        // Apply Selection
        if ((!PERFORM_OLD_NUE_SELECTION && IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)))
        {
            const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
            selectedBackground += weight;

            // Fill selected shower histograms
            if (std::abs(nu.m_selShowerTruePdg) == 11)
            {
                electronCount_shower += weight;
                electron_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (std::abs(nu.m_selShowerTruePdg) == 13)
            {
                muonCount_shower += weight;
                muon_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (nu.m_selShowerTruePdg == 22)
            {
                photonCount_shower += weight;
                photon_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (std::abs(nu.m_selShowerTruePdg) == 211)
            {
                pionCount_shower += weight;
                pion_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (nu.m_selShowerTruePdg == 2212)
            {
                protonCount_shower += weight;
                proton_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else
            {
                otherCount_shower += weight;
                other_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }

            // Fill selected track histograms
            if (std::abs(nu.m_selTrackTruePdg) == 11)
            {
                electronCount_track += weight;
                electron_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 13)
            {
                muonCount_track += weight;
                muon_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 22)
            {
                photonCount_track += weight;
                photon_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 211)
            {
                pionCount_track += weight;
                pion_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 2212)
            {
                protonCount_track += weight;
                proton_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == -9999)
            {
                noTrackCount += weight;
            }
            else // i think this will include tracks that wont exist?
            {
                otherCount_track += weight;
                other_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
        }
    }

    // Integrated shower pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron_shower: " << electronCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "muon_shower: " << muonCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "photon_shower: " << photonCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "pion_shower: " << pionCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "proton_shower: " << protonCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "other_shower: " << otherCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron_track: " << electronCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "muon_track: " << muonCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "photon_track: " << photonCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "pion_track: " << pionCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "proton_track: " << protonCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "noTrackCount: " << noTrackCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "other_track: " << otherCount_track * 100.0 / selectedBackground << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("selected numu shower pdg decomposition", "selected numu shower pdg decomposition");
    THStack * showerStack = new THStack("showerStack", "Background Selected #nu_{#mu}/#bar{#nu_{#mu}} Shower PDG;Pandrizzle Score;nEvents");

    electron_shower->SetFillColor(kGray);
    muon_shower->SetFillColor(kBlack);
    photon_shower->SetFillColor(kBlue);
    pion_shower->SetFillColor(kGreen);
    proton_shower->SetFillColor(kRed);
    other_shower->SetFillColor(kBlack);

    electron_shower->SetFillStyle(3144);
    muon_shower->SetFillStyle(3144);
    photon_shower->SetFillStyle(3144);
    pion_shower->SetFillStyle(3144);
    proton_shower->SetFillStyle(3144);
    other_shower->SetFillStyle(3144);

    showerStack->Add(electron_shower);
    showerStack->Add(muon_shower);
    showerStack->Add(photon_shower);
    showerStack->Add(pion_shower);
    showerStack->Add(proton_shower);
    showerStack->Add(other_shower);

    showerStack->Draw("hist");

    auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
    legend1->AddEntry(electron_shower, "e^{#pm}", "f");
    legend1->AddEntry(muon_shower, "#mu^{#pm}", "f");
    legend1->AddEntry(photon_shower, "#gamma", "f");
    legend1->AddEntry(pion_shower, "#pi^{#pm}", "f");
    legend1->AddEntry(proton_shower, "proton", "f");
    legend1->AddEntry(other_shower, "other", "f");

    legend1->Draw("same");

    // Draw those histograms
    TCanvas * c2 = new TCanvas("selected numu track pdg decomposition", "selected numu track pdg decomposition");
    THStack * trackStack = new THStack("trackStack", "Background Selected #nu_{#mu}/#bar{#nu_{#mu}} Track PDG;Pandrizzle Score;nEvents");

    electron_track->SetFillColor(kGray);
    muon_track->SetFillColor(kBlack);
    photon_track->SetFillColor(kBlue);
    pion_track->SetFillColor(kGreen);
    proton_track->SetFillColor(kRed);
    other_track->SetFillColor(kBlack);

    electron_track->SetFillStyle(3144);
    muon_track->SetFillStyle(3144);
    photon_track->SetFillStyle(3144);
    pion_track->SetFillStyle(3144);
    proton_track->SetFillStyle(3144);
    other_track->SetFillStyle(3144);

    trackStack->Add(electron_track);
    trackStack->Add(muon_track);
    trackStack->Add(photon_track);
    trackStack->Add(pion_track);
    trackStack->Add(proton_track);
    trackStack->Add(other_track);

    trackStack->Draw("hist");

    auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
    legend2->AddEntry(electron_track, "e^{#pm}", "f");
    legend2->AddEntry(muon_track, "#mu^{#pm}", "f");
    legend2->AddEntry(photon_track, "#gamma", "f");
    legend2->AddEntry(pion_track, "#pi^{#pm}", "f");
    legend2->AddEntry(proton_track, "proton", "f");
    legend2->AddEntry(other_track, "other", "f");

    legend2->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void SelectedNCBackgroundPlots(const NeutrinoEventVector &nuVector)
{
    // NC selected shower PDG
    int nPandrizzleBins(50);
    double minPandrizzle(-1.0), maxPandrizzle(1.0);

    double electronCount_shower(0.0), muonCount_shower(0.0), photonCount_shower(0.0), pionCount_shower(0.0), protonCount_shower(0.0), otherCount_shower(0.0);
    TH1D * electron_shower = new TH1D("electron_shower", "electron_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * muon_shower = new TH1D("muon_shower", "muon_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * photon_shower = new TH1D("photon_shower", "photon_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * pion_shower = new TH1D("pion_shower", "pion_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton_shower = new TH1D("proton_shower", "proton_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * other_shower = new TH1D("other_shower", "other_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);

    TH1D * allPhoton_shower = new TH1D("allPhoton_shower", "allPhoton_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * allElectron_shower = new TH1D("allElectron_shower", "allElectron_shower", nPandrizzleBins, minPandrizzle, maxPandrizzle);

    // NC selected track PDG
    int nPandizzleBins(50);
    double minPandizzle(-1.0), maxPandizzle(1.0);

    double electronCount_track(0.0), muonCount_track(0.0), photonCount_track(0.0), pionCount_track(0.0), protonCount_track(0.0), otherCount_track(0.0);
    TH1D * electron_track = new TH1D("electron_track", "electron_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * muon_track = new TH1D("muon_track", "muon_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * photon_track = new TH1D("photon_track", "photon_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * pion_track = new TH1D("pion_track", "pion_track", nPandizzleBins, minPandizzle, maxPandizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton_track = new TH1D("proton_track", "proton_track", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * other_track = new TH1D("other_track", "other_track", nPandizzleBins, minPandizzle, maxPandizzle);

    double selectedBackground(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));

        // This is me testing the photon/electron separation of all events... 
        if (nu.m_selShowerTruePdg == 22)
            allPhoton_shower->Fill(nu.m_selShowerPandrizzleScore, weight);

        if (std::abs(nu.m_selShowerTruePdg) == 11)
            allElectron_shower->Fill(nu.m_selShowerPandrizzleScore, weight);

        // Look at background 
        if (IsNueFlavourCCSignal(nu))
            continue;

        // Look at NC events
        if (!nu.m_isNC)
            continue;

        // Apply Selection
        if ((!PERFORM_OLD_NUE_SELECTION && IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)))
        {
            selectedBackground += weight;

            // Fill selected shower histograms
            if (std::abs(nu.m_selShowerTruePdg) == 11)
            {
                electronCount_shower += weight;
                electron_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (std::abs(nu.m_selShowerTruePdg) == 13)
            {
                muonCount_shower += weight;
                muon_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (nu.m_selShowerTruePdg == 22)
            {
                photonCount_shower += weight;
                photon_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (std::abs(nu.m_selShowerTruePdg) == 211)
            {
                pionCount_shower += weight;
                pion_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else if (nu.m_selShowerTruePdg == 2212)
            {
                protonCount_shower += weight;
                proton_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }
            else
            {
                otherCount_shower += weight;
                other_shower->Fill(nu.m_selShowerPandrizzleScore, weight);
            }

            // Fill selected track histograms
            if (std::abs(nu.m_selTrackTruePdg) == 11)
            {
                electronCount_track += weight;
                electron_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 13)
            {
                muonCount_track += weight;
                muon_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 22)
            {
                photonCount_track += weight;
                photon_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 211)
            {
                pionCount_track += weight;
                pion_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 2212)
            {
                protonCount_track += weight;
                proton_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else // i think this will include tracks that wont exist?
            {
                otherCount_track += weight;
                other_track->Fill(nu.m_selTrackPandizzleScore, weight);
            }
        }
    }

    TCanvas * c4 = new TCanvas("jam", "jam");
    allPhoton_shower->SetLineColor(kRed);
    allPhoton_shower->Draw("hist");
    allElectron_shower->SetLineColor(kBlue);
    allElectron_shower->Draw("hist same");

    // Integrated shower pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron_shower: " << electronCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "muon_shower: " << muonCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "photon_shower: " << photonCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "pion_shower: " << pionCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "proton_shower: " << protonCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "other_shower: " << otherCount_shower * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron_track: " << electronCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "muon_track: " << muonCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "photon_track: " << photonCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "pion_track: " << pionCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "proton_track: " << protonCount_track * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "other_track: " << otherCount_track * 100.0 / selectedBackground << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("selected NC shower pdg decomposition", "selected NC shower pdg decomposition");
    THStack * showerStack = new THStack("showerStack", "Background Selected NC Shower PDG;Pandrizzle Score;nEvents");

    electron_shower->SetFillColor(kGray);
    muon_shower->SetFillColor(kBlack);
    photon_shower->SetFillColor(kBlue);
    pion_shower->SetFillColor(kGreen);
    proton_shower->SetFillColor(kRed);
    other_shower->SetFillColor(kBlack);

    electron_shower->SetFillStyle(3144);
    muon_shower->SetFillStyle(3144);
    photon_shower->SetFillStyle(3144);
    pion_shower->SetFillStyle(3144);
    proton_shower->SetFillStyle(3144);
    other_shower->SetFillStyle(3144);

    showerStack->Add(electron_shower);
    showerStack->Add(muon_shower);
    showerStack->Add(photon_shower);
    showerStack->Add(pion_shower);
    showerStack->Add(proton_shower);
    showerStack->Add(other_shower);

    showerStack->Draw("hist");

    auto legend1 = new TLegend(0.1,0.7,0.48,0.9);
    legend1->AddEntry(electron_shower, "e^{#pm}", "f");
    legend1->AddEntry(muon_shower, "#mu^{#pm}", "f");
    legend1->AddEntry(photon_shower, "#gamma", "f");
    legend1->AddEntry(pion_shower, "#pi^{#pm}", "f");
    legend1->AddEntry(proton_shower, "proton", "f");
    legend1->AddEntry(other_shower, "other", "f");

    legend1->Draw("same");

    // Draw those histograms
    TCanvas * c2 = new TCanvas("selected NC track pdg decomposition", "selected NC track pdg decomposition");
    THStack * trackStack = new THStack("trackStack", "Background Selected NC Track PDG;Pandizzle Score;nEvents");

    electron_track->SetFillColor(kGray);
    muon_track->SetFillColor(kBlack);
    photon_track->SetFillColor(kBlue);
    pion_track->SetFillColor(kGreen);
    proton_track->SetFillColor(kRed);
    other_track->SetFillColor(kBlack);

    electron_track->SetFillStyle(3144);
    muon_track->SetFillStyle(3144);
    photon_track->SetFillStyle(3144);
    pion_track->SetFillStyle(3144);
    proton_track->SetFillStyle(3144);
    other_track->SetFillStyle(3144);

    trackStack->Add(electron_track);
    trackStack->Add(muon_track);
    trackStack->Add(photon_track);
    trackStack->Add(pion_track);
    trackStack->Add(proton_track);
    trackStack->Add(other_track);

    trackStack->Draw("hist");

    auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
    legend2->AddEntry(electron_track, "e^{#pm}", "f");
    legend2->AddEntry(muon_track, "#mu^{#pm}", "f");
    legend2->AddEntry(photon_track, "#gamma", "f");
    legend2->AddEntry(pion_track, "#pi^{#pm}", "f");
    legend2->AddEntry(proton_track, "proton", "f");
    legend2->AddEntry(other_track, "other", "f");

    legend2->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void RejectedSignalPlots(const NeutrinoEventVector &nuVector)
{
    // See the distribution?
    int nEnergyBins(40);
    double minEnergy(0.0), maxEnergy(10.0);
    TH1D * nueRejected = new TH1D("nueRejected", "nueRejected", nEnergyBins, minEnergy, maxEnergy); // energy distribution of the rejected signal

    // Rejected shower PDG
    int nPandrizzleBins(50);
    double minPandrizzle(-1.0), maxPandrizzle(1.0);

    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * muon = new TH1D("muon", "muon", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * photon = new TH1D("photon", "photon", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * pion = new TH1D("pion", "pion", nPandrizzleBins, minPandrizzle, maxPandrizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * other = new TH1D("other", "other", nPandrizzleBins, minPandrizzle, maxPandrizzle);

    double selectedSignal(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Move on if not selection signal
        if (!IsNueCCSignal(nu, IS_NEUTRINO))
            continue;

        // Look at signal events that fail selection
        if ((!PERFORM_OLD_NUE_SELECTION && IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)))
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        selectedSignal += weight;

        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            electronCount += weight;
            electron->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 13)
        {
            muonCount += weight;
            muon->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == 22)
        {
            photonCount += weight;
            photon->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 211)
        {
            pionCount += weight;
            pion->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == 2212)
        {
            protonCount += weight;
            proton->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else
        {
            otherCount += weight;
            other->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
    }

    // Integrated pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron: " << electronCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "muon: " << muonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "photon: " << photonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "pion: " << pionCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "proton: " << protonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "other: " << otherCount * 100.0 / selectedSignal << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("pdg decomposition", "pdg decomposition");
    THStack * pdgStack = new THStack("pdgStack", "Rejected Signal Decomposition;Pandrizzle Score;nEvents");

    electron->SetFillColor(kGray);
    muon->SetFillColor(kBlack);
    photon->SetFillColor(kBlue);
    pion->SetFillColor(kGreen);
    proton->SetFillColor(kRed);
    other->SetFillColor(kBlack);

    electron->SetFillStyle(3144);
    muon->SetFillStyle(3144);
    photon->SetFillStyle(3144);
    pion->SetFillStyle(3144);
    proton->SetFillStyle(3144);
    other->SetFillStyle(3144);

    pdgStack->Add(electron);
    pdgStack->Add(muon);
    pdgStack->Add(photon);
    pdgStack->Add(pion);
    pdgStack->Add(proton);
    pdgStack->Add(other);

    pdgStack->Draw("hist");

    auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
    legend2->AddEntry(electron, "e^{#pm}", "f");
    legend2->AddEntry(muon, "#mu^{#pm}", "f");
    legend2->AddEntry(photon, "#gamma", "f");
    legend2->AddEntry(pion, "#pi^{#pm}", "f");
    legend2->AddEntry(proton, "proton", "f");
    legend2->AddEntry(other, "other", "f");

    legend2->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

// Sanity check plots
void SelectedSignalPlots(const NeutrinoEventVector &nuVector)
{
    // See the distribution?
    int nEnergyBins(40);
    double minEnergy(0.0), maxEnergy(10.0);
    TH1D * nueSelected = new TH1D("nueSelected", "nueSelected", nEnergyBins, minEnergy, maxEnergy); // energy distribution of the rejected signal

    // Selected shower PDG
    int nPandrizzleBins(50);
    double minPandrizzle(-1.0), maxPandrizzle(1.0);

    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * muon = new TH1D("muon", "muon", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * photon = new TH1D("photon", "photon", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * pion = new TH1D("pion", "pion", nPandrizzleBins, minPandrizzle, maxPandrizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nPandrizzleBins, minPandrizzle, maxPandrizzle);
    TH1D * other = new TH1D("other", "other", nPandrizzleBins, minPandrizzle, maxPandrizzle);

    double selectedSignal(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        /*
        // Move on if not selection signal
        if (!IsNueCCSignal(nu, IS_NEUTRINO))
            continue;
        */
        
        // Look at signal events that fail selection
        bool passNueSelection(IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION));

        if (!passNueSelection)
            continue;
        
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        selectedSignal += weight;

        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            electronCount += weight;
            electron->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 13)
        {
            muonCount += weight;
            muon->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == 22)
        {
            photonCount += weight;
            photon->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 211)
        {
            pionCount += weight;
            pion->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == 2212)
        {
            protonCount += weight;
            proton->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
        else
        {
            otherCount += weight;
            other->Fill(nu.m_selShowerPandrizzleScore, weight);
        }
    }

    // Integrated pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron: " << electronCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "muon: " << muonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "photon: " << photonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "pion: " << pionCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "proton: " << protonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "other: " << otherCount * 100.0 / selectedSignal << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("pdg decomposition", "pdg decomposition");
    THStack * pdgStack = new THStack("pdgStack", "Selected Signal Decomposition;Pandrizzle Score;nEvents");

    electron->SetFillColor(kGray);
    muon->SetFillColor(kBlack);
    photon->SetFillColor(kBlue);
    pion->SetFillColor(kGreen);
    proton->SetFillColor(kRed);
    other->SetFillColor(kBlack);

    electron->SetFillStyle(3144);
    muon->SetFillStyle(3144);
    photon->SetFillStyle(3144);
    pion->SetFillStyle(3144);
    proton->SetFillStyle(3144);
    other->SetFillStyle(3144);

    pdgStack->Add(electron);
    pdgStack->Add(muon);
    pdgStack->Add(photon);
    pdgStack->Add(pion);
    pdgStack->Add(proton);
    pdgStack->Add(other);

    pdgStack->Draw("hist");

    auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
    legend2->AddEntry(electron, "e^{#pm}", "f");
    legend2->AddEntry(muon, "#mu^{#pm}", "f");
    legend2->AddEntry(photon, "#gamma", "f");
    legend2->AddEntry(pion, "#pi^{#pm}", "f");
    legend2->AddEntry(proton, "proton", "f");
    legend2->AddEntry(other, "other", "f");

    legend2->Draw("same");
}

//------------------------------------------------------------------------------------------------------------------------------------------

// Sanity check plots
void SelectedEnergyPlots(const NeutrinoEventVector &nuVector)
{
    // See the distribution?
    int nEnergyBins(32);
    double minEnergy(0.0), maxEnergy(8.0);
    TH1D * nueSelected = new TH1D("nueSelected", "nueSelected", nEnergyBins, minEnergy, maxEnergy); 

    // Selected shower PDG
    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nEnergyBins, minEnergy, maxEnergy);
    TH1D * muon = new TH1D("muon", "muon", nEnergyBins, minEnergy, maxEnergy);
    TH1D * photon = new TH1D("photon", "photon", nEnergyBins, minEnergy, maxEnergy);
    TH1D * pion = new TH1D("pion", "pion", nEnergyBins, minEnergy, maxEnergy); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nEnergyBins, minEnergy, maxEnergy);
    TH1D * other = new TH1D("other", "other", nEnergyBins, minEnergy, maxEnergy);

    double selectedSignal(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        bool passNueSelection(IsNueSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION));

        if (!passNueSelection)
            continue;
        
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        selectedSignal += weight;

        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            electronCount += weight;
            electron->Fill(nu.m_nueRecoENu, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 13)
        {
            muonCount += weight;
            muon->Fill(nu.m_nueRecoENu, weight);
        }
        else if (nu.m_selShowerTruePdg == 22)
        {
            photonCount += weight;
            photon->Fill(nu.m_nueRecoENu, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 211)
        {
            pionCount += weight;
            pion->Fill(nu.m_nueRecoENu, weight);
        }
        else if (nu.m_selShowerTruePdg == 2212)
        {
            protonCount += weight;
            proton->Fill(nu.m_nueRecoENu, weight);
        }
        else
        {
            otherCount += weight;
            other->Fill(nu.m_nueRecoENu, weight);
        }
    }

    // Integrated pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron: " << electronCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "muon: " << muonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "photon: " << photonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "pion: " << pionCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "proton: " << protonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "other: " << otherCount * 100.0 / selectedSignal << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("pdg decomposition", "pdg decomposition");
    THStack * pdgStack = new THStack("pdgStack", "Selected Signal Decomposition;Nu Reco Energy [GeV];nEvents");

    electron->SetFillColor(kGray);
    muon->SetFillColor(kBlack);
    photon->SetFillColor(kBlue);
    pion->SetFillColor(kGreen);
    proton->SetFillColor(kRed);
    other->SetFillColor(kBlack);

    electron->SetFillStyle(3144);
    muon->SetFillStyle(3144);
    photon->SetFillStyle(3144);
    pion->SetFillStyle(3144);
    proton->SetFillStyle(3144);
    other->SetFillStyle(3144);

    pdgStack->Add(electron);
    pdgStack->Add(muon);
    pdgStack->Add(photon);
    pdgStack->Add(pion);
    pdgStack->Add(proton);
    pdgStack->Add(other);

    pdgStack->Draw("hist");

    auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
    legend2->AddEntry(electron, "e^{#pm}", "f");
    legend2->AddEntry(muon, "#mu^{#pm}", "f");
    legend2->AddEntry(photon, "#gamma", "f");
    legend2->AddEntry(pion, "#pi^{#pm}", "f");
    legend2->AddEntry(proton, "proton", "f");
    legend2->AddEntry(other, "other", "f");

    legend2->Draw("same");
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
