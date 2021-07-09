#ifndef SIGNAL_H
#define SIGNAL_H 1

bool IsNueCCSignal(const NeutrinoEvent &nu, const bool isNu);
bool IsNueFlavourCCSignal(const NeutrinoEvent &nu);
bool IsNumuCCSignal(const NeutrinoEvent &nu, const bool isNu);
bool IsNumuFlavourCCSignal(const NeutrinoEvent &nu);
bool IsTrueInFiducialVolume(const NeutrinoEvent &nu);
bool IsTrackTrueStartInFiducialVolume(const NeutrinoEvent &nu);
bool IsTrackTrueEndInFiducialVolume(const NeutrinoEvent &nu);

#endif
