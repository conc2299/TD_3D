
#include "par3d/MakePartitioner3d.h"

#include "ord/OpenRoad.hh"
#include "par3d/partitioner3d.h"
#include "utl/decode.h"

namespace par3d{
    // Tcl files encoded into strings.
    extern const char* par3d_tcl_inits[];
} // namespace gpl3d

extern "C" {
    extern int Par3d_Init(Tcl_Interp* interp);
}

namespace ord{

par3d::Partitioner3d* makePartitioner3d()
{
    return new par3d::Partitioner3d();
}

void initPartitioner3d(OpenRoad* openroad)
{
    Tcl_Interp* tcl_interp = openroad->tclInterp();
    Par3d_Init(tcl_interp);
    utl::evalTclInit(tcl_interp, par3d::par3d_tcl_inits);

    par3d::Partitioner3d* kernel = openroad->getPartitioner3d();

    kernel->init(openroad->getDb(),
                 openroad->getDbNetwork(),
                 openroad->getSta(),
                 openroad->getLogger());
}

void deletePartitioner3d(par3d::Partitioner3d* part3d)
{
    delete part3d;
}

} //namespace ord