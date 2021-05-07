#include "neutrinoEvent.h"

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

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassOldNueSelection(const NeutrinoEvent &nu)
{
    if (nu.m_recoNuVtxNChildren == 0)
        return false;

    if (nu.m_selTrackPandizzleScore > NUE_PANDIZZLE_CUT_OLD)
        return false;

    if (nu.m_selShowerElectronScore < NUE_ELECTRON_SCORE_CUT)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;

    const double showerDisplacementX(nu.m_selShowerRecoStartX - nu.m_nuRecoVertexX);
    const double showerDisplacementY(nu.m_selShowerRecoStartY - nu.m_nuRecoVertexY);
    const double showerDisplacementZ(nu.m_selShowerRecoStartZ - nu.m_nuRecoVertexZ);
    const double showerDisplacementSquared((showerDisplacementX * showerDisplacementX) + (showerDisplacementY * showerDisplacementY) + (showerDisplacementZ * showerDisplacementZ));

    if (showerDisplacementSquared > (NUE_SHOWER_SEP_CUT * NUE_SHOWER_SEP_CUT))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNueSelection(const NeutrinoEvent &nu)
{
    if (nu.m_recoNuVtxNChildren == 0)
        return false;

    if (nu.m_selShowerPandrizzleScore < NUE_PANDRIZZLE_CUT)
        return false;

    if (nu.m_selTrackPandizzleScore > NUE_PANDIZZLE_CUT)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNumuSelection(const NeutrinoEvent &nu)
{
    if (nu.m_recoNuVtxNChildren == 0)
        return false;
    
    if (nu.m_selTrackPandizzleScore < NUMU_PANDIZZLE_CUT)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNueSelected(const NeutrinoEvent &nu)
{
    return PassOldNueSelection(nu);
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNumuSelected(const NeutrinoEvent &nu)
{
    if (IsNueSelected(nu))
        return false;

    if (PassNumuSelection(nu))
        return true;

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsRecoInFiducialVolume(const NeutrinoEvent &nu)
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


