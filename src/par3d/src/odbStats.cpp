#include "odbStats.h"

#include "odb/db.h"
#include "odb/dbSet.h"
#include "db_sta/dbNetwork.hh"
#include "sta/Sta.hh"
#include "HierarchyTree.hpp"
#include "NetlistToBlock.h"
#include "SequencePair.h"



using utl::PAR3D;

void show_part_layer(odb::dbDatabase* db, utl::Logger* logger)
{
  logger->report("=====Partition Layer Information:=====");
  odb::dbBlock* block = db->getChip()->getBlock();
  for (auto bterm : block->getBTerms()) {
    auto name = bterm->getName();
    auto layer = odb::dbIntProperty::find(bterm, "partition_id");
    if (layer) {
      logger->report("BTerm: {} Layer: {}", name, layer->getValue());
    } else {
      logger->report("BTerm: {} Layer: not found", name);
    }
  }
  for (auto inst : block->getInsts()) {
    auto name = inst->getName();
    auto layer = odb::dbIntProperty::find(inst, "partition_id");
    if (layer) {
      logger->report("Inst: {} Layer: {}", name, layer->getValue());
    } else {
      logger->report("Inst: {} Layer: not found", name);
    }
  }
}

void odb_info(odb::dbDatabase* db, utl::Logger* logger)
{
  odb::dbBlock* block = db->getChip()->getBlock();
  odb::dbSet<odb::dbInst> insts = block->getInsts();
  odb::dbSet<odb::dbBTerm> bterms = block->getBTerms();
  odb::dbSet<odb::dbNet> nets = block->getNets();
  odb::dbSet<odb::dbRow> rows = block->getRows();

  logger->report("=====ODB Information:=====");
  logger->report("Number of instances: {}", insts.size());
  for (auto inst : insts) {
    logger->report("Instance name: {}", inst->getName());
    logger->report("    Instance type: {}", inst->getMaster()->getName());
    logger->report("    Instance width: {}", inst->getBBox()->getDX());
    logger->report("    Instance height: {}", inst->getBBox()->getDY());
    logger->report("    Placement status : {}",
                   inst->getPlacementStatus().getString());
  }
  logger->report("Number of pins: {}", bterms.size());
  for (auto pin : bterms) {
    logger->report("Pin name: {}", pin->getName());
    logger->report("    pin width: {}", pin->getBBox().dx());
    logger->report("    pin height: {}", pin->getBBox().dy());
  }
  logger->report("Number of nets: {}", nets.size());
  for (auto net : nets) {
    logger->report("Net name: {}", net->getName());
    logger->report("    Net bbox: {}", net->getTermBBox());
    logger->report("    Net term count: {}", net->getTermCount());
  }
  logger->report("Number of rows: {}", rows.size());
  for (auto row : rows) {
    logger->report("Row name: {}", row->getName());
    logger->report("    Row bbox: {}", row->getBBox());
    logger->report("    Site Spacing: {}", row->getSpacing());
    logger->report("    Site Count: {}", row->getSiteCount());
  }
}

void total_area(odb::dbDatabase* db, utl::Logger* logger)
{
  odb::dbBlock* block = db->getChip()->getBlock();
  odb::dbSet<odb::dbInst> insts = block->getInsts();
  odb::dbSet<odb::dbBTerm> bterms = block->getBTerms();

  double total_inst_area = 0.0;
  double total_bterm_area = 0.0;
  for (auto inst : insts) {
    double dx
        = db->getChip()->getBlock()->dbuToMicrons(inst->getBBox()->getDX());
    double dy
        = db->getChip()->getBlock()->dbuToMicrons(inst->getBBox()->getDY());
    double area = dx * dy;
    total_inst_area += area;
  }
  for (auto bterm : bterms) {
    double area
        = db->getChip()->getBlock()->dbuAreaToMicrons(bterm->getBBox().area());
    total_bterm_area += area;
  }

  logger->report("=====Total Area:=====");
  logger->report("Total number of instances: {}", insts.size());
  logger->report("Total instance area: {} um^2", total_inst_area);
  logger->report("Total number of bterms: {}", bterms.size());
  logger->report("Total bterm area: {} um^2", total_bterm_area);
}

// void print_submodule_hierarchy(odb::dbModInst* module,
//                                utl::Logger* logger,
//                                int depth)
// {
//     std::string indent(depth,'\t');
//     logger->report("{}name:{}", indent,module->getName());
//     logger->report("{}Hierarchical name:{}", indent,module->getHierarchicalName());
//     auto submodinsts = module->get
//     // for (auto submodule : submodules)
//     // {
//     //     print_submodule_hierarchy(submodule,logger,depth+1);
//     // }
//     // logger->report("{}num insts:{}",indent,num_insts);
// }

std::vector<std::string> split_string(const std::string& s, char del)
{
    std::vector<std::string> tokens;
    size_t start = 0, end;
    while((end = s.find(del,start)) != std::string::npos)
    {
        tokens.push_back(s.substr(start,end - start));
        start = end + 1;
    }
    tokens.push_back(s.substr(start));
    return tokens;
}


using Info = odb::dbModInst*;
void print_hierarchy(odb::dbDatabase* db, utl::Logger* logger)
{
    auto top = db->getChip()->getBlock();
	logger->report("=====SubBlock Information=====");
	for(auto subblock : top->getChildren())
	{
		logger->report("SubBlock name: {}", subblock->getName());
		logger->report("SubBlock number of instances: {}", subblock->getInsts().size());
		logger->report("SubBlock number of nets: {}", subblock->getNets().size());
		logger->report("SubBlock number of pins: {}", subblock->getBTerms().size());
	}
    logger->report("=====Module infomation=====");
    logger->report("top name:{}", top->getName());

    HierarchyTree<Info> hierTree(
		nullptr
    );

    auto delimiter = top->getHierarchyDelimiter();
    for(auto modinst : top->getModInsts())
    {
        auto hiername = modinst->getHierarchicalName();
        hierTree.addHierarchyNode(split_string(hiername,delimiter),
			modinst
        );
    }

    auto print = [&](Info& info, size_t depth){
		if(depth > 0) {
			std::string indent(depth,'\t');
			logger->report("{}name:{}",indent,info->getName());
			logger->report("{}num insts:{}",indent,info->getMaster()->getInsts().size());
			logger->report("{}master name:{}",indent,info->getMaster()->getName());
			logger->report("{}hierarchy name:{}",indent,info->getHierarchicalName());
			// logger->report("{}num pins:{}",indent,std::get<4>(info));
			for(auto inst: info->getMaster()->getInsts())
			{
				if(inst->isBlock())
				{
					logger->report("{}inst name:{}",indent,inst->getName());
					logger->report("{}inst isHier:{}",indent,inst->isHierarchical());
				}
			}
		}
    };

    size_t area = 0;
    size_t nummodinst = 0;
    auto accumulate = [&](const Info& info, size_t depth){
        nummodinst++;
    };

    hierTree.dfsForeach(accumulate);
    logger->report("totalarea:{}",area);
    logger->report("nummodinst:{}",nummodinst);
    hierTree.dfsForeach(print);

	logger->report("=====End of Module infomation=====");
	logger->report("=====Partitioning Result=====");
	par3d::NetlistToBlockPartitioner netlistToBlockPartitioner(db, logger);
	auto hypergraph = netlistToBlockPartitioner.partitionMixType(0.01f);
	logger->report("Number of nodes: {}", hypergraph.getNumNodes());
	logger->report("Number of hyperedges: {}", hypergraph.getNumNets());
	logger->report("Node Information:");
	auto block = db->getChip()->getBlock();
	const double area_to_microns = std::pow(block->getDbUnitsPerMicron(), 2);
	for (size_t i = 0; i < hypergraph.getNumNodes(); ++i) {
		auto node = hypergraph.getNodeInfo(i);
		logger->report("Node {}: name = {}, type = {}. total area = {}, inst num = {}, num pins = {}",
			i,node.getName(), node.getTypeName(), node.getTotalInstArea() / area_to_microns, node.getInsts().size(),
			node.getNumPins()
		);
	}
	logger->report("Hyperedge Information:");
	for (size_t i = 0; i < hypergraph.getNumNets(); ++i) {
		auto net = hypergraph.getNetInfo(i);
		logger->report("Hyperedge {}: name = {}, weight = {},",i, net->getName(), net->getWeight());
		std::string connectedNodes = "\t";
		for (const auto& node : hypergraph.getConnectedNodeIds(i)) {
			connectedNodes += std::to_string(node) + " ";
		}
		logger->report("{}", connectedNodes);	
	}
	auto sp = par3d::SequencePairTwoLayers(
		hypergraph,
		0
	);
	sp.initialize();
	sp.evaluateCost();
	sp.printFloorplan();

}
