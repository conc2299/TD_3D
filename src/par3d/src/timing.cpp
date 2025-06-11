#include "par3d/partitioner3d.h"

#include "db_sta/dbSta.hh"
#include "sta/Search.hh"
#include "sta/PathEnd.hh"
#include "sta/Path.hh"
#include "sta/Clock.hh"
#include "sta/Corner.hh"
#include "sta/ClkNetwork.hh"
#include "sta/ExceptionPath.hh"
#include "sta/Delay.hh"
#include "sta/Units.hh"

using namespace par3d;

void Partitioner3d::timing_analysis(
    unsigned int num_path
)
{
    sta_->searchPreamble();
    // sta_engine->ensureLevelized();
    sta::ExceptionFrom* e_from = nullptr;
    sta::ExceptionThruSeq* e_thrus = nullptr;
    sta::ExceptionTo* e_to = nullptr;
    bool include_unconstrained = false;
    // bool get_max = true;  // max for setup check, min for hold check
    // Timing paths are grouped into path groups according to the clock
    // associated with the endpoint of the path, for example, path group for clk
    int group_count = num_path;
    int endpoint_count = 1;  // The number of paths to report for each endpoint.

    // // sta_->setAnalysisType(sta::AnalysisType::bc_wc);
    auto sta_search = sta_->search();
    sta::PathEndSeq path_ends = sta_search->findPathEnds(
        e_from,
        e_thrus,
        e_to,
        include_unconstrained,
        sta_->cmdCorner(),
        sta::MinMaxAll::max(),
        group_count,
        endpoint_count,
        false,
        -sta::INF,
        sta::INF,
        true,
        nullptr,
        true,
        false,
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
    auto wns = sta_->worstSlack(sta::MinMax::max());
    auto tns = sta_->totalNegativeSlack(sta::MinMax::max());
    logger_->report("Worst negative slack: {}",sta_->units()->find("time")->asString(wns));
    logger_->report("Total negative slack: {}",sta_->units()->find("time")->asString(tns));
}

