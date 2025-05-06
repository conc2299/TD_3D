#pragma once

#include "db_sta/dbSta.hh"
#include "db_sta/dbNetwork.hh"
#include "db_sta/SpefWriter.hh"

namespace par3d{

void report_sta_units(sta::dbSta* sta,utl::Logger* logger);

void report_parasitic_annotation(sta::dbSta* sta);

void report_constraint(odb::dbDatabase* db, sta::dbNetwork* network, sta::dbSta* sta, utl::Logger* logger);


}

