
%{
#include "par3d/partitioner3d.h"

namespace ord{
    par3d::Partitioner3d* getPartitioner3d();
}
using ord::getPartitioner3d;


%}

%inline 
%{
void partitioner3d_run(
    const char* solution_path,
    const char* spef_path
)
{
    getPartitioner3d()->run(solution_path,spef_path);
}
%}
    
    