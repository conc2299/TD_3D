#pragma once

#include <vector>

namespace odb {
class dbNet;
}

namespace sta{
class dbNetwork;
class Pin;
}

namespace utl{
class Logger;
}



namespace par3d{



class PartResult{
public:
    PartResult(utl::Logger* logger);
    ~PartResult() = default;
    // make sure the min_id and max_id are set
    void set_minmax_id(int min_id, int max_id);
    void handle_net(sta::dbNetwork* dbNetwork,odb::dbNet* net);
    const std::vector<sta::Pin*>& get_pins(int layer_id) const;
    int min_netspan_id() const;
    int max_netspan_id() const;
    
private:
    utl::Logger* logger_;
    int min_id_,max_id_;
    int min_netspan_id_,max_netspan_id_;
    // pin in layer_id is stored in layer_pins_[layer_id - min_id_]
    std::vector<std::vector<sta::Pin*>> layer_pins_;
};

}