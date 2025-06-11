
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
    const char* solution_path,
    bool timing_aware_flag
)
{
    getPartitioner3d()->run(
        num_parts,
        balance_constraint,
        seed,
        top_n,
        solution_path,
        timing_aware_flag
    );
}

void partitioner3d_extract_parasitics(
    const char* spef_path
)
{
    getPartitioner3d()->extract_parasitics(spef_path);
}

void partitioner3d_report_timing(
    unsigned int num_path
)
{
    getPartitioner3d()->report_timing(num_path);
}

void partitioner3d_clear_parasitics()
{
    getPartitioner3d()->clear_parasitics();
}

void partitioner3d_print_module_hierarchy()
{
    getPartitioner3d()->print_module_hierarchy();
}

%}
    
    