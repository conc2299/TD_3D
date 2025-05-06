#include "gpl3d/Placer3d.h"

#include "utl/Logger.h"

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

void Placer3d::run(int argc)
{
    debugPrint(
        logger_,
        GPL3D,
        "run",
        1,
        "total arg count {}",
        argc
    );
}

}