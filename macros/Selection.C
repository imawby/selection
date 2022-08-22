#include "neutrinoEvent.h"
#include "Selection.h"
#include "Signal.h"
//------------------------------------------------------------------------------------------------------------------------------------------

bool PassOldNueSelection(const NeutrinoEvent &nu, const bool isNu)
{
    if (!isNu)
    {
        std::cout << "this hasn't been set for anti neutrinos isobel!!" << std::endl;
        throw;
    }

    if (nu.m_recoNuVtxNChildren == 0)
        return false;

    if (nu.m_selTrackPandizzleScore > NUE_PANDIZZLE_CUT_OLD)
        return false;

    if (nu.m_selShowerElectronScore < NUE_ELECTRON_SCORE_CUT)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;

    const double showerDisplacementX(nu.m_selShowerStartX - nu.m_nuRecoVertexX);
    const double showerDisplacementY(nu.m_selShowerStartY - nu.m_nuRecoVertexY);
    const double showerDisplacementZ(nu.m_selShowerStartZ - nu.m_nuRecoVertexZ);
    const double showerDisplacementSquared((showerDisplacementX * showerDisplacementX) + (showerDisplacementY * showerDisplacementY) + (showerDisplacementZ * showerDisplacementZ));

    if (showerDisplacementSquared > (NUE_SHOWER_SEP_CUT * NUE_SHOWER_SEP_CUT))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNueJamSelection(const NeutrinoEvent &nu, const bool isNu)
{
    if (nu.m_recoNuVtxNChildren == 0)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;

    // does it pass the jam-enhanced pandrizzle cuts?    
    const double nueJamPandizzleCut(isNu ? NUE_PANDIZZLE_CUT : ANUE_PANDIZZLE_CUT);
    const double nueJamPandrizzleCut(isNu ? NUE_JAM_PANDRIZZLE_CUT : ANUE_JAM_PANDRIZZLE_CUT);

    if ((nu.m_selShowerJamPandrizzleScore > nueJamPandrizzleCut) && (nu.m_selTrackPandizzleScore < nueJamPandizzleCut))
        return true;

    // does it pass the pandrizzle cuts
    const double nuePandizzleCut(isNu ? NUE_PANDIZZLE_CUT : ANUE_PANDIZZLE_CUT);
    const double nuePandrizzleCut(isNu ? NUE_PANDRIZZLE_CUT : ANUE_PANDRIZZLE_CUT);

    if ((nu.m_selShowerPandrizzleScore > nuePandrizzleCut) && (nu.m_selTrackPandizzleScore < nuePandizzleCut))
        return true;

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNueSelection(const NeutrinoEvent &nu, const bool isNu)
{
    if (nu.m_recoNuVtxNChildren == 0)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;
    
    const double nuePandizzleCut(isNu ? NUE_PANDIZZLE_CUT : ANUE_PANDIZZLE_CUT);
    const double nuePandrizzleCut(isNu ? NUE_PANDRIZZLE_CUT : ANUE_PANDRIZZLE_CUT);

    if (nu.m_selTrackPandizzleScore > nuePandizzleCut)
        return false;

    if (nu.m_selShowerPandrizzleScore < nuePandrizzleCut)
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------
/*
bool PassNueSelection(const NeutrinoEvent &nu, double pandrizzleCut, double pandizzleCut)
{
    if (nu.m_recoNuVtxNChildren == 0)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;
    
    if (nu.m_selTrackPandizzleScore > pandizzleCut)
        return false;

    if (nu.m_selShowerPandrizzleScore < pandrizzleCut)
        return false;

    return true;
}
*/
//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNumuSelection(const NeutrinoEvent &nu, const bool isNu)
{
    const double numuPandizzleCut(isNu ? NUMU_PANDIZZLE_CUT : ANUMU_PANDIZZLE_CUT);

    if (nu.m_recoNuVtxNChildren == 0)
        return false;
    
    if (nu.m_selTrackPandizzleScore < numuPandizzleCut)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNueSelected(const NeutrinoEvent &nu, const bool isNu, const bool isJamSelection)
{
    if (isJamSelection)
        return PassNueJamSelection(nu, isNu);

    return PassNueSelection(nu, isNu);
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNumuSelected(const NeutrinoEvent &nu, const bool isNu, const bool isJamSelection)
{
    if (IsNueSelected(nu, isNu, isJamSelection))
        return false;

    return PassNumuSelection(nu, isNu);
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassCVNNueSelection(const NeutrinoEvent &nu)
{
    if (!IsRecoInFiducialVolume(nu))
        return false;

    if (nu.m_cvnResultNue < CVN_NUE_CUT)
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassCVNNumuSelection(const NeutrinoEvent &nu)
{
    if (!IsRecoInFiducialVolume(nu))
        return false;

    if (nu.m_cvnResultNumu < CVN_NUMU_CUT)
        return false;

    return true;
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


