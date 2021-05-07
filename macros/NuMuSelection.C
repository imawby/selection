#include "NuSelection.h"

void NuSelection(const std::string &inputFileName)
{
    NeutrinoEventVector neutrinoEventVector;
    ReadFile(inputFileName, neutrinoEventVector);

    NuSelectionHistogramCollection numuHistogramCollection;
    InitialiseNuSelectionHistogramCollection(numuHistogramCollection, "numu");

    PerformSelection(neutrinoEventVector, numuHistogramCollection);

    ProcessHistogramCollection(numuHistogramCollection);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ReadFile(const std::string &inputFileName, NeutrinoEventVector &nuEventVector)
{
    TFile *inputFile = new TFile(inputFileName.c_str());
    TTree *tree = (TTree*)inputFile->Get("ccnuselection/ccnusel");

    NeutrinoEvent nu;

    tree->SetBranchAddress("Run", &nu.m_run);
    tree->SetBranchAddress("SubRun", &nu.m_subrun);
    tree->SetBranchAddress("Event", &nu.m_event);
    tree->SetBranchAddress("NC", &nu.m_isNC);
    tree->SetBranchAddress("BeamPdg", &nu.m_beamPdg);
    tree->SetBranchAddress("NuPdg", &nu.m_nuPdg);
    tree->SetBranchAddress("ProjectedPOTWeight", &nu.m_projectedPOTWeight);
    tree->SetBranchAddress("TargetZ", &nu.m_target);
    tree->SetBranchAddress("Enu", &nu.m_eNu);
    tree->SetBranchAddress("Q2", &nu.m_qSqr);
    tree->SetBranchAddress("MomLepT", &nu.m_lepMom);
    tree->SetBranchAddress("LepNuAngle", &nu.m_lepNuOpeningAngle);
    tree->SetBranchAddress("NueRecoENu", &nu.m_nueRecoENu);
    tree->SetBranchAddress("NumuRecoENu", &nu.m_numuRecoENu);
    tree->SetBranchAddress("RecoNuVtxX", &nu.m_nuRecoVertexX);
    tree->SetBranchAddress("RecoNuVtxY", &nu.m_nuRecoVertexY);
    tree->SetBranchAddress("RecoNuVtxZ", &nu.m_nuRecoVertexZ);
    tree->SetBranchAddress("SelTrackPandizzleVar", &nu.m_selTrackPandizzleScore);
    tree->SetBranchAddress("SelTrackMVAElectron", &nu.m_selShowerElectronScore);
    tree->SetBranchAddress("SelShowerRecoStartX", &nu.m_selShowerRecoStartX);
    tree->SetBranchAddress("SelShowerRecoStartY", &nu.m_selShowerRecoStartY);
    tree->SetBranchAddress("SelShowerRecoStartZ", &nu.m_selShowerRecoStartZ);

    for (Int_t i = 0; i < tree->GetEntries(); ++i)
    {
        tree->GetEntry(i);
        nuEventVector.push_back(nu);
    }
    /*
    for (const NeutrinoEvent &neutrino : nuEventVector)
    {
        if (IsNumuCCSignal(neutrino))
            std::cout << "JAM" << std::endl;
    }
    */
}

//------------------------------------------------------------------------------------------------------------------------------------------

void InitialiseNuSelectionHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection, const std::string &histPrefix)
{
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_eNuHists, histPrefix + "_ENu", 50, 0.0, 10.0);
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_qSqrHists, histPrefix + "_QSqr", 50, 0.0, 10.0);
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_lepMomHists, histPrefix + "_LepMom", 50, 0.0, 10.0);
    InitialiseNuSelectionHistograms(nuSelectionHistogramCollection.m_lepNuOpeningAngleHists, histPrefix + "_LepNuOpeningAngle", 50, 0.0, 18.0);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void InitialiseNuSelectionHistograms(NuSelectionHistograms &nuSelectionHistograms, const std::string &histPrefix, const unsigned int nBins, const float xMin, const float xMax)
{
    nuSelectionHistograms.m_signal = new TH1D((histPrefix + "_Signal").c_str(), (histPrefix + "_Signal").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_background = new TH1D((histPrefix + "_Background").c_str(), (histPrefix + "_Background").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_selected = new TH1D((histPrefix + "_Selected").c_str(), (histPrefix + "_Selected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_signalSelected = new TH1D((histPrefix + "_SignalSelected").c_str(), (histPrefix + "_SignalSelected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_backgroundRejected = new TH1D((histPrefix + "_BackgroundRejected").c_str(), (histPrefix + "_BackgroundRejected").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_efficiency = new TH1D((histPrefix + "_Efficiency").c_str(), (histPrefix + "_Efficiency").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_purity = new TH1D((histPrefix + "_Purity").c_str(), (histPrefix + "_Purity").c_str(), nBins, xMin, xMax);
    nuSelectionHistograms.m_backgroundRejection = new TH1D((histPrefix + "_BackgroundRejection").c_str(), (histPrefix + "_BackgroundRejection").c_str(), nBins, xMin, xMax);
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsInFiducialVolume(const NeutrinoEvent &nu)
{
    const double minX(-360.0 + 50.0), maxX(360.0 - 50.0);
    const double minY(-600.0 + 50.0), maxY(600.0 - 50.0);
    const double minZ(50.0), maxZ(1394.0 - 150.0);

    if ((nu.m_nuRecoVertexX < minX) || (nu.m_nuRecoVertexX > maxX))
        return false;

    if ((nu.m_nuRecoVertexY < minY) || (nu.m_nuRecoVertexY > maxY))
        return false;

    if ((nu.m_nuRecoVertexZ < minZ) || (nu.m_nuRecoVertexZ > maxZ))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNueCCSignal(const NeutrinoEvent &nu)
{
    if (std::fabs(nu.m_nuPdg) != 12)
        return false;

    if (!IsInFiducialVolume(nu))
        return false;

    if (nu.m_isNC)
        return false;

    // Interaction on liquid argon
    if (nu.m_target != 18)
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNueSelection(const NeutrinoEvent &nu)
{
    //ISOBEL TURN THESE INTO CONFIGURABLE PARAMETERS
    if (nu.m_selTrackPandizzleScore > 0.35)
        return false;

    if (nu.m_selShowerElectronScore < 0.0)
        return false;

    const double showerDisplacementX(nu.m_selShowerRecoStartX - nu.m_nuRecoVertexX);
    const double showerDisplacementY(nu.m_selShowerRecoStartY - nu.m_nuRecoVertexY);
    const double showerDisplacementZ(nu.m_selShowerRecoStartZ - nu.m_nuRecoVertexZ);
    const double showerDisplacementSquared((showerDisplacementX * showerDisplacementX) + (showerDisplacementY * showerDisplacementY) + (showerDisplacementZ * showerDisplacementZ));

    if (showerDisplacementSquared > (5.0 * 5.0))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNumuCCSignal(const NeutrinoEvent &nu)
{
    if (std::fabs(nu.m_nuPdg) != 14)
        return false;

    if (!IsInFiducialVolume(nu))
        return false;

    if (nu.m_isNC)
        return false;

    // Interaction on liquid argon
    if (nu.m_target != 18)
        return false;

   return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNumuSelection(const NeutrinoEvent &nu)
{
    //ISOBEL TURN THESE INTO CONFIGURABLE PARAMETERS
    if (nu.m_selTrackPandizzleScore < 0.05)
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void PerformSelection(const NeutrinoEventVector &nuVector, NuSelectionHistogramCollection &nueSelectionHistogramCollection, 
    NuSelectionHistogramCollection &numuSelectionHistogramCollection)
{
    for (const NeutrinoEvent &nu : nuVector)
    {
        const bool isNueCCSignal(IsNueCCSignal(nu));
        const bool isNumuCCSignal(isNueCCSignal ? false : IsNumuCCSignal(nu));

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));

        // Fill truth histograms
        if (isNueCCSignal)
        { 
            nueSelectionHistogramCollection.m_eNuHists.m_signal->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_signal->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_signal->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signal->Fill(nu.m_lepNuOpeningAngle, weight);
        }
        else
        {
            nueSelectionHistogramCollection.m_eNuHists.m_background->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_background->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_background->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_background->Fill(nu.m_lepNuOpeningAngle, weight);
        }

        if (isNumuCCSignal)
        { 
            numuSelectionHistogramCollection.m_eNuHists.m_signal->Fill(nu.m_eNu, weight);
            numuSelectionHistogramCollection.m_qSqrHists.m_signal->Fill(nu.m_qSqr, weight);
            numuSelectionHistogramCollection.m_lepMomHists.m_signal->Fill(nu.m_lepMom, weight);
            numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signal->Fill(nu.m_lepNuOpeningAngle, weight);
        }
        else
        {
            numuSelectionHistogramCollection.m_eNuHists.m_background->Fill(nu.m_eNu, weight);
            numuSelectionHistogramCollection.m_qSqrHists.m_background->Fill(nu.m_qSqr, weight);
            numuSelectionHistogramCollection.m_lepMomHists.m_background->Fill(nu.m_lepMom, weight);
            numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_background->Fill(nu.m_lepNuOpeningAngle, weight);
        }

        // Apply Selection
        if (PassNueSelection(nu))
        {
            nueSelectionHistogramCollection.m_eNuHists.m_selected->Fill(nu.m_eNu, weight);
            nueSelectionHistogramCollection.m_qSqrHists.m_selected->Fill(nu.m_qSqr, weight);
            nueSelectionHistogramCollection.m_lepMomHists.m_selected->Fill(nu.m_lepMom, weight);
            nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_selected->Fill(nu.m_lepNuOpeningAngle, weight);

            if (isNueCCSignal)
            {
                nueSelectionHistogramCollection.m_eNuHists.m_signalSelected->Fill(nu.m_eNu, weight);
                nueSelectionHistogramCollection.m_qSqrHists.m_signalSelected->Fill(nu.m_qSqr, weight);
                nueSelectionHistogramCollection.m_lepMomHists.m_signalSelected->Fill(nu.m_lepMom, weight);
                nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
            }
        }
        else
        {
            if (!isNueCCSignal)
            {
                nueSelectionHistogramCollection.m_eNuHists.m_backgroundRejected->Fill(nu.m_eNu, weight);
                nueSelectionHistogramCollection.m_qSqrHists.m_backgroundRejected->Fill(nu.m_qSqr, weight);
                nueSelectionHistogramCollection.m_lepMomHists.m_backgroundRejected->Fill(nu.m_lepMom, weight);
                nueSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_backgroundRejected->Fill(nu.m_lepNuOpeningAngle, weight);
            }

            if (PassNumuSelection(nu))
            {    
                numuSelectionHistogramCollection.m_eNuHists.m_selected->Fill(nu.m_eNu, weight);
                numuSelectionHistogramCollection.m_qSqrHists.m_selected->Fill(nu.m_qSqr, weight);
                numuSelectionHistogramCollection.m_lepMomHists.m_selected->Fill(nu.m_lepMom, weight);
                numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_selected->Fill(nu.m_lepNuOpeningAngle, weight);

                if (isNumuCCSignal)
                {
                    numuSelectionHistogramCollection.m_eNuHists.m_signalSelected->Fill(nu.m_eNu, weight);
                    numuSelectionHistogramCollection.m_qSqrHists.m_signalSelected->Fill(nu.m_qSqr, weight);
                    numuSelectionHistogramCollection.m_lepMomHists.m_signalSelected->Fill(nu.m_lepMom, weight);
                    numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_signalSelected->Fill(nu.m_lepNuOpeningAngle, weight);
                }
            }
            else
            {
                if (!isNumuCCSignal)
                {
                    numuSelectionHistogramCollection.m_eNuHists.m_backgroundRejected->Fill(nu.m_eNu, weight);
                    numuSelectionHistogramCollection.m_qSqrHists.m_backgroundRejected->Fill(nu.m_qSqr, weight);
                    numuSelectionHistogramCollection.m_lepMomHists.m_backgroundRejected->Fill(nu.m_lepMom, weight);
                    numuSelectionHistogramCollection.m_lepNuOpeningAngleHists.m_backgroundRejected->Fill(nu.m_lepNuOpeningAngle, weight);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ProcessHistogramCollection(NuSelectionHistogramCollection &nuSelectionHistogramCollection)
{
    ProcessHistograms(nuSelectionHistogramCollection.m_eNuHists);
    ProcessHistograms(nuSelectionHistogramCollection.m_qSqrHists);
    ProcessHistograms(nuSelectionHistogramCollection.m_lepMomHists);
    ProcessHistograms(nuSelectionHistogramCollection.m_lepNuOpeningAngleHists);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void ProcessHistograms(NuSelectionHistograms &nuSelectionHistograms)
{
    // throw if not same binning
    int nBins = nuSelectionHistograms.m_signal->GetXaxis()->GetNbins();

    // Efficiency
    for (int i = -1; i < nBins; ++i)
    {
        const double signalSelected(nuSelectionHistograms.m_signalSelected->GetBinContent(i + 1));
        const double signal(nuSelectionHistograms.m_signal->GetBinContent(i + 1));
        const float efficiency(signal > 0.0 ? signalSelected / signal : 0.0);
        nuSelectionHistograms.m_efficiency->SetBinContent(i + 1, efficiency);
    }

    // Purity
    for (int i = -1; i < nBins; ++i)
    {
        const double signalSelected(nuSelectionHistograms.m_signalSelected->GetBinContent(i + 1));
        const double selected(nuSelectionHistograms.m_selected->GetBinContent(i + 1));
        const float purity(selected > 0.0 ? signalSelected / selected : 0.0);
        nuSelectionHistograms.m_purity->SetBinContent(i + 1, purity);
    }

    // BackgroundRejected
    for (int i = -1; i < nBins; ++i)
    {
        const double backgroundRejected(nuSelectionHistograms.m_backgroundRejected->GetBinContent(i + 1));
        const double background(nuSelectionHistograms.m_background->GetBinContent(i + 1));
        const float backgroundRejection(background > 0.0 ? backgroundRejected / background : 0.0);
        nuSelectionHistograms.m_backgroundRejection->SetBinContent(i + 1, backgroundRejection);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

double GetOscWeight(const NeutrinoEvent &nu)
{
    double weight = getProb3ppWeights(DEF_SIN2THETA12, DEF_SIN2THETA13, DEF_SIN2THETA23, DEF_DLITTLEM2, DEF_DBIGM2, DEF_DELTA, nu.m_eNu, nu.m_beamPdg, nu.m_nuPdg);

    return weight;
}

//------------------------------------------------------------------------------------------------------------------------------------------
