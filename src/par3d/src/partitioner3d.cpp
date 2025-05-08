#include "odbStats.h"
#include "staStats.h"

#include "par3d/partitioner3d.h"
#include "par/PartitionMgr.h"
#include "db_sta/SpefWriter.hh"
#include "sta/Units.hh"
#include "sta/Parasitics.hh"


namespace par3d {

using utl::PAR3D;
using sta::SpefWriter;

    
void Partitioner3d::init(
    odb::dbDatabase* db,
    sta::dbNetwork* db_network,
    sta::dbSta* sta,
    utl::Logger* logger
)
{
    db_ = db;
    db_network_ = db_network;
    sta_ = sta;
    logger_ = logger;
}

void Partitioner3d::run(
    int num_parts,
    float balance_constraint,
    unsigned int seed,
    unsigned int top_n,
    const char* solution_path
)
{
    // sta_engine->ensureGraph();
    // sta_->setDebugLevel("parasitic_reduce",2);
    sta_->setThreadCount(7);
    auto delay_calc = sta_->arcDelayCalc()->name();
    logger_->report(
        "Arc delay calc method : {}",
        delay_calc
    );
    /*
        run triton part
        and write the solution to the file
    */
    num_parts_ = num_parts;
    total_area(db_, logger_);
    partition_tritonpart(
        num_parts_,  // num_parts_arg
        balance_constraint,  // balance_constraint_arg
        seed,  // seed_arg
        top_n,  // top_n_arg
        true,  // timing_aware_flag_arg
        0.0f,  // extra_delay
        true,  // guardband_flag_arg
        solution_path
    );
    
}

void Partitioner3d::extract_parasitics(
    const char* spef_path
)
{
    std::map<sta::Corner*, std::ostream*> corner_map;
    std::ofstream spef_file(spef_path);
    corner_map[sta_->cmdCorner()] = &spef_file;
    if (spef_file.is_open() == false) {
        corner_map[sta_->cmdCorner()] = nullptr;
        logger_->warn(PAR3D, 10, "No spef file. Path : {}", spef_path);
    }
    SpefWriter spef_writer(
        logger_,
        sta_,
        corner_map
    );
    estimate_partitioned_parasitics(&spef_writer);
}

void Partitioner3d::report_timing(
    const char* report_file
)
{
    /*
        print information about db & sta
    */
    // odb_info(db_, logger_);
    // show_part_layer(db_, logger_);
    // logger_->report("==========Before Parasitic Estimation==========");
    // report_sta_units(sta_, logger_);
    // report_parasitic_annotation(sta_);
    // run timing analysis
    timing_analysis(false);
}

void Partitioner3d::partition_tritonpart(
    unsigned int num_parts_arg,
    float balance_constraint_arg,
    unsigned int seed_arg,
    unsigned int top_n_arg,
    bool timing_aware_flag_arg,
    float extra_delay,
    bool guardband_flag_arg,
    const char* solution_filename_arg
)
{
    par::PartitionMgr partition_mgr;
    partition_mgr.init(db_, db_network_, sta_, logger_);
    partition_mgr.tritonPartDesign(
        num_parts_arg,  // num_parts_arg
        balance_constraint_arg,  // balance_constraint_arg
        {1.0},  // base_balance_arg
        {1.0},  // scale_factor_arg
        seed_arg,  // seed_arg
        timing_aware_flag_arg,  // timing_aware_flag_arg
        top_n_arg,     // top_n_arg
        false,  // placement_flag_arg
        false,  // fence_flag_arg
        0.0f,   //fence_lx_arg
        0.0f,  //fence_ly_arg
        0.0f, // fence_ux_arg
        0.0f,   //fence_uy_arg
        "",  // fixed_file_arg
        "",  // community_file_arg
        "",  // group_file_arg
        solution_filename_arg,  // solution_filename_arg
        1.0f,  // net_timing_factor
        1.0f,  // path_timing_factor
        1.0f,  // path_snaking_factor
        1.0f,  // timing_exp_factor
        extra_delay,  // extra_delay
        guardband_flag_arg,  // guardband_flag
        { 1.0 },  // e_wt_factors
        { 1.0 },   // v_wt_factors
        {},
        1000,   //thr_coarsen_hyperedge_size_skip
        10,   //thr_coarsen_vertices
        50,  //thr_coarsen_hyperedges
        1.5, //coarsening_ratio
        30, //max_coarsen_iters
        0.0001 , //adj_diff_ratio
        0, //min_num_vertices_each_part
        100, //num_initial_solutions
        10, //num_best_initial_solutions
        100, //refiner_iters
        100, //max_moves
        0.5, //early_stop_ratio
        25, //total_corking_passes
        true, //v_cycle_flag
        1, //max_num_vcycle
        4, //num_coarsen_solutions
        40, //num_vertices_threshold_ilp
        1000 //global_net_threshold
    );
}

}
