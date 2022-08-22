#include "NeutrinoEvent.C"
#include "Signal.C"
#include "Selection.C"

void NumuBackground(const std::string &inputFileName);
void SelectedPlots(const NeutrinoEventVector &nuVector);
void SelectedBackgroundPlots(const NeutrinoEventVector &nuVector);
//void SelectedNumuBackgroundPlots(const NeutrinoEventVector &nuVector);
void RejectedSignalPlots(const NeutrinoEventVector &nuVector);
void RejectedSignalNumuLengthPlots(const NeutrinoEventVector &nuVector);
void SelectedSignalPlots(const NeutrinoEventVector &nuVector);
double GetOscWeight(const NeutrinoEvent &nu);

bool PERFORM_OLD_NUE_SELECTION = false;
bool IS_NEUTRINO = false;
bool IS_JAM_SELECTION = false;

void NumuBackground(const std::string &inputFileName)
{
    std::cout << "\033[31m" << "Performing " << "\033[33m" << (PERFORM_OLD_NUE_SELECTION ? "old " : "new ") << "\033[31m" << "nue selection" << "\033[0m" << std::endl; 

    NeutrinoEventVector neutrinoEventVector;
    ReadFile(inputFileName, neutrinoEventVector);

    std::cout << "HERE AAAA" << std::endl;

    SelectedPlots(neutrinoEventVector);
    //SelectedBackgroundPlots(neutrinoEventVector);
    //SelectedNumuBackgroundPlots(neutrinoEventVector);
    //RejectedSignalPlots(neutrinoEventVector);
    //RejectedSignalNumuLengthPlots(neutrinoEventVector);
    //SelectedSignalPlots(neutrinoEventVector);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void SelectedPlots(const NeutrinoEventVector &nuVector)
{

    std::cout << "HERE BBBBB" << std::endl;

    // Flavour background
    int nEnergyBins(32);
    double minEnergy(0.0), maxEnergy(8.0);

    double numuCount(0.0), numuFVCount(0.0), nueCount(0.0), nutauCount(0.0), ncCount(0.0);
    TH1D * numuSelected_True = new TH1D("numuSelected_True", "numuSelected_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * numuFVSelected_True = new TH1D("numuFVSelected_True", "numuFVSelected_True", nEnergyBins, minEnergy, maxEnergy); // out of FV anumu/numu
    TH1D * nueSelected_True = new TH1D("nueSelected_True", "nueSelected_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nutauSelected_True = new TH1D("nutauSelected_True", "nutauSelected_True", nEnergyBins, minEnergy, maxEnergy);
    TH1D * ncSelected_True = new TH1D("ncSelected_True", "ncSelected_True", nEnergyBins, minEnergy, maxEnergy);

    TH1D * numuSelected_Reco = new TH1D("numuSelected_Reco", "numuSelected_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * numuFVSelected_Reco = new TH1D("numuFVSelected_Reco", "numuFVSelected_Reco", nEnergyBins, minEnergy, maxEnergy); // out of FV anumu/numu
    TH1D * nueSelected_Reco = new TH1D("nueSelected_Reco", "nueSelected_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nutauSelected_Reco = new TH1D("nutauSelected_Reco", "nutauSelected_Reco", nEnergyBins, minEnergy, maxEnergy);
    TH1D * ncSelected_Reco = new TH1D("ncSelected_Reco", "ncSelected_Reco", nEnergyBins, minEnergy, maxEnergy);

    // Selected track PDG
    int nPandizzleBins(50);
    double minPandizzle(-1.0), maxPandizzle(1.0);

    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0), noTrackCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * muon = new TH1D("muon", "muon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * photon = new TH1D("photon", "photon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * pion = new TH1D("pion", "pion", nPandizzleBins, minPandizzle, maxPandizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * other = new TH1D("other", "other", nPandizzleBins, minPandizzle, maxPandizzle);


    double selected(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Apply numu selection
        if (IsNumuSelected(nu, IS_NEUTRINO, IS_JAM_SELECTION))
        {
            const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
            selected += weight;

            if (nu.m_isNC)
            {
                ncCount += weight;
                ncSelected_True->Fill(nu.m_eNu, weight);
                ncSelected_Reco->Fill(nu.m_numuRecoENu, weight);
            }
            else if ((std::abs(nu.m_nuPdg) == 14) && (IsNumuFlavourCCSignal(nu)))
            {
                numuCount += weight;
                numuSelected_True->Fill(nu.m_eNu, weight);
                numuSelected_Reco->Fill(nu.m_numuRecoENu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 14)
            {
                numuFVCount += weight;
                numuFVSelected_True->Fill(nu.m_eNu, weight);
                numuFVSelected_Reco->Fill(nu.m_numuRecoENu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 12)
            {
                nueCount += weight;
                nueSelected_True->Fill(nu.m_eNu, weight);
                nueSelected_Reco->Fill(nu.m_numuRecoENu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 16)
            {
                nutauCount += weight;
                nutauSelected_True->Fill(nu.m_eNu, weight);
                nutauSelected_Reco->Fill(nu.m_numuRecoENu, weight);
            }
            else
            {
                std::cout << "what is this" << std::endl;
                throw;
            }

            if (std::abs(nu.m_selTrackTruePdg) == 11)
            {
                electronCount += weight;
                electron->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 13)
            {
                muonCount += weight;
                muon->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 22)
            {
                photonCount += weight;
                photon->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 211)
            {
                pionCount += weight;
                pion->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 2212)
            {
                protonCount += weight;
                proton->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == -9999)
            {
                noTrackCount += weight;
            }
            else
            {
                otherCount += weight;
                other->Fill(nu.m_selTrackPandizzleScore, weight);
            }
        }
    }

    // Integrated flavour metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "numu/anumu: " << numuCount * 100/selected << "%" << std::endl;
    std::cout << "Out of FV anumu/numu: " << numuFVCount * 100/selected << "%" << std::endl;
    std::cout << "nue/anue: " << nueCount * 100/selected << "%" << std::endl;
    std::cout << "nutau/anutau: " << nutauCount * 100/selected << "%" << std::endl;
    std::cout << "NC: " << ncCount * 100/selected << "%" << std::endl;

    // Integrated pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron: " << electronCount * 100.0 / selected << "%" << std::endl;
    std::cout << "muon: " << muonCount * 100.0 / selected << "%" << std::endl;
    std::cout << "photon: " << photonCount * 100.0 / selected << "%" << std::endl;
    std::cout << "pion: " << pionCount * 100.0 / selected << "%" << std::endl;
    std::cout << "proton: " << protonCount * 100.0 / selected << "%" << std::endl;
    std::cout << "no track: " << noTrackCount * 100.0 / selected << "%" << std::endl;
    std::cout << "other: " << otherCount * 100.0 / selected << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("flavour decomposition true", "flavour decomposition true");

    numuSelected_True->SetFillColor(kBlack);
    numuFVSelected_True->SetFillColor(kGray);
    nueSelected_True->SetFillColor(kBlue);
    nutauSelected_True->SetFillColor(kGreen);
    ncSelected_True->SetFillColor(kRed);

    numuSelected_Reco->SetFillColor(kBlack);
    numuFVSelected_Reco->SetFillColor(kGray);
    nueSelected_Reco->SetFillColor(kBlue);
    nutauSelected_Reco->SetFillColor(kGreen);
    ncSelected_Reco->SetFillColor(kRed);

    numuSelected_True->SetLineColor(kBlack);
    numuFVSelected_True->SetLineColor(kGray);
    nueSelected_True->SetLineColor(kBlue);
    nutauSelected_True->SetLineColor(kGreen);
    ncSelected_True->SetLineColor(kRed);

    numuSelected_Reco->SetLineColor(kBlack);
    numuFVSelected_Reco->SetLineColor(kGray);
    nueSelected_Reco->SetLineColor(kBlue);
    nutauSelected_Reco->SetLineColor(kGreen);
    ncSelected_Reco->SetLineColor(kRed);

    numuSelected_True->SetFillStyle(3144);
    numuFVSelected_True->SetFillStyle(3144);
    nueSelected_True->SetFillStyle(3144);
    nutauSelected_True->SetFillStyle(3144);
    ncSelected_True->SetFillStyle(3144);

    numuSelected_Reco->SetFillStyle(3144);
    numuFVSelected_Reco->SetFillStyle(3144);
    nueSelected_Reco->SetFillStyle(3144);
    nutauSelected_Reco->SetFillStyle(3144);
    ncSelected_Reco->SetFillStyle(3144);

    THStack * flavourStack_True = new THStack("flavourStack_True", ";True Neutrino Energy [GeV]; nEvents");
    THStack * flavourStack_Reco = new THStack("flavourStack_Reco", ";Reco Neutrino Energy [GeV]; nEvents");

    flavourStack_True->Add(numuSelected_True);
    flavourStack_True->Add(nutauSelected_True);
    flavourStack_True->Add(ncSelected_True);
    flavourStack_True->Add(nueSelected_True);
    flavourStack_True->Add(numuFVSelected_True);
    flavourStack_True->Draw("hist");

    auto legend_True = new TLegend(0.1,0.7,0.48,0.9);
    legend_True->AddEntry(numuSelected_True, "CC #nu_{#mu} + CC #bar{#nu_{#mu}}", "f");
    legend_True->AddEntry(numuFVSelected_True, "CC #nu_{#mu} + CC #bar{#nu_{#mu}} out of FV", "f");
    legend_True->AddEntry(nueSelected_True, "CC #nu_{e} + #bar{#nu_{e}}", "f");
    legend_True->AddEntry(nutauSelected_True, "CC #nu_{#tau} + #bar{#nu_{#tau}}", "f");
    legend_True->AddEntry(ncSelected_True, "NC", "f");
    legend_True->Draw("same");

    TCanvas * c5 = new TCanvas("flavour decomposition reco", "flavour decomposition reco");

    flavourStack_Reco->Add(numuSelected_Reco);
    flavourStack_Reco->Add(nutauSelected_Reco);
    flavourStack_Reco->Add(ncSelected_Reco);
    flavourStack_Reco->Add(nueSelected_Reco);
    flavourStack_Reco->Add(numuFVSelected_Reco);
    flavourStack_Reco->Draw("hist");

    auto legend_Reco = new TLegend(0.1,0.7,0.48,0.9);
    legend_Reco->AddEntry(numuSelected_Reco, "CC #nu_{#mu} + CC #bar{#nu_{#mu}}", "f");
    legend_Reco->AddEntry(numuFVSelected_Reco, "CC #nu_{#mu} + CC #bar{#nu_{#mu}} out of FV", "f");
    legend_Reco->AddEntry(nueSelected_Reco, "CC #nu_{e} + #bar{#nu_{e}}", "f");
    legend_Reco->AddEntry(nutauSelected_Reco, "CC #nu_{#tau} + #bar{#nu_{#tau}}", "f");
    legend_Reco->AddEntry(ncSelected_Reco, "NC", "f");
    legend_Reco->Draw("same");

    TCanvas * c2 = new TCanvas("pdg decomposition", "pdg decomposition");
    THStack * pdgStack = new THStack("pdgStack", "Selected Background Decomposition;Pandizzle Score;nEvents");

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

void SelectedBackgroundPlots(const NeutrinoEventVector &nuVector)
{
    // Flavour background
    int nEnergyBins(32);
    double minEnergy(0.0), maxEnergy(8.0);

    double numuFVCount(0.0), nueCount(0.0), nutauCount(0.0), ncCount(0.0);
    TH1D * numuFVSelected = new TH1D("numuFVSelected", "numuFVSelected", nEnergyBins, minEnergy, maxEnergy); // out of FV anumu/numu
    TH1D * nueSelected = new TH1D("nueSelected", "nueSelected", nEnergyBins, minEnergy, maxEnergy);
    TH1D * nutauSelected = new TH1D("nutauSelected", "nutauSelected", nEnergyBins, minEnergy, maxEnergy);
    TH1D * ncSelected = new TH1D("ncSelected", "ncSelected", nEnergyBins, minEnergy, maxEnergy);

    // Selected track PDG
    int nPandizzleBins(50);
    double minPandizzle(-1.0), maxPandizzle(1.0);

    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0), noTrackCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * muon = new TH1D("muon", "muon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * photon = new TH1D("photon", "photon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * pion = new TH1D("pion", "pion", nPandizzleBins, minPandizzle, maxPandizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * other = new TH1D("other", "other", nPandizzleBins, minPandizzle, maxPandizzle);

    double selectedBackground(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Looking at selected background
        if (IsNumuFlavourCCSignal(nu))
            continue;

        // Apply nue selection
        bool isNueSelected((!PERFORM_OLD_NUE_SELECTION && PassNueSelection(nu, IS_NEUTRINO)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)));

        if (isNueSelected)
            continue;

        // Look at numu selected background
        if (PassNumuSelection(nu, IS_NEUTRINO))
        {
            const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
            selectedBackground += weight;

            if (nu.m_isNC)
            {
                ncCount += weight;
                ncSelected->Fill(nu.m_eNu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 14)
            {
                numuFVCount += weight;
                numuFVSelected->Fill(nu.m_eNu, weight);
            }
            else if (std::abs(nu.m_nuPdg) == 12)
            {
                nueCount += weight;
                nueSelected->Fill(nu.m_eNu, weight);
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

            if (std::abs(nu.m_selTrackTruePdg) == 11)
            {
                electronCount += weight;
                electron->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 13)
            {
                muonCount += weight;
                muon->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 22)
            {
                photonCount += weight;
                photon->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (std::abs(nu.m_selTrackTruePdg) == 211)
            {
                pionCount += weight;
                pion->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == 2212)
            {
                protonCount += weight;
                proton->Fill(nu.m_selTrackPandizzleScore, weight);
            }
            else if (nu.m_selTrackTruePdg == -9999)
            {
                noTrackCount += weight;
            }
            else
            {
                otherCount += weight;
                other->Fill(nu.m_selTrackPandizzleScore, weight);
            }
        }
    }

    // Integrated flavour metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "Out of FV anumu/numu: " << numuFVCount * 100/selectedBackground << "%" << std::endl;
    std::cout << "nue/anue: " << nueCount * 100/selectedBackground << "%" << std::endl;
    std::cout << "nutau/anutau: " << nutauCount * 100/selectedBackground << "%" << std::endl;
    std::cout << "NC: " << ncCount * 100/selectedBackground << "%" << std::endl;

    // Integrated pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron: " << electronCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "muon: " << muonCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "photon: " << photonCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "pion: " << pionCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "proton: " << protonCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "no track: " << noTrackCount * 100.0 / selectedBackground << "%" << std::endl;
    std::cout << "other: " << otherCount * 100.0 / selectedBackground << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("flavour decomposition", "flavour decomposition");
    THStack * flavourStack = new THStack("flavourStack", "Selected Background Decomposition;True Neutrino Energy [GeV]; nEvents");

    numuFVSelected->SetFillColor(kGray);
    nueSelected->SetFillColor(kBlue);
    nutauSelected->SetFillColor(kGreen);
    ncSelected->SetFillColor(kRed);

    numuFVSelected->SetFillStyle(3144);
    nueSelected->SetFillStyle(3144);
    nutauSelected->SetFillStyle(3144);
    ncSelected->SetFillStyle(3144);

    flavourStack->Add(nutauSelected);
    flavourStack->Add(ncSelected);
    flavourStack->Add(nueSelected);
    flavourStack->Add(numuFVSelected);
    flavourStack->Draw("hist");

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(nueSelected, "CC #nu_{e} + #bar{#nu_{e}}", "f");
    legend->AddEntry(numuFVSelected, "CC #nu_{#mu} + CC #bar{#nu_{#mu}} out of FV", "f");
    legend->AddEntry(nutauSelected, "CC #nu_{#tau} + #bar{#nu_{#tau}}", "f");
    legend->AddEntry(ncSelected, "NC", "f");
    legend->Draw("same");

    TCanvas * c2 = new TCanvas("pdg decomposition", "pdg decomposition");
    THStack * pdgStack = new THStack("pdgStack", "Selected Background Decomposition;Pandizzle Score;nEvents");

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
/*
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
        // Look at numu background
        if (IsNueCCSignal(nu))
            continue;

        if ((std::abs(nu.m_nuPdg) != 14) || nu.m_isNC)
            continue;

        // Apply Selection
        if ((!PERFORM_OLD_NUE_SELECTION && PassNueSelection(nu)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu)))
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
*/
//------------------------------------------------------------------------------------------------------------------------------------------

void RejectedSignalPlots(const NeutrinoEventVector &nuVector)
{
    // See the distribution?
    int nEnergyBins(40);
    double minEnergy(0.0), maxEnergy(10.0);
    TH1D * nueRejected = new TH1D("nueRejected", "nueRejected", nEnergyBins, minEnergy, maxEnergy); // energy distribution of the rejected signal

    // Rejected track PDG
    int nPandizzleBins(50);
    double minPandizzle(-1.0), maxPandizzle(1.0);

    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0), noShowerCount(0.0), noTrackCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * muon = new TH1D("muon", "muon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * photon = new TH1D("photon", "photon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * pion = new TH1D("pion", "pion", nPandizzleBins, minPandizzle, maxPandizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * other = new TH1D("other", "other", nPandizzleBins, minPandizzle, maxPandizzle);

    double selectedSignal(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Move on if not selection signal
        if (!IsNumuCCSignal(nu, IS_NEUTRINO))
            continue;

        // Move on if selected in nue selection
        if ((!PERFORM_OLD_NUE_SELECTION && PassNueSelection(nu, IS_NEUTRINO)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)))
            continue;

        // Look at signal events that fail selection
        if (PassNumuSelection(nu, IS_NEUTRINO))
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        selectedSignal += weight;
        
        /*    
        // what is the pdg breakdown of the selected tracks in the rejected signal?
        if (std::abs(nu.m_selTrackTruePdg) == 11)
        {
            electronCount += weight;
            electron->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (std::abs(nu.m_selTrackTruePdg) == 13)
        {
            muonCount += weight;
            muon->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selTrackTruePdg == 22)
        {
            photonCount += weight;
            photon->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (std::abs(nu.m_selTrackTruePdg) == 211)
        {
            pionCount += weight;
            pion->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selTrackTruePdg == 2212)
        {
            protonCount += weight;
            proton->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selTrackTruePdg == -9999)
        {
            noTrackCount += weight;
        }
        else
        {
            otherCount += weight;
            other->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        */

        // what is the pdge breakdown of the selected shower of the rejected signal?
        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            electronCount += weight;
            electron->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 13)
        {
            muonCount += weight;
            muon->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == 22)
        {
            photonCount += weight;
            photon->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (std::abs(nu.m_selShowerTruePdg) == 211)
        {
            pionCount += weight;
            pion->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == 2212)
        {
            protonCount += weight;
            proton->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selShowerTruePdg == -9999)
        {
            noShowerCount += weight;
        }
        else
        {
            otherCount += weight;
            other->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        
    }

    // Integrated pdg metrics
    std::cout << "////////////////////////////////////////////" << std::endl;
    std::cout << "electron: " << electronCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "muon: " << muonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "photon: " << photonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "pion: " << pionCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "proton: " << protonCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "no track (may not be relevant): " << noTrackCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "no shower (may not be relevant): " << noShowerCount * 100.0 / selectedSignal << "%" << std::endl;
    std::cout << "other: " << otherCount * 100.0 / selectedSignal << "%" << std::endl;

    // Draw those histograms
    TCanvas * c1 = new TCanvas("pdg decomposition", "pdg decomposition");
    THStack * pdgStack = new THStack("pdgStack", "Rejected Signal Decomposition;Pandizzle Score;nEvents");

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

void RejectedSignalNumuLengthPlots(const NeutrinoEventVector &nuVector)
{
    // See the distribution?
    int nLengthBins(40);
    double minLength(0.0), maxLength(2000.0);
    TH1D * numuRejected = new TH1D("numuRejected", "numuRejected", nLengthBins, minLength, maxLength);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Move on if not signal
        if (!IsNumuCCSignal(nu, IS_NEUTRINO))
            continue;

        // Move on if selected in nue selection
        if ((!PERFORM_OLD_NUE_SELECTION && PassNueSelection(nu, IS_NEUTRINO)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)))
            continue;

        // Look at signal events that fail selection
        if (PassNumuSelection(nu, IS_NEUTRINO))
            continue;

        // Move on if selected track is not a muon
        if (nu.m_selTrackTruePdg != 13)
            continue;

        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));

        double length(maxLength);

        if (IsTrackTrueEndInFiducialVolume(nu))
        {
            double x = (nu.m_selTrackTrueEndX - nu.m_selTrackTrueStartX);
            double y = (nu.m_selTrackTrueEndY - nu.m_selTrackTrueStartY);
            double z = (nu.m_selTrackTrueEndZ - nu.m_selTrackTrueStartZ);

            length = std::sqrt((x * x) + (y * y) + (z * z));
        }

        numuRejected->Fill(length, weight);
    }

    // Draw those histograms
    TCanvas * c1 = new TCanvas("rejected length", "rejected length");
    numuRejected->SetTitle(";Length [cm];nEvents");
    numuRejected->Draw("hist");
}

//------------------------------------------------------------------------------------------------------------------------------------------

// Sanity check plots
void SelectedSignalPlots(const NeutrinoEventVector &nuVector)
{
    // See the distribution?
    int nEnergyBins(40);
    double minEnergy(0.0), maxEnergy(10.0);
    TH1D * nueSelected = new TH1D("nueSelected", "nueSelected", nEnergyBins, minEnergy, maxEnergy); // energy distribution of the selected signal

    // Selected track PDG
    int nPandizzleBins(50);
    double minPandizzle(-1.0), maxPandizzle(1.0);

    double electronCount(0.0), muonCount(0.0), photonCount(0.0), pionCount(0.0), protonCount(0.0), otherCount(0.0);
    TH1D * electron = new TH1D("electron", "electron", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * muon = new TH1D("muon", "muon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * photon = new TH1D("photon", "photon", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * pion = new TH1D("pion", "pion", nPandizzleBins, minPandizzle, maxPandizzle); //charged pions (neutral pions are invisible anyway..)
    TH1D * proton = new TH1D("proton", "proton", nPandizzleBins, minPandizzle, maxPandizzle);
    TH1D * other = new TH1D("other", "other", nPandizzleBins, minPandizzle, maxPandizzle);

    double selectedSignal(0);

    for (const NeutrinoEvent &nu : nuVector)
    {
        // Move on if not signal
        if (!IsNumuCCSignal(nu, IS_NEUTRINO))
            continue;

        // Move on if selected in nue selection
        if ((!PERFORM_OLD_NUE_SELECTION && PassNueSelection(nu, IS_NEUTRINO)) || (PERFORM_OLD_NUE_SELECTION && PassOldNueSelection(nu, IS_NEUTRINO)))
            continue;

        
        // Look at signal events that are selected
        //if (!PassNumuSelection(nu, IS_NEUTRINO))
        //continue;
        
        const double weight(nu.m_projectedPOTWeight * (nu.m_isNC ? 1.0 : GetOscWeight(nu)));
        selectedSignal += weight;

        if (std::abs(nu.m_selTrackTruePdg) == 11)
        {
            electronCount += weight;
            electron->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (std::abs(nu.m_selTrackTruePdg) == 13)
        {
            muonCount += weight;
            muon->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selTrackTruePdg == 22)
        {
            photonCount += weight;
            photon->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (std::abs(nu.m_selTrackTruePdg) == 211)
        {
            pionCount += weight;
            pion->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else if (nu.m_selTrackTruePdg == 2212)
        {
            protonCount += weight;
            proton->Fill(nu.m_selTrackPandizzleScore, weight);
        }
        else
        {
            otherCount += weight;
            other->Fill(nu.m_selTrackPandizzleScore, weight);
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
    THStack * pdgStack = new THStack("pdgStack", "Selected Signal Decomposition;Pandizzle Score;nEvents");

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
