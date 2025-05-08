#include <map>

#include "PartResult.h"

#include "par3d/partitioner3d.h"
#include "db_sta/dbSta.hh"
#include "db_sta/dbNetwork.hh"
#include "db_sta/SpefWriter.hh"
#include "odb/db.h"
#include "sta/Corner.hh"
#include "sta/Parasitics.hh"
#include "sta/ArcDelayCalc.hh"
#include "sta/Network.hh"


namespace par3d {

using sta::Net;
using sta::Pin;
using sta::PinSet;
using sta::Corner;
using sta::Parasitic;
using sta::Parasitics;
using sta::ParasiticNode;
using sta::ParasiticAnalysisPt;
using sta::ArcDelayCalc;
using sta::SpefWriter;
using sta::NetConnectedPinIterator;

using utl::PAR3D;

#define TSV_CAP 50e-15
#define TSV_RES 0.02
#define EPS_RES 1e-6

void Partitioner3d::add_net_parasitic(
    Parasitic* parasitic,
    Net* net,
    PartResult* part_result
)
{
    Parasitics* parasitics = sta_->parasitics();
    int min_id = part_result->min_netspan_id();
    int max_id = part_result->max_netspan_id();
    if(min_id != max_id) {
        ParasiticNode* lastNode = nullptr;
        size_t resistor_id = 0;
        for(int i = min_id; i <= max_id; i++) {
            ParasiticNode* currentNode = parasitics->ensureParasiticNode(
                parasitic, net, i, db_network_);
            const std::vector<Pin*>& pins = part_result->get_pins(i);
            // connect the pins to the current node
            for (Pin* pin : pins) {
                ParasiticNode* pinNode = parasitics->ensureParasiticNode(parasitic, pin, db_network_);
                parasitics->makeResistor(parasitic, resistor_id++, EPS_RES, currentNode, pinNode);
            }
            // add pi model parasitic
            if(lastNode != nullptr) {
                parasitics->makeResistor(parasitic, resistor_id++, TSV_RES, lastNode, currentNode);
                parasitics->incrCap(lastNode, TSV_CAP / 2.0);
                parasitics->incrCap(currentNode, TSV_CAP / 2.0);
            }
            lastNode = currentNode;
        }
    }
}

void Partitioner3d::estimate_partitioned_parasitics(SpefWriter* spef_writer)
{
    odb::dbBlock* block = db_->getChip()->getBlock();    
    odb::dbSet<odb::dbNet> nets = block->getNets();

    ArcDelayCalc* arc_delay_calc = sta_->arcDelayCalc();
    PartResult part_result(logger_);
    part_result.set_minmax_id(0, num_parts_ - 1);
    int tsv_num = 0;
    for(odb::dbNet* net : nets) {
        // logger_->info(PAR3D, 1, "Net name: {}", net->getName());
        part_result.handle_net(db_network_, net);
        Net* sta_net = db_network_->dbToSta(net);
        tsv_num += part_result.get_tsv_num();
        // logger_->info(PAR3D, 1, "\tmin_id: {}, max_id: {}", min_id, max_id);
        for(Corner* corner : *sta_->corners()) {
            const ParasiticAnalysisPt* parasitics_ap
                = corner->findParasiticAnalysisPt(sta::MinMax::max());
            Parasitic* parasitic
                = sta_->makeParasiticNetwork(sta_net, true, parasitics_ap);
            add_net_parasitic(parasitic, sta_net, &part_result);
            arc_delay_calc->reduceParasitic(
                parasitic, sta_net, sta_->cmdCorner(), sta::MinMaxAll::all());
            if (spef_writer && part_result.get_tsv_num() > 0) {
                spef_writer->writeNet(corner, sta_net, parasitic);
            }
        }
    }
    logger_->info(PAR3D, 1, "Total TSV num: {}", tsv_num);
}

}