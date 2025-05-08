#pragma once

#include "odb/db.h"
#include "db_sta/dbSta.hh"
#include "utl/Logger.h"

void odb_info(odb::dbDatabase* db, utl::Logger* logger);

void show_part_layer(odb::dbDatabase* db, utl::Logger* logger);

void total_area(odb::dbDatabase* db, utl::Logger* logger);