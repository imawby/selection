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

    const double showerDisplacementX(nu.m_selShowerRecoStartX - nu.m_nuRecoVertexX);
    const double showerDisplacementY(nu.m_selShowerRecoStartY - nu.m_nuRecoVertexY);
    const double showerDisplacementZ(nu.m_selShowerRecoStartZ - nu.m_nuRecoVertexZ);
    const double showerDisplacementSquared((showerDisplacementX * showerDisplacementX) + (showerDisplacementY * showerDisplacementY) + (showerDisplacementZ * showerDisplacementZ));

    if (showerDisplacementSquared > (NUE_SHOWER_SEP_CUT * NUE_SHOWER_SEP_CUT))
        return false;

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PassNueSelection(const NeutrinoEvent &nu, const bool isNu)
{
    if (nu.m_recoNuVtxNChildren == 0)
        return false;

    if (!IsRecoInFiducialVolume(nu))
        return false;
    
    // CHEATING
    /*
    // Remove all anumu/numu background
    if ((std::abs(nu.m_nuPdg) == 14) && !nu.m_isNC)
        return false;
    
    
    // Remove all anutau/nutau background
    if ((std::abs(nu.m_nuPdg) == 16) && !nu.m_isNC)
        return false;
    
    // Remove all NC events
    if (nu.m_isNC)
        return false;
    

    // Prefect
    if (IsNueCCSignal(nu, isNu))
    {
        return true;
    }
    else
    {
        return false;
    }
*/

    // remove events that are actually photons.. 
    if(nu.m_selShowerTruePdg == 22)
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

bool IsNueSelected(const NeutrinoEvent &nu, const bool isNu)
{
    return PassNueSelection(nu, isNu);
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNumuSelected(const NeutrinoEvent &nu, const bool isNu)
{
    if (IsNueSelected(nu, isNu))
        return false;

    if (PassNumuSelection(nu, isNu))
        return true;

    return false;
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


