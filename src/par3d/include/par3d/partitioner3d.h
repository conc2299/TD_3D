#pragma once

namespace odb {
class dbDatabase;
class dbInst;

}  // namespace odb
namespace sta {
class Net;
class dbSta;
class dbNetwork;
class SpefWriter;
class Parasitic;
}

namespace utl {
class Logger;
}

namespace par3d {
class PartResult;

class Partitioner3d
{
public:
    Partitioner3d() = default;
    ~Partitioner3d() = default;

    void init(odb::dbDatabase* db,
              sta::dbNetwork* db_network,
              sta::dbSta* sta,
              utl::Logger* logger);

    void estimate_partitioned_parasitics(sta::SpefWriter* spef_writer = nullptr);
    void run(int num_parts,
            float balance_constraint,
            unsigned int seed,
            unsigned int top_n,
            const char* solution_path
        );
    void extract_parasitics(
        const char* spef_path
    );
    void report_timing(
        const char* report_file
    );

private:
    void add_net_parasitic(
        sta::Parasitic* parasitic,
        sta::Net* net,
        PartResult* part_result
    );
    void partition_tritonpart(
        unsigned int num_parts_arg,
        float balance_constraint_arg,
        unsigned int seed_arg,
        unsigned int top_n_arg,
        bool timing_aware_flag_arg,
        float extra_delay,
        bool guardband_flag_arg,
        const char* solution_filename_arg
    );

    void timing_analysis(
        bool after_placement_arg
    );

    void place();
    double vccap;
    double vcres;
    int num_parts_;
    odb::dbDatabase* db_ = nullptr;
    sta::dbNetwork* db_network_ = nullptr;
    sta::dbSta* sta_ = nullptr;
    utl::Logger* logger_ = nullptr;
    odb::dbDatabase* pardb_ = nullptr;
};

}  // namespace gpl3d