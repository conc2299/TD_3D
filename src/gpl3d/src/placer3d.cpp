#include "gpl3d/Placer3d.h"
#include "odbStats.h"

#include "utl/Logger.h"
#include "par/PartitionMgr.h"

namespace gpl3d{

using utl::GPL3D;

void Placer3d::init(odb::dbDatabase* db,
            sta::dbNetwork* db_network,
            sta::dbSta* sta,
            utl::Logger* logger)
{
    db_ = db;
    db_network_ = db_network;
    sta_ = sta;
    logger_ = logger;
}

void Placer3d::run(char* solution_path)
{
    debugPrint(
        logger_,
        GPL3D,
        "run",
        1,
        "solution file path {}",
        solution_path
    );
    logger_->report(
        "Starting TritonPart partitioning...\n"
    );
    partition_tritonpart(
        2,  // num_parts_arg
        1.0,  // balance_constraint_arg
        42,  // seed_arg
        1000,  // top_n_arg
        true,  // timing_aware_flag_arg
        0.0f,  // extra_delay
        true,  // guardband_flag_arg
        solution_path // solution_filename_arg
    );
    logger_->report(
        "Finish TritonPart partitioning\n"
    );
    show_part_layer(db_, logger_);
    odb_info(db_, logger_);
    // Perform timing analysis after partitioning
    timing_analysis(true); 
    // odb_show(db_,sta_,logger_);
}

void Placer3d::partition_tritonpart(
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

void Placer3d::place()
{
    auto insts = db_->getChip()->getBlock()->getInsts();
}

}
