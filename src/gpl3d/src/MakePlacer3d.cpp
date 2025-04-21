
#include "gpl3d/MakePlacer3d.h"

#include "ord/OpenRoad.hh"
#include "gpl3d/Placer3d.h"
#include "utl/decode.h"

namespace gpl3d{
    // Tcl files encoded into strings.
    extern const char* gpl3d_tcl_inits[];
} // namespace gpl3d

extern "C" {
    extern int Gpl3d_Init(Tcl_Interp* interp);
}

namespace ord{

gpl3d::Placer3d* makePlacer3d()
{
    return new gpl3d::Placer3d();
}

void initPlacer3d(OpenRoad* openroad)
{
    Tcl_Interp* tcl_interp = openroad->tclInterp();
    Gpl3d_Init(tcl_interp);
    utl::evalTclInit(tcl_interp, gpl3d::gpl3d_tcl_inits);

    gpl3d::Placer3d* kernel = openroad->getPlacer3d();

    kernel->init(openroad->getDb(),
                 openroad->getDbNetwork(),
                 openroad->getSta(),
                 openroad->getLogger());
}

void deletePlacer3d(gpl3d::Placer3d* placer3d)
{
    delete placer3d;
}

} //namespace ord