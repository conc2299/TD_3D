#include <fstream>


#include "par3d/partitioner3d.h"
#include "odb/db.h"
#include "utl/Logger.h"
#include "odbStats.h"


namespace par3d
{

using utl::PAR3D;

void Partitioner3d::generate_fixed_file(
    const char* fixed_file,
    const int port_fix_layer
)
{
    std::ofstream file(fixed_file);
    if (!file.is_open()) {
        logger_->error(PAR3D, 0, "Failed to open fixed file: {}", fixed_file);
        return;
    }
    logger_->info(PAR3D, 2, "Writing fixed file: {}, fix ports to layer_{}", fixed_file, port_fix_layer);
    auto block = db_->getChip()->getBlock();
    for (auto bterm : block->getBTerms()) {
        auto name = bterm->getName();
        file << name << " " << port_fix_layer << std::endl;
    }
    for (auto insts : block->getInsts()) {
        auto name = insts->getName();
        file << name << " " << -1 << std::endl;
    }
}

void Partitioner3d::print_module_hierarchy()
{
    print_hierarchy(db_,logger_);
}

}