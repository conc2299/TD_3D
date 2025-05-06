#pragma once

namespace odb {
class dbDatabase;
class dbInst;

}  // namespace odb
namespace sta {
class dbSta;
class dbNetwork;
}

namespace utl {
class Logger;
}

namespace gpl3d {

class Placer3d
{
public:
    Placer3d() = default;
    ~Placer3d() = default;

    void init(odb::dbDatabase* db,
              sta::dbNetwork* db_network,
              sta::dbSta* sta,
              utl::Logger* logger);
    void run(char* solution_path);

private:
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

    odb::dbDatabase* db_ = nullptr;
    sta::dbNetwork* db_network_ = nullptr;
    sta::dbSta* sta_ = nullptr;
    utl::Logger* logger_ = nullptr;
};

class RandomPlacer3d
{
public:
    RandomPlacer3d(odb::dbDatabase* db, utl::Logger* logger);
    ~RandomPlacer3d();
    
    void init(odb::dbDatabase* db, utl::Logger* logger);
    
private:
    odb::dbDatabase* db_;
    utl::Logger* logger_;
};

}  // namespace gpl3d