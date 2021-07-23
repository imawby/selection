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

// cheat full pandora reco (hit cuts)
//double NUE_PANDRIZZLE_CUT = 0.754;
//double NUE_PANDIZZLE_CUT = 0.64;
//double NUMU_PANDIZZLE_CUT = 0.44;

// 7 sigma coverage - old file
//double NUE_PANDRIZZLE_CUT = -0.04;
//double NUE_PANDIZZLE_CUT = 0.84;
//double NUMU_PANDIZZLE_CUT = 0.34;

// 7 sigma coverage - new file
//double NUE_PANDRIZZLE_CUT = -0.064;
//double NUE_PANDIZZLE_CUT = 0.57;
//double NUMU_PANDIZZLE_CUT = 0.34;

// 7 sigma coverage - subset train (CURRENT STANDARD)
double NUE_PANDRIZZLE_CUT = 0.418;
double NUE_PANDIZZLE_CUT = 0.712;
double NUMU_PANDIZZLE_CUT = 0.34;

// cheat shower characterisation
//double NUE_PANDRIZZLE_CUT = 0.82;
//double NUE_PANDIZZLE_CUT = 0.81;
//double NUMU_PANDIZZLE_CUT = 0.32;

// cheat muon reco
//double NUE_PANDRIZZLE_CUT = 0.476;
//double NUE_PANDIZZLE_CUT = 0.656;
//double NUMU_PANDIZZLE_CUT = 0.48;

// cheat electron reco
//double NUE_PANDRIZZLE_CUT = 0.79;
//double NUE_PANDIZZLE_CUT = 0.676;
//double NUMU_PANDIZZLE_CUT = 0.34;

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


bool PassOldNueSelection(const NeutrinoEvent &nu, const bool isNu);
bool PassNueSelection(const NeutrinoEvent &nu, const bool isNu);
bool PassNumuSelection(const NeutrinoEvent &nu, const bool isNu);
bool IsNueSelected(const NeutrinoEvent &nu, const bool isNu);
bool IsNumuSelected(const NeutrinoEvent &nu, const bool isNu);
bool IsRecoInFiducialVolume(const NeutrinoEvent &nu);

#endif
