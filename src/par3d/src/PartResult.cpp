#include "PartResult.h"

#include "odb/db.h"

#include "db_sta/dbNetwork.hh"

namespace par3d{

using sta::Pin;
using utl::PAR3D;

PartResult::PartResult(utl::Logger* logger)
    : logger_(logger),
      min_id_(INT_MAX),
      max_id_(INT_MIN),
      min_netspan_id_(INT_MAX),
      max_netspan_id_(INT_MIN)
{
}

void PartResult::set_minmax_id(int min_id, int max_id)
{
    min_id_ = min_id;
    max_id_ = max_id;
    layer_pins_.resize(max_id_ - min_id_ + 1);
}

void PartResult::handle_net(sta::dbNetwork* dbNetwork,odb::dbNet* net)
{
    // clear layer_pins_
    for(auto& layer_pins : layer_pins_) {
        layer_pins.clear();
    }
    min_netspan_id_ = INT_MAX;
    max_netspan_id_ = INT_MIN;

    //
    odb::dbSet<odb::dbITerm> iterms = net->getITerms();
    for (auto iterm : iterms) {
        auto property = odb::dbIntProperty::find(iterm->getInst(), "partition_id");
        if (property == nullptr) {
            logger_->warn(PAR3D, 10, "No partition id for iterm {} in net {}", iterm->getName(), net->getName());
        }
        int partition_id = property->getValue();
        if (partition_id < min_netspan_id_) {
            min_netspan_id_ = partition_id;
        }
        if (partition_id > max_netspan_id_) {
            max_netspan_id_ = partition_id;
        }
        Pin* iterm_pin = dbNetwork->dbToSta(iterm);
        layer_pins_[partition_id - min_id_].push_back(iterm_pin);
    }
    odb::dbSet<odb::dbBTerm> bterms = net->getBTerms();
    for (auto bterm : bterms) {
        // do the same thing as iterm
        auto property = odb::dbIntProperty::find(bterm, "partition_id");
        if (property == nullptr) {
            logger_->warn(PAR3D, 10, "No partition id for bterm {} in net {}", bterm->getName(), net->getName());
        }
        int partition_id = property->getValue();
        if (partition_id < min_netspan_id_) {
            min_netspan_id_ = partition_id;
        }
        if (partition_id > max_netspan_id_) {
            max_netspan_id_ = partition_id;
        }
        Pin* bterm_pin = dbNetwork->dbToSta(bterm);
        layer_pins_[partition_id - min_id_].push_back(bterm_pin);
    }
}

const std::vector<sta::Pin*>& PartResult::get_pins(int layer_id) const
{
    return layer_pins_[layer_id - min_id_];
}

int PartResult::min_netspan_id() const
{
    return min_netspan_id_;
}

int PartResult::max_netspan_id() const
{
    return max_netspan_id_;
}

}