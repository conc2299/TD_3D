
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
    int num_parts,
    float balance_constraint,
    unsigned int seed,
    unsigned int top_n,
    const char* solution_path
)
{
    getPartitioner3d()->run(
        num_parts,
        balance_constraint,
        seed,
        top_n,
        solution_path
    );
}

void partitioner3d_extract_parasitics(
    const char* spef_path
)
{
    getPartitioner3d()->extract_parasitics(spef_path);
}

void partitioner3d_report_timing(
    const char* report_file
)
{
    getPartitioner3d()->report_timing(report_file);
}

%}
    
    