#ifndef SELECTION_H
#define SELECTION_H 1

double NUE_PANDIZZLE_CUT_OLD = 0.36;
double NUE_ELECTRON_SCORE_CUT = -0.02;
double NUE_SHOWER_SEP_CUT = 4.8;

double NUE_PANDRIZZLE_CUT = 0.42;
double NUE_PANDIZZLE_CUT = 0.72;

double NUMU_PANDIZZLE_CUT = 0.05;


/*
double NUE_PANDIZZLE_CUT_OLD = 0.56;
double NUE_ELECTRON_SCORE_CUT = -0.04;
double NUE_SHOWER_SEP_CUT = 5.2;

double NUE_PANDRIZZLE_CUT = 0.42;
double NUE_PANDIZZLE_CUT = 0.72;

double NUMU_PANDIZZLE_CUT = 0.484;
*/
bool PassOldNueSelection(const NeutrinoEvent &nu);
bool PassNueSelection(const NeutrinoEvent &nu);
bool PassNumuSelection(const NeutrinoEvent &nu);
bool IsNueSelected(const NeutrinoEvent &nu);
bool IsNumuSelected(const NeutrinoEvent &nu);
bool IsRecoInFiducialVolume(const NeutrinoEvent &nu);

#endif
