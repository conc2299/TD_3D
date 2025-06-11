/*
    Partition the hierarchical netlist into multiple blocks
    input:
*/
#pragma once

#include "utl/Logger.h"
#include "odb/db.h"
#include "Block.h"
#include "Hypergraph.hpp"

namespace par3d{

class NetlistToBlockPartitioner
{
public:
    NetlistToBlockPartitioner(
        odb::dbDatabase* db,
        utl::Logger* logger
    );

    HyperGraph<odb::dbNet*, Block> partitionMixType(
        double minimalBlockSizeRatio = 0.01f,
        double softBlockWhitespaceRatio = 0.2f
    );
    
private:
    
    odb::dbDatabase* db_;
    utl::Logger* logger_;
};

}
