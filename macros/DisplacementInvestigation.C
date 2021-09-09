#include "NeutrinoEvent.C"
#include "Selection.C"

void DisplacementInvestigation(const std::string &inputFileName);
void DrawHistograms(NeutrinoEventVector &nuEventVector_full, ElectronValidation &electronValidation);

void DisplacementInvestigation(const std::string &inputFileName_full)
{
    ElectronValidation electronValidation;
    NeutrinoEventVector nuEventVector_full;
    ReadFile(inputFileName_full, nuEventVector_full, electronValidation);

    DrawHistograms(nuEventVector_full, electronValidation);    
}

//------------------------------------------------------------------------------------------------------------------------------------------

void DrawHistograms(NeutrinoEventVector &nuEventVector_full, ElectronValidation &electronValidation)
{
    const int pandrizzleBins = 25, displacementBins = 17;
    double pandrizzleMin = -1.0, displacementMin = -100.0;
    double pandrizzleMax = 1.0, displacementMax = 100.0;
    double displacementBinLimits[displacementBins] = {-100.0,-20.0,-10.0,-5.0,-4.0,-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0,4.0,5.0,10.0,20.0,100.0};

    TH1D * displacementSel = new TH1D("displacementSel", "displacementSel", displacementBins, displacementBinLimits);
    TH2D * displacementPandrizzleSel = new TH2D("displacementPandrizzleSel", "displacementPandrizzleSel", pandrizzleBins, pandrizzleMin, pandrizzleMax, 
        displacementBins, displacementBinLimits);
    TH1D * countPandrizzleSel = new TH1D("countPandrizzleSel", "countPandrizzleSel", displacementBins, displacementBinLimits);
    TH1D * avPandrizzleSel = new TH1D("avPandrizzleSel", "avPandrizzleSel", displacementBins, displacementBinLimits);

    double evalRatioMin = -1.0, evalRatioMax = 1.0;
    TH1D * evalRatioSel_Downstream =  new TH1D("evalRatioSel_Downstream", "evalRatioSel_Downstream", 100, evalRatioMin, evalRatioMax);
    TH1D * evalRatioSel_Upstream =  new TH1D("evalRatioSel_Upstream", "evalRatioSel_Upstream", 100, evalRatioMin, evalRatioMax);

    double concentrationMin = 0.0, concentrationMax = 50.0;
    TH1D * concentrationSel_Downstream = new TH1D("concentrationSel_Downstream", "concentrationSel_Downstream", 100, concentrationMin, concentrationMax);
    TH1D * concentrationSel_Upstream = new TH1D("concentrationSel_Upstream", "concentrationSel_Upstream", 100, concentrationMin, concentrationMax);

    double coreHaloRatioMin = 0.0, coreHaloRatioMax = 100.0;
    TH1D * coreHaloRatioSel_Downstream = new TH1D("coreHaloRatioSel_Downstream", "coreHaloRatioSel_Downstream", 100, coreHaloRatioMin, coreHaloRatioMax);
    TH1D * coreHaloRatioSel_Upstream = new TH1D("coreHaloRatioSel_Upstream", "coreHaloRatioSel_Upstream", 100, coreHaloRatioMin, coreHaloRatioMax);

    double conicalnessMin = 0.0, conicalnessMax = 100.0;
    TH1D * conicalnessSel_Downstream = new TH1D("conicalnessSel_Downstream", "conicalnessSel_Downstream", 100, conicalnessMin, conicalnessMax);
    TH1D * conicalnessSel_Upstream = new TH1D("conicalnessSel_Upstream", "conicalnessSel_Upstream", 100, conicalnessMin, conicalnessMax);

    double deDxMin = 0.0, deDxMax = 20.0;
    TH1D * deDxSel_Downstream = new TH1D("deDxSel_Downstream", "deDxSel_Downstream", 100, deDxMin, deDxMax);
    TH1D * deDxSel_Upstream = new TH1D("deDxSel_Upstream", "deDxSel_Upstream", 100, deDxMin, deDxMax);

    //double displacementMin = 0.0, displacementMax = 100.0;

    double dcaMin = 0.0, dcaMax = 50.0;
    TH1D * dcaSel_Downstream = new TH1D("dcaSel_Downstream", "dcaSel_Downstream", 100, dcaMin, dcaMax);
    TH1D * dcaSel_Upstream = new TH1D("dcaSel_Upstream", "dcaSel_Upstream", 100, dcaMin, dcaMax);

    double widenessMin = 0.0, widenessMax = 0.1;
    TH1D * widenessSel_Downstream  = new TH1D("widenessSel_Downstream", "widenessSel_Downstream", 100, widenessMin, widenessMax);
    TH1D * widenessSel_Upstream  = new TH1D("widenessSel_Upstream", "widenessSel_Upstream", 100, widenessMin, widenessMax);

    double energyDensityMin = 0.0, energyDensityMax = 5.0;
    TH1D * energyDensitySel_Downstream = new TH1D("energyDensitySel_Downstream", "energyDensitySel_Downstream", 100, energyDensityMin, energyDensityMax);
    TH1D * energyDensitySel_Upstream = new TH1D("energyDensitySel_Upstream", "energyDensitySel_Upstream", 100, energyDensityMin, energyDensityMax);

    for (const NeutrinoEvent &nu : nuEventVector_full)
    {
        if (!IsRecoInFiducialVolume(nu))
            continue;

        // Fill selected shower histograms
        if (std::abs(nu.m_selShowerTruePdg) == 11)
        {
            double displacementX = nu.m_selShowerStartX - nu.m_nuRecoVertexX;
            double displacementY = nu.m_selShowerStartY - nu.m_nuRecoVertexY;
            double displacementZ = nu.m_selShowerStartZ - nu.m_nuRecoVertexZ;
            //double displacement = std::sqrt((displacementX * displacementX) + (displacementY * displacementY) + (displacementZ * displacementZ));
            double displacement = nu.m_selShowerDisplacement;

            double dot = (nu.m_selShowerDirectionX * displacementX) + (nu.m_selShowerDirectionY * displacementY) + (nu.m_selShowerDirectionZ * displacementZ);
            double showerDirectionMag = std::sqrt((nu.m_selShowerDirectionX * nu.m_selShowerDirectionX) + (nu.m_selShowerDirectionY * nu.m_selShowerDirectionY) + 
                (nu.m_selShowerDirectionZ * nu.m_selShowerDirectionZ));

            if (std::fabs(showerDirectionMag) < std::numeric_limits<float>::epsilon())
                std::cout << "displacement: (" << displacementX << ", " << displacementY << ", " << displacementZ << ")" << std::endl;

            double cosOpeningAngle = dot / (showerDirectionMag * displacement);
            bool nuVertexDownstream(false);

            if (cosOpeningAngle < 0.f)
                nuVertexDownstream = true;

            if (nuVertexDownstream)
                displacement = displacement * (-1.f);


            displacementSel->Fill(displacement, 1.0);
            displacementPandrizzleSel->Fill(nu.m_selShowerPandrizzleScore, displacement, 1.0);
            countPandrizzleSel->Fill(displacement, 1.0);
            avPandrizzleSel->Fill(displacement, nu.m_selShowerPandrizzleScore);

            if (nuVertexDownstream)
            {
                evalRatioSel_Downstream->Fill(nu.m_selShowerEvalRatio);
                concentrationSel_Downstream->Fill(nu.m_selShowerConcentration);
                coreHaloRatioSel_Downstream->Fill(nu.m_selShowerCoreHaloRatio);
                conicalnessSel_Downstream->Fill(nu.m_selShowerConicalness);
                deDxSel_Downstream->Fill(nu.m_selShowerdEdxBestPlane);
                dcaSel_Downstream->Fill(nu.m_selShowerDCA);
                widenessSel_Downstream->Fill(nu.m_selShowerWideness);
                energyDensitySel_Downstream->Fill(nu.m_selShowerEnergyDensity);
            }
            else
            {
                evalRatioSel_Upstream->Fill(nu.m_selShowerEvalRatio);
                concentrationSel_Upstream->Fill(nu.m_selShowerConcentration);
                coreHaloRatioSel_Upstream->Fill(nu.m_selShowerCoreHaloRatio);
                conicalnessSel_Upstream->Fill(nu.m_selShowerConicalness);
                deDxSel_Upstream->Fill(nu.m_selShowerdEdxBestPlane);
                dcaSel_Upstream->Fill(nu.m_selShowerDCA);
                widenessSel_Upstream->Fill(nu.m_selShowerWideness);
                energyDensitySel_Upstream->Fill(nu.m_selShowerEnergyDensity);
            }

        }
    }

                evalRatioSel_Downstream->Scale(1.f/evalRatioSel_Downstream->Integral());
                concentrationSel_Downstream->Scale(1.f/concentrationSel_Downstream->Integral());
                coreHaloRatioSel_Downstream->Scale(1.f/coreHaloRatioSel_Downstream->Integral());
                conicalnessSel_Downstream->Scale(1.f/conicalnessSel_Downstream->Integral());
                deDxSel_Downstream->Scale(1.f/deDxSel_Downstream->Integral());
                dcaSel_Downstream->Scale(1.f/dcaSel_Downstream->Integral());
                widenessSel_Downstream->Scale(1.f/widenessSel_Downstream->Integral());
                energyDensitySel_Downstream->Scale(1.f/energyDensitySel_Downstream->Integral());

                evalRatioSel_Upstream->Scale(1.f/evalRatioSel_Upstream->Integral());
                concentrationSel_Upstream->Scale(1.f/concentrationSel_Upstream->Integral());
                coreHaloRatioSel_Upstream->Scale(1.f/coreHaloRatioSel_Upstream->Integral());
                conicalnessSel_Upstream->Scale(1.f/conicalnessSel_Upstream->Integral());
                deDxSel_Upstream->Scale(1.f/deDxSel_Upstream->Integral());
                dcaSel_Upstream->Scale(1.f/dcaSel_Upstream->Integral());
                widenessSel_Upstream->Scale(1.f/widenessSel_Upstream->Integral());
                energyDensitySel_Upstream->Scale(1.f/energyDensitySel_Upstream->Integral());


    // Obtain average pandrizzle scores
    avPandrizzleSel->Divide(countPandrizzleSel);

    gStyle->SetOptStat(0);

    TCanvas * cAvPandrizzleSel = new TCanvas("AvPandrizzleSel", "AvPandrizzleSel");
    cAvPandrizzleSel->Divide(2,1);
    cAvPandrizzleSel->cd(1);
    avPandrizzleSel->SetTitle("av. Pandrizzle score - Selected True Electrons;Displacement;av. Pandrizzle Score");
    avPandrizzleSel->Draw("hist");
    cAvPandrizzleSel->cd(2);
    countPandrizzleSel->SetTitle("Displacement Distribution - Selected True Electrons;Displacement;nEvents");
    countPandrizzleSel->Draw("hist");

    TCanvas * cDisplacementSel = new TCanvas("DisplacementSel", "DisplacementSel");
    displacementSel->SetTitle("Displacement - Selected True Electrons;Displacement;nElectrons");
    displacementSel->Draw("hist");

    TCanvas * cDisplacementPandrizzleSel = new TCanvas("DisplacementPandrizzleSel", "DisplacementPandrizzleSel");
    displacementPandrizzleSel->SetTitle(";Pandrizzle Score;Displacement");
    displacementPandrizzleSel->Draw("colz");

    TCanvas * cVariables = new TCanvas("cVariables", "cVariables");
    cVariables->Divide(4,2);
    cVariables->cd(1);
    evalRatioSel_Upstream->SetTitle("EvalRatio;EvalRatio;nEntries");
    evalRatioSel_Downstream->SetLineColor(kRed);
    evalRatioSel_Upstream->SetLineColor(kBlue);
    evalRatioSel_Upstream->Draw("hist");
    evalRatioSel_Downstream->Draw("hist same");
    TLegend * lEvalRatio = new TLegend(0.1,0.7,0.48,0.9);
    lEvalRatio->AddEntry(evalRatioSel_Downstream, "Nu Downstream", "l");
    lEvalRatio->AddEntry(evalRatioSel_Upstream, "Nu Upstream", "l");
    lEvalRatio->Draw("same");

    cVariables->cd(2);
    concentrationSel_Downstream->SetTitle("concentration;concentration;nEntries");
    concentrationSel_Downstream->SetLineColor(kRed);
    concentrationSel_Upstream->SetLineColor(kBlue);
    concentrationSel_Downstream->Draw("hist");
    concentrationSel_Upstream->Draw("hist same");

    cVariables->cd(3);
    coreHaloRatioSel_Downstream->SetTitle("coreHaloRatio;coreHaloRatio;nEntries");
    coreHaloRatioSel_Downstream->SetLineColor(kRed);
    coreHaloRatioSel_Upstream->SetLineColor(kBlue);
    coreHaloRatioSel_Downstream->Draw("hist");
    coreHaloRatioSel_Upstream->Draw("hist same");

    cVariables->cd(4);
    conicalnessSel_Upstream->SetTitle("conicalness;conicalness;nEntries");
    conicalnessSel_Downstream->SetLineColor(kRed);
    conicalnessSel_Upstream->SetLineColor(kBlue);
    conicalnessSel_Upstream->Draw("hist");
    conicalnessSel_Downstream->Draw("hist same");

    cVariables->cd(5);
    deDxSel_Downstream->SetTitle("dEdxBestPlane;dEdxBestPlane;nEntries");
    deDxSel_Downstream->SetLineColor(kRed);
    deDxSel_Upstream->SetLineColor(kBlue);
    deDxSel_Downstream->Draw("hist");
    deDxSel_Upstream->Draw("hist same");

    cVariables->cd(6);
    dcaSel_Upstream->SetTitle("DCA;DCA;nEntries");
    dcaSel_Downstream->SetLineColor(kRed);
    dcaSel_Upstream->SetLineColor(kBlue);
    dcaSel_Upstream->Draw("hist");
    dcaSel_Downstream->Draw("hist same");

    cVariables->cd(7);
    widenessSel_Downstream->SetTitle("wideness;wideness;nEntries");
    widenessSel_Downstream->SetLineColor(kRed);
    widenessSel_Upstream->SetLineColor(kBlue);
    widenessSel_Downstream->Draw("hist");
    widenessSel_Upstream->Draw("hist same");

    cVariables->cd(8);
    energyDensitySel_Downstream->SetTitle("energyDensity;energyDensity;nEntries");
    energyDensitySel_Downstream->SetLineColor(kRed);
    energyDensitySel_Upstream->SetLineColor(kBlue);
    energyDensitySel_Downstream->Draw("hist");
    energyDensitySel_Upstream->Draw("hist same");

    /*
    pandrizzle_signal->Write("pandrizzle_signal");
    pandrizzle_background->Write("pandrizzle_background");
    pandizzle_signal->Write("pandizzle_signal");
    pandizzle_background->Write("pandizzle_background");

    file->Close();
    */
}

//------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------
/*
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
*/
