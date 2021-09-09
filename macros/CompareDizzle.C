void CompareDizzle(const std::string cheatFile)
{
    TFile * standard = new TFile("DizzleScores_standard.root", "READ");
    TFile * cheat = new TFile(cheatFile.c_str(), "READ");

    TH1D * standard_signal = (TH1D*)standard->Get("pandrizzle_signal");
    TH1D * cheat_signal = (TH1D*)cheat->Get("pandrizzle_signal");
    TH1D * standard_background = (TH1D*)standard->Get("pandrizzle_background");
    TH1D * cheat_background = (TH1D*)cheat->Get("pandrizzle_background");

    standard_signal->SetLineColor(kBlack);
    cheat_signal->SetLineColor(kRed);

    standard_background->SetLineColor(kBlack);
    cheat_background->SetLineColor(kRed);

    TCanvas * cSignal = new TCanvas("Signal Pandrizzle", "Signal Pandrizzle");
    cheat_signal->Draw("hist");
    standard_signal->Draw("hist same");

    auto lSignal = new TLegend(0.1,0.7,0.48,0.9);
    lSignal->AddEntry(standard_signal, "Standard", "l");
    lSignal->AddEntry(cheat_signal, "Cheat", "l");
    lSignal->Draw("same");

    TCanvas * cBackground = new TCanvas("Background Pandrizzle", "Background Pandrizzle");
    standard_background->Draw("hist");
    cheat_background->Draw("hist same");

    auto lBackground = new TLegend(0.1,0.7,0.48,0.9);
    lBackground->AddEntry(standard_background, "Standard", "l");
    lBackground->AddEntry(cheat_background, "Cheat", "l");
    lBackground->Draw("same");
}
