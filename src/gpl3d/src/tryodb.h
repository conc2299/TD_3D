#pragma once

#include "odb/db.h"
#include "db_sta/dbSta.hh"
#include "utl/Logger.h"

void odb_show(odb::dbDatabase* db, sta::dbSta* sta_engine, utl::Logger* logger);


void odb_info(odb::dbDatabase* db, utl::Logger* logger);