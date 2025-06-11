#pragma once
#include <vector>
#include <set>
#include <cstddef>
#include <stdexcept>

namespace par3d{

template <typename NetInfo, typename NodeInfo>
class HyperGraph 
{
public:
    HyperGraph(){};
    HyperGraph(
        const std::vector<NodeInfo>& nodeInfo,
        const std::vector<std::set<size_t>>& hyperedge,
        const std::vector<NetInfo>& netInfo
    );
    size_t getNumNodes() const { return nodes.size(); }
    size_t getNumNets() const { return nets.size(); }
    std::set<size_t>& getConnectedNodeIds(size_t hyperedgeId);
    std::set<size_t>& getConnectedNetIds(size_t nodeId);
    NetInfo& getNetInfo(size_t netId) { return nets[netId]; }
    NodeInfo& getNodeInfo(size_t nodeId) { return nodes[nodeId]; }

private:
    // NodeInfo contains the information of a node, such as its total area.
    std::vector<NodeInfo> nodes;
    // NetInfo containes the information of a net, such as the corresponding dbNet in odb
    std::vector<NetInfo> nets;  
    
    // data structure for mutually find node from hyperedge and vice versa
    std::vector<std::set<size_t>> nodes_in_hyperedge;  // hyperedges[i] is a list of node ids
    std::vector<std::set<size_t>> hyperedges_contain_node;  // nodes_in_hyperedge[i] is a list of hyperedge ids

    std::vector<int> fixed_attr;  // the block id of fixed vertices
    std::vector<int> community_attr;  // community attribute
};

template <typename NetInfo, typename NodeInfo>
HyperGraph<NetInfo, NodeInfo>::HyperGraph(
    const std::vector<NodeInfo>& nodeInfo,
    const std::vector<std::set<size_t>>& hyperedge,
    const std::vector<NetInfo>& netInfo)
    : nodes(nodeInfo), nets(netInfo), nodes_in_hyperedge(hyperedge)
{
    // Initialize hyperedges_contain_node
    hyperedges_contain_node.resize(nodes.size());
    for (size_t i = 0; i < hyperedges_contain_node.size(); ++i) {
        hyperedges_contain_node[i].clear();
    }
    
    for (size_t i = 0; i < nodes_in_hyperedge.size(); ++i) {
        for (int nodeId : nodes_in_hyperedge[i]) {
            hyperedges_contain_node[nodeId].insert(i);
        }
    }

}

template <typename NetInfo, typename NodeInfo>
std::set<size_t>& HyperGraph<NetInfo, NodeInfo>::getConnectedNodeIds(size_t hyperedgeId)
{
    if (hyperedgeId < nodes_in_hyperedge.size()) {
        return nodes_in_hyperedge[hyperedgeId];
    } else {
        throw std::out_of_range("Hyperedge ID out of range");
    }
}

template <typename NetInfo, typename NodeInfo>
std::set<size_t>& HyperGraph<NetInfo, NodeInfo>::getConnectedNetIds(size_t nodeId)
{
    if (nodeId < hyperedges_contain_node.size()) {
        return hyperedges_contain_node[nodeId];
    } else {
        throw std::out_of_range("Node ID out of range");
    }
}

}