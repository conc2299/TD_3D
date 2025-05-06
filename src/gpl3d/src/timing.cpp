#include "gpl3d/Placer3d.h"

#include "db_sta/dbSta.hh"
#include "sta/Search.hh"
#include "sta/PathEnd.hh"
#include "sta/Path.hh"
#include "sta/Clock.hh"
#include "sta/Corner.hh"
#include "sta/ClkNetwork.hh"
#include "sta/ExceptionPath.hh"
#include "sta/StringSet.hh"

using namespace gpl3d;

void Placer3d::timing_analysis(
    bool after_placement_arg
)
{
    // sta_engine->ensureGraph();
    sta_->setThreadCount(1);
    // sta_->setArcDelayCalc("dmp_ceff_elmore");
    auto delay_calc = sta_->arcDelayCalc()->name();
    logger_->report(
        "Arc delay calc method : {}",
        delay_calc
    );

    
    sta_->searchPreamble();
    // sta_engine->ensureLevelized();

    sta::ExceptionFrom* e_from = nullptr;
    sta::ExceptionThruSeq* e_thrus = nullptr;
    sta::ExceptionTo* e_to = nullptr;
    bool include_unconstrained = false;
    bool get_max = true;  // max for setup check, min for hold check
    // Timing paths are grouped into path groups according to the clock
    // associated with the endpoint of the path, for example, path group for clk
    int group_count = 100;
    int endpoint_count = 3;  // The number of paths to report for each endpoint.

    auto sta_search = sta_->search();
    
    // sta_->setAnalysisType(sta::AnalysisType::bc_wc);
    sta::PathEndSeq path_ends = sta_search->findPathEnds(
        e_from,
        e_thrus,
        e_to,
        include_unconstrained,
        sta_->cmdCorner(),
        sta::MinMaxAll::all(),
        group_count,
        endpoint_count,
        false,
        -sta::INF,
        sta::INF,
        true,
        nullptr,
        true,
        true,
        false,
        false,
        false,
        false
    );
    sta_->setReportPathFormat(sta::ReportPathFormat::full_clock_expanded);
    for (auto path_end : path_ends) {
        sta_->reportPathEnd(path_end);
        // logger->report("Path end name: {}", path->name(sta_engine));
        // logger->report("Path arrival time: {}", path->arrival(sta_engine));
        // logger->report("Path required: {}", path_end->requiredTime(sta_engine));
        // logger->report("Path slew: {}", path->slew(sta_engine));
        // logger->report("Path slack: {}", path_end->slack(sta_engine));
        // logger->report("Path clockname: {}", path->clock(sta_engine)->name());
        // logger->report("");
        // logger->report("Path end delay: {}", path_end->pathDelay()->delay());
    }
    printf("Path end count: %d\n", path_ends.size());
    sta_->reportParasiticAnnotation(
        true,
        sta_->cmdCorner()
    );
}