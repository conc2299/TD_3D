#include  <fstream>
#include  <iostream>

#include "db_sta/SpefWriter.hh"
#include "db_sta/dbNetwork.hh"
#include "sta/Network.hh"
#include "par3d/partitioner3d.h"


#include "vcmodel.h"

namespace par3d{

using utl::PAR3D;

bool Partitioner3d::write_spef_after_partition(
    const std::string& spef_path
)
{
    std::map<sta::Corner*, std::ostream*> spef_streams;
    spef_streams[sta_->cmdCorner()] = nullptr;
    spef_streams[sta_->cmdCorner()] = new std::ofstream(spef_path);

    sta::SpefWriter spef_writer(
        logger_,
        sta_,
        spef_streams
    ); 

    auto block = db_->getChip()->getBlock();
    odb::dbSet<odb::dbNet> nets = block->getNets();
    for(auto net:nets) {
        printf("net name: %s\n", net->getName());
        
    }

}


}