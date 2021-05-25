#include "neutrinoEvent.h"
#include "Signal.h"

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNueCCSignal(const NeutrinoEvent &nu)
{
    if (std::fabs(nu.m_nuPdg) != 12)
        return false;

    if (nu.m_isNC)
        return false;

    // Interaction on liquid argon
    if (nu.m_target != 18)
        return false;

    if (!IsTrueInFiducialVolume(nu))
        return false;

    return true;
}


//------------------------------------------------------------------------------------------------------------------------------------------

bool IsNumuCCSignal(const NeutrinoEvent &nu)
{
    if (std::fabs(nu.m_nuPdg) != 14)
        return false;

    if (nu.m_isNC)
        return false;

    // Interaction on liquid argon
    if (nu.m_target != 18)
        return false;

    if (!IsTrueInFiducialVolume(nu))
        return false;

   return true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool IsTrueInFiducialVolume(const NeutrinoEvent &nu)
{
    const double minX(-360.0 + 50.0), maxX(360.0 - 50.0);
    const double minY(-600.0 + 50.0), maxY(600.0 - 50.0);
    const double minZ(50.0), maxZ(1394.0 - 150.0);

    if ((nu.m_nuTrueVertexX < minX) || (nu.m_nuTrueVertexX > maxX))
        return false;

    if ((nu.m_nuTrueVertexY < minY) || (nu.m_nuTrueVertexY > maxY))
        return false;

    if ((nu.m_nuTrueVertexZ < minZ) || (nu.m_nuTrueVertexZ > maxZ))
        return false;

    return true;
}
