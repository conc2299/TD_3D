#include "tryodb.h"

#include "odb/db.h"
#include "odb/dbSet.h"
#include "sta/Search.hh"
#include "sta/PathEnd.hh"
#include "sta/Path.hh"
#include "sta/Clock.hh"
#include "sta/Corner.hh"
#include "sta/ClkNetwork.hh"
#include "sta/ExceptionPath.hh"
#include "boost/range/adaptor/sliced.hpp"

using utl::GPL3D;

void odb_info(odb::dbDatabase* db, utl::Logger* logger) {
    odb::dbBlock* block = db->getChip()->getBlock();

    odb::dbSet<odb::dbInst> insts = block->getInsts();
    odb::dbSet<odb::dbNet> nets = block->getNets();
    odb::dbSet<odb::dbBTerm> bterms = block->getBTerms();
    odb::dbSet<odb::dbRow> rows = block->getRows();

    logger->report("Number of instances: {}", insts.size());
    for(auto inst : insts) {
        logger->report("Instance name: {}", inst->getName());
        logger->report("    Instance type: {}", inst->getMaster()->getName());
        logger->report("    Instance width: {}", inst->getBBox()->getDX());
        logger->report("    Instance height: {}", inst->getBBox()->getDY());
        inst->setLocation(0,0);
        int location_x = 0;
        int location_y = 0;
        inst->getLocation(location_x, location_y);
        logger->report("    Placement status : {}", inst->getPlacementStatus().getString());
        logger->report("    Instance location : {}", odb::Point(location_x, location_y));
    }
    logger->report("Number of nets: {}", nets.size());
    for(auto net : nets) {
        logger->report("Net name: {}", net->getName());
        logger->report("    Net bbox: {}", net->getTermBBox());
        logger->report("    Net term count: {}", net->getTermCount());
    }
    logger->report("Number of pins: {}", bterms.size());
    logger->report("Number of rows: {}", rows.size());
    for(auto row : rows) {
        logger->report("Row name: {}", row->getName());
        logger->report("    Row bbox: {}", row->getBBox());
        logger->report("    Site Count: {}", row->getSiteCount());
    }
}

void odb_show(odb::dbDatabase* db, sta::dbSta* sta_engine, utl::Logger* logger){
    odb_info(db, logger);
    // sta_engine->ensureGraph();
    sta_engine->setThreadCount(6);

    
    sta_engine->searchPreamble();
    // sta_engine->ensureLevelized();

    sta::ExceptionFrom* e_from = nullptr;
    sta::ExceptionThruSeq* e_thrus = nullptr;
    sta::ExceptionTo* e_to = nullptr;
    bool include_unconstrained = false;
    bool get_max = true;  // max for setup check, min for hold check
    // Timing paths are grouped into path groups according to the clock
    // associated with the endpoint of the path, for example, path group for clk
    int group_count = 100;
    int endpoint_count = 1;  // The number of paths to report for each endpoint.

    auto sta_search = sta_engine->search();

    sta::PathEndSeq path_ends = sta_search->findPathEnds(
        e_from,
        e_thrus,
        e_to,
        include_unconstrained,
        sta_engine->cmdCorner(),
        sta::MinMaxAll::max(),
        group_count,
        endpoint_count,
        true,
        -sta::INF,
        sta::INF,
        true,
        nullptr,
        get_max,
        !get_max,
        false,
        false,
        false,
        false
    );
    logger->report("Number of path ends: {}\n", path_ends.size());
    sta_engine->setReportPathFormat(sta::ReportPathFormat::full_clock_expanded);
    for (auto path_end : path_ends) {
        sta_engine->reportPathEnd(path_end);
        // logger->report("Path end name: {}", path->name(sta_engine));
        // logger->report("Path arrival time: {}", path->arrival(sta_engine));
        // logger->report("Path required: {}", path_end->requiredTime(sta_engine));
        // logger->report("Path slew: {}", path->slew(sta_engine));
        // logger->report("Path slack: {}", path_end->slack(sta_engine));
        // logger->report("Path clockname: {}", path->clock(sta_engine)->name());
        // logger->report("");
        // logger->report("Path end delay: {}", path_end->pathDelay()->delay());
    }
}

