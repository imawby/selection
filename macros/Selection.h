#ifndef SELECTION_H
#define SELECTION_H 1

double NUE_PANDIZZLE_CUT_OLD = 0.36;
double NUE_ELECTRON_SCORE_CUT = -0.02;
double NUE_SHOWER_SEP_CUT = 4.8;

//////////////////////////////////////////////////////////////////////
// FHC
//////////////////////////////////////////////////////////////////////

//neutrino (not anti) - using highest pandizzle score to select numu
// efficiency x purity
//double NUE_PANDRIZZLE_CUT = 0.50;
//double NUE_PANDIZZLE_CUT = 0.76;
//double NUMU_PANDIZZLE_CUT = 0.34;

// cheat available track & shower - tune with sensitivity
//double NUE_PANDRIZZLE_CUT = 0.35;
//double NUE_PANDIZZLE_CUT = 0.658;
//double NUMU_PANDIZZLE_CUT = 0.40;

// cheat primary track & shower - tune with sensitivity
//double NUE_PANDRIZZLE_CUT = 0.352;
//double NUE_PANDIZZLE_CUT = 0.616;
//double NUMU_PANDIZZLE_CUT = 0.34;

// cheat pfo track & shower - tune with sensitivity
//double NUE_PANDRIZZLE_CUT = 0.22;
//double NUE_PANDIZZLE_CUT = 0.79;
//double NUMU_PANDIZZLE_CUT = 0.4;

// cheat gamma removal - tune with sensitivity
//double NUE_PANDRIZZLE_CUT = -0.028;
//double NUE_PANDIZZLE_CUT = 0.83;
//double NUMU_PANDIZZLE_CUT = 0.34;

// cheat gamma removal + available - tune with sensitivity
//double NUE_PANDRIZZLE_CUT = -0.004;
//double NUE_PANDIZZLE_CUT = 0.82;
//double NUMU_PANDIZZLE_CUT = 0.4;

// cheat CC lepton reco
//double NUE_PANDRIZZLE_CUT = 0.73;
//double NUE_PANDIZZLE_CUT = 0.89;
//double NUMU_PANDIZZLE_CUT = 0.46;

// cheat full pandora reco (20 hit cuts)
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.91;
//double NUE_PANDIZZLE_CUT = 0.96;
//double NUMU_PANDIZZLE_CUT = 0.44;

// cheat visible vertex full pandora reco (20 hit cuts)
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.762;
//double NUE_PANDIZZLE_CUT = 0.69;
//double NUMU_PANDIZZLE_CUT = 0.3;

// cheat visible vertex electron gamma plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.76;
//double NUE_PANDIZZLE_CUT = 0.9;
//double NUMU_PANDIZZLE_CUT = 0.36;

// cheat visible vertex electron gamma reco  nu vertex
//double NUE_PANDRIZZLE_CUT = 0.704;
//double NUE_PANDIZZLE_CUT = 0.688;
//double NUMU_PANDIZZLE_CUT = 0.34;

// cheat visible vertex complete electron gamma plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.892;
//double NUE_PANDIZZLE_CUT = 0.888;
//double NUMU_PANDIZZLE_CUT = 0.3;

// cheat visible vertex complete electron gamma reco nu vertex
//double NUE_PANDRIZZLE_CUT = 0.792;
//double NUE_PANDIZZLE_CUT = 0.888;
//double NUMU_PANDIZZLE_CUT = 0.30;

// cheat visible vertex complete electron gamma plu nu vertex - no track/shower ID
//double NUE_PANDRIZZLE_CUT = 0.875;
//double NUE_PANDIZZLE_CUT = 0.83;
//double NUMU_PANDIZZLE_CUT = 0.38;

// cheat SHIFTED vertex complete electron gamma plu nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.91;
//double NUE_PANDIZZLE_CUT = 0.9;
//double NUMU_PANDIZZLE_CUT = 0.4;

// cheat CC electron refinement
//double NUE_PANDRIZZLE_CUT = 0.77;
//double NUE_PANDIZZLE_CUT = 0.692;
//double NUMU_PANDIZZLE_CUT = 0.40;

// cheat CC electron refinement plus vertex - visible
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.58;
//double NUE_PANDIZZLE_CUT = 0.68;
//double NUMU_PANDIZZLE_CUT = 0.40;

// cheat CC electron refinement - REDO
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.744;
//double NUE_PANDIZZLE_CUT = 0.676;
//double NUMU_PANDIZZLE_CUT = 0.34;

// cheat pandora gamma vertex refinement plus nu vertex - visible
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.70;
//double NUE_PANDIZZLE_CUT = 0.82;
//double NUMU_PANDIZZLE_CUT = 0.36;

// cheat CC electron plus gamma refinement - REDO
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.812;
//double NUE_PANDIZZLE_CUT = 0.808;
//double NUMU_PANDIZZLE_CUT = 0.38;

// cheat CC electron plus gamma refinement - visible
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.70;
//double NUE_PANDIZZLE_CUT = 0.82;
//double NUMU_PANDIZZLE_CUT = 0.28;

// cheat track shower ID
//double NUE_PANDRIZZLE_CUT = 0.476;
//double NUE_PANDIZZLE_CUT = 0.684;
//double NUMU_PANDIZZLE_CUT = 0.38;

// cheat electron plus characteristion
//double NUE_PANDRIZZLE_CUT = 0.816;
//double NUE_PANDIZZLE_CUT = 0.824;
//double NUMU_PANDIZZLE_CUT = 0.4;

// 7 sigma coverage - subset train (CURRENT STANDARD)
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.37;
//double NUE_PANDIZZLE_CUT = 0.72;
//double NUMU_PANDIZZLE_CUT = 0.34;

// pre selection cut (50% purity)
//double NUE_PANDRIZZLE_CUT = 0.026;
//double NUE_PANDIZZLE_CUT = 0.572;
//double NUMU_PANDIZZLE_CUT = 0.34;

// cheat shower characterisation
//double NUE_PANDRIZZLE_CUT = 0.82;
//double NUE_PANDIZZLE_CUT = 0.81;
//double NUMU_PANDIZZLE_CUT = 0.32;

// cheat muon reco
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.476;
//double NUE_PANDIZZLE_CUT = 0.656;
//double NUMU_PANDIZZLE_CUT = 0.48;

// cheat electron reco (cheat primary electrons in CC nue and CC numu)
//double NUE_PANDRIZZLE_CUT = 0.79;
//double NUE_PANDIZZLE_CUT = 0.676;
//double NUMU_PANDIZZLE_CUT = 0.34;

// cheat electron reco (only cheat primary electrons in CC nue)
//double NUE_PANDRIZZLE_CUT = 0.844;
//double NUE_PANDIZZLE_CUT = 0.832;
//double NUMU_PANDIZZLE_CUT = 0.36;

// cheat electron gamma
//double NUE_PANDRIZZLE_CUT = 0.712;
//double NUE_PANDIZZLE_CUT = 0.784;
//double NUMU_PANDIZZLE_CUT = 0.38;

// cheat electron gamma plus characterisation
//double NUE_PANDRIZZLE_CUT = 0.816;
//double NUE_PANDIZZLE_CUT = 0.824;
//double NUMU_PANDIZZLE_CUT = 0.42;

// cheat electron gamma plus vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.85;
//double NUE_PANDIZZLE_CUT = 0.9;
//double NUMU_PANDIZZLE_CUT = 0.42;

// cheat vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.58;
//double NUE_PANDIZZLE_CUT = 0.78;
//double NUMU_PANDIZZLE_CUT = 0.38;

// hybrid gamma removal - matching
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.70;
//double NUE_PANDIZZLE_CUT = 0.68;
//double NUMU_PANDIZZLE_CUT = 0.48;

// hybrid electron extension plus nu vertex - matching
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.55;
//double NUE_PANDIZZLE_CUT = 0.68;
//double NUMU_PANDIZZLE_CUT = 0.30;

// hybrid electron extension shift displacement plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.76;
//double NUE_PANDIZZLE_CUT = 0.92;
//double NUMU_PANDIZZLE_CUT = 0.30;

// hybrid electron extension shift dedx plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.85;
//double NUE_PANDIZZLE_CUT = 0.94;
//double NUMU_PANDIZZLE_CUT = 0.30;

// hybrid electron extension shift displacement shift dedx plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.85;
//double NUE_PANDIZZLE_CUT = 0.96;
//double NUMU_PANDIZZLE_CUT = 0.30;



// hybrid electron extension gamma refinement plus nu vertex - matching
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.64;
//double NUE_PANDIZZLE_CUT = 0.82;
//double NUMU_PANDIZZLE_CUT = 0.36;

// hybrid electron extension gamma refinement plus nu vertex
//double NUE_PANDRIZZLE_CUT = 0.72;
//double NUE_PANDIZZLE_CUT = 0.76;
//double NUMU_PANDIZZLE_CUT = 0.46;

// hybrid electron extension gamma refinement shift displacement plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.79;
//double NUE_PANDIZZLE_CUT = 0.94;
//double NUMU_PANDIZZLE_CUT = 0.36;

// hybrid electron extension gamma refinement shift displacement shift dedx plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.88;
//double NUE_PANDIZZLE_CUT = 0.94;
//double NUMU_PANDIZZLE_CUT = 0.36;

// hybrid electron extension gamma refinement shift dedx plus nu vertex
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.85;
//double NUE_PANDIZZLE_CUT = 0.94;
//double NUMU_PANDIZZLE_CUT = 0.34;

// real electron extension 100 + shift displacement + shift dedx
//double NUE_PANDRIZZLE_CUT = 0.43;
//double NUE_PANDIZZLE_CUT = 0.68;
//double NUMU_PANDIZZLE_CUT = 0.28;

// real electron extension 100 + BDT cut
//double NUE_PANDRIZZLE_CUT = 0.13;
//double NUE_PANDIZZLE_CUT = 0.78;
//double NUMU_PANDIZZLE_CUT = 0.3;

// real electron extension 100 + BDT variables
//double NUE_PANDRIZZLE_CUT = 0.55;
//double NUE_PANDIZZLE_CUT = 0.86;
//double NUMU_PANDIZZLE_CUT = 0.32;

// real electron extension 100 + BOTH BDTS (M02 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.35;
//double NUE_PANDRIZZLE_CUT = 0.90;
//double NUE_PANDIZZLE_CUT = 0.90;
//double NUMU_PANDIZZLE_CUT = 0.32;

// real electron extension 100 + BOTH BDTS (M01 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.30;
//double NUE_PANDRIZZLE_CUT = 0.90;
//double NUE_PANDIZZLE_CUT = 0.85;
//double NUMU_PANDIZZLE_CUT = 0.40;

// real electron extension 100 + BOTH BDTS (00 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.25;
//double NUE_PANDRIZZLE_CUT = 0.90;
//double NUE_PANDIZZLE_CUT = 0.85;
//double NUMU_PANDIZZLE_CUT = 0.36;

// real electron extension 100 + BOTH BDTS (01 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.50;
//double NUE_PANDRIZZLE_CUT = 0.95;
//double NUE_PANDIZZLE_CUT = 0.95;
//double NUMU_PANDIZZLE_CUT = 0.32;

// real electron extension 100 + BOTH BDTS (02 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.2;
//double NUE_PANDRIZZLE_CUT = 0.90;
//double NUE_PANDIZZLE_CUT = 0.75;
//double NUMU_PANDIZZLE_CUT = 0.36;

// real electron extension  (02)
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.58;
//double NUE_PANDIZZLE_CUT = 0.62;
//double NUMU_PANDIZZLE_CUT = 0.38;

// real electron extension 100 (02 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_PANDRIZZLE_CUT = 0.46;
//double NUE_PANDIZZLE_CUT = 0.74;
//double NUMU_PANDIZZLE_CUT = 0.36;

// real electron extension 25 + BOTH BDTS + MODULAR SHOWER VARS (02 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.4;
//double NUE_PANDRIZZLE_CUT = 0.925;
//double NUE_PANDIZZLE_CUT = 0.90;
//double NUMU_PANDIZZLE_CUT = 0.36;

// real electron extension standard vertexing 25 + BOTH BDTS + MODULAR SHOWER VARS (01 CUT)
double NUE_JAM_PANDRIZZLE_CUT = 0.025;
double NUE_PANDRIZZLE_CUT = 0.625;
double NUE_PANDIZZLE_CUT = 0.85;
double NUMU_PANDIZZLE_CUT = 0.34;

// real electron extension standard vertexing 25 + BOTH BDTS + MODULAR SHOWER VARS (02 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.25;
//double NUE_PANDRIZZLE_CUT = 0.775;
//double NUE_PANDIZZLE_CUT = 0.85;
//double NUMU_PANDIZZLE_CUT = 0.34;

// real electron extension standard vertexing 25 + BOTH BDTS + MODULAR SHOWER VARS (03 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.325;
//double NUE_PANDRIZZLE_CUT = 0.775;
//double NUE_PANDIZZLE_CUT = 0.90;
//double NUMU_PANDIZZLE_CUT = 0.34;

// real electron extension standard vertexing 25 + BOTH BDTS + MODULAR SHOWER VARS (04 CUT)
//double NUE_JAM_PANDRIZZLE_CUT = 0.10;
//double NUE_PANDRIZZLE_CUT = 0.625;
//double NUE_PANDIZZLE_CUT = 0.85;
//double NUMU_PANDIZZLE_CUT = 0.44;

// INACTIVE ALG, NEW SELECTION, OLD VERTEXING
//double NUE_JAM_PANDRIZZLE_CUT = 0.25;
//double NUE_PANDRIZZLE_CUT = 0.85;
//double NUE_PANDIZZLE_CUT = 0.9;
//double NUMU_PANDIZZLE_CUT = 0.34;

// INACTIVE ALG, NEW SELECTION, NEW VERTEXING
//double NUE_JAM_PANDRIZZLE_CUT = 0.4;
//double NUE_PANDRIZZLE_CUT = 0.85;
//double NUE_PANDIZZLE_CUT = 0.95;
//double NUMU_PANDIZZLE_CUT = 0.36;

//////////////////////////////////////////////////////////////////////
// RHC
//////////////////////////////////////////////////////////////////////

//antineutrino - using longest track to select numu (fool)
// efficiency x purity
//double ANUE_PANDRIZZLE_CUT = 0.60;
//double ANUE_PANDIZZLE_CUT = 0.44;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// cheat primary track & shower - tune with sensitivity
//double ANUE_PANDRIZZLE_CUT = 0.66;
//double ANUE_PANDIZZLE_CUT = 0.296;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// cheat available track & shower - tune with sensitivity
//double ANUE_PANDRIZZLE_CUT = 0.66;
//double ANUE_PANDIZZLE_CUT = 0.098;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// cheat gamma removal - tune with sensitivity
//double ANUE_PANDRIZZLE_CUT = 0.314;
//double ANUE_PANDIZZLE_CUT = 0.248;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// cheat gamma removal + available - tune with sensitivity
//double ANUE_PANDRIZZLE_CUT = 0.316;
//double ANUE_PANDIZZLE_CUT = 0.784;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// cheat CC lepton reco
//double ANUE_PANDRIZZLE_CUT = 0.756;
//double ANUE_PANDIZZLE_CUT = 0.176;
//double ANUMU_PANDIZZLE_CUT = 0.5;

// cheat full pandora reco (hit cuts)
//double ANUE_PANDRIZZLE_CUT = 0.812;
//double ANUE_PANDIZZLE_CUT = 0.308;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// cheat pfo track & shower - tune with sensitivity
//double ANUE_PANDRIZZLE_CUT = 0.656;
//double ANUE_PANDIZZLE_CUT = 0.736;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// 3 sigma coverage - old file
//double ANUE_PANDRIZZLE_CUT = 0.314;
//double ANUE_PANDIZZLE_CUT = 0.244;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// 3 sigma coverage - new file
//double ANUE_PANDRIZZLE_CUT = 0.256;
//double ANUE_PANDIZZLE_CUT = 0.398;
//double ANUMU_PANDIZZLE_CUT = 0.44;

// 3 sigma coverage - subset train (CCURENT STANDARD)
double ANUE_PANDRIZZLE_CUT = 0.664;
double ANUE_PANDIZZLE_CUT = 0.472;
double ANUMU_PANDIZZLE_CUT = 0.42;

// cheat shower characterisation
//double ANUE_PANDRIZZLE_CUT = 0.891;
//double ANUE_PANDIZZLE_CUT = 0.65;
//double ANUMU_PANDIZZLE_CUT = 0.38;

// cheat muon reco
//double ANUE_PANDRIZZLE_CUT = 0.72;
//double ANUE_PANDIZZLE_CUT = 0.44;
//double ANUMU_PANDIZZLE_CUT = 0.54;

// cheat electron reco
//double ANUE_PANDRIZZLE_CUT = 0.76;
//double ANUE_PANDIZZLE_CUT = 0.24;
//double ANUMU_PANDIZZLE_CUT = 0.42;

//////////////////////////////////////////////////////////////////////
// CVN CUTS
//////////////////////////////////////////////////////////////////////
double CVN_NUE_CUT = 0.85;
double CVN_NUMU_CUT = 0.50;

//////////////////////////////////////////////////////////////////////
// PLACEHOLDER
//////////////////////////////////////////////////////////////////////
//double NUE_JAM_PANDRIZZLE_CUT = 1.0;
//double NUE_JAM_PANDIZZLE_CUT = 1.0;
double ANUE_JAM_PANDRIZZLE_CUT = 1.0;
double ANUE_JAM_PANDIZZLE_CUT = 1.0;

bool PassOldNueSelection(const NeutrinoEvent &nu, const bool isNu);
bool PassNueJamSelection(const NeutrinoEvent &nu, const bool isNu);
bool PassNueSelection(const NeutrinoEvent &nu, const bool isNu);
bool PassNumuSelection(const NeutrinoEvent &nu, const bool isNu);
bool IsNueSelected(const NeutrinoEvent &nu, const bool isNu, const bool isJamSelection);
bool IsNumuSelected(const NeutrinoEvent &nu, const bool isNu, const bool isJamSelection);
bool IsRecoInFiducialVolume(const NeutrinoEvent &nu);

#endif
