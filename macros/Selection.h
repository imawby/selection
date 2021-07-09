#ifndef SELECTION_H
#define SELECTION_H 1

double NUE_PANDIZZLE_CUT_OLD = 0.36;
double NUE_ELECTRON_SCORE_CUT = -0.02;
double NUE_SHOWER_SEP_CUT = 4.8;

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
double NUE_PANDRIZZLE_CUT = -0.004;
double NUE_PANDIZZLE_CUT = 0.82;
double NUMU_PANDIZZLE_CUT = 0.4;

// 5sigma coverage
//double NUE_PANDRIZZLE_CUT = 0.34;
//double NUE_PANDIZZLE_CUT = 0.696;
//double NUMU_PANDIZZLE_CUT = 0.34;

// ANTI
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
double ANUE_PANDRIZZLE_CUT = 0.316;
double ANUE_PANDIZZLE_CUT = 0.784;
double ANUMU_PANDIZZLE_CUT = 0.46;

// cheat pfo track & shower - tune with sensitivity
//double ANUE_PANDRIZZLE_CUT = 0.656;
//double ANUE_PANDIZZLE_CUT = 0.736;
//double ANUMU_PANDIZZLE_CUT = 0.46;

// 2sigma coverage
//double ANUE_PANDRIZZLE_CUT = 0.66;
//double ANUE_PANDIZZLE_CUT = 0.736;
//double ANUMU_PANDIZZLE_CUT = 0.46;




// CVN CUTS
double CVN_NUE_CUT = 0.85;
double CVN_NUMU_CUT = 0.50;

/*
double NUE_PANDIZZLE_CUT_OLD = 0.56;
double NUE_ELECTRON_SCORE_CUT = -0.04;
double NUE_SHOWER_SEP_CUT = 5.2;

double NUE_PANDRIZZLE_CUT = 0.42;
double NUE_PANDIZZLE_CUT = 0.72;

double NUMU_PANDIZZLE_CUT = 0.484;
*/
bool PassOldNueSelection(const NeutrinoEvent &nu, const bool isNu);
bool PassNueSelection(const NeutrinoEvent &nu, const bool isNu);
bool PassNumuSelection(const NeutrinoEvent &nu, const bool isNu);
bool IsNueSelected(const NeutrinoEvent &nu, const bool isNu);
bool IsNumuSelected(const NeutrinoEvent &nu, const bool isNu);
bool IsRecoInFiducialVolume(const NeutrinoEvent &nu);

#endif
