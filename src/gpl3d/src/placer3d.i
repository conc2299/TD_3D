
%{
#include "gpl3d/Placer3d.h"

namespace ord{
    gpl3d::Placer3d* getPlacer3d();
}
using ord::getPlacer3d;


%}

%inline 
%{
void placer3d_run(
    int argc
)
{
    getPlacer3d()->run(argc);
}
%}

