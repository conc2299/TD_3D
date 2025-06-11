#include <unordered_map>
#include <set>
#include "NetlistToBlock.h"

namespace par3d{

NetlistToBlockPartitioner::NetlistToBlockPartitioner(
    odb::dbDatabase* db,
    utl::Logger* logger
) : db_(db), logger_(logger)
{}


std::multiset<Block> hierarchicalClustering(
    // the top module
    odb::dbModule* module,
    size_t minimalBlockSize
)
{
    // First generate block from submodules
    std::multiset<Block> blocks;
    for(auto submodinst : module->getModInsts())
    {
        std::multiset<Block>&& subBlocks = hierarchicalClustering(submodinst->getMaster(),minimalBlockSize);
        for(auto& block : subBlocks){
            blocks.insert(std::move(block));
        }
    }
    // Then generate block from macros and cells in this modinst
    Block cellBlock(BlockType::SOFT_BLOCK,module->getName());
    for(auto inst : module->getInsts())
    {
        // isBlock indicates whether the instance is a macro or not
        if(inst->isBlock())
        {
            Block macroBlock(BlockType::HARD_BLOCK,inst->getName());
            // macroBlock.setModuleDimension(
            //     inst->getMaster()->getWidth(),
            //     inst->getMaster()->getHeight()
            // );
            macroBlock.addInst(inst);
            blocks.insert(std::move(macroBlock));
        }
        else
        {
            // If the instance is not a macro, add all instance to the block
            // TODO: handle soft blocks
            cellBlock.addInst(inst);
        }
    }
    // If the block is too small, merge it with the previous blocks
    if (cellBlock.getTotalInstArea() < minimalBlockSize && !blocks.empty())
    {
        // Find the smallest block and merge it with the current cellBlock
        auto it = blocks.begin();
        // auto toMerge = blocks.extract(blocks.begin());
        // toMerge.value().mergeBlock(cellBlock);
        // blocks.insert(std::move(toMerge.value()));

        while(it->getType() != BlockType::SOFT_BLOCK && it != blocks.end()){
            ++it;
        }
        if(it != blocks.end() && it->getType() == BlockType::SOFT_BLOCK)
        {
            // Merge with the smallest soft block
            auto toMerge = blocks.extract(it);
            toMerge.value().mergeBlock(cellBlock);
            blocks.insert(std::move(toMerge.value()));
        }
        else
        {
            blocks.insert(std::move(cellBlock));
        }
    }
    else
    {
        blocks.insert(std::move(cellBlock));
    }
    return blocks;
}

void printBlock(Block& block, utl::Logger* logger)
{
    logger->report("Block type: {}, Block name: {},total area: {}, insts: {}",
                   block.getTypeName(),
                   block.getName(),
                   block.getTotalInstArea(),
                   block.getInsts().size());
}


HyperGraph<odb::dbNet*, Block> NetlistToBlockPartitioner::partitionMixType(
    double minimalBlockSizeRatio,
    double softBlockWhitespaceRatio
)
{
    // Implementation of the partitioning logic goes here.
    // This will involve traversing the database, creating HardBlock instances,
    // and populating the HyperGraph structure with nodes and hyperedges.
    auto block = db_->getChip()->getBlock();
    auto insts = block->getInsts();
    size_t totalCellArea = 0;
    for (auto inst : insts) {
        if (inst->getMaster()->isBlock()) {
            totalCellArea += inst->getMaster()->getArea();
        }
    }
    size_t minimalBlockSize = static_cast<size_t>(totalCellArea * minimalBlockSizeRatio);
    logger_->report("Total cell area: {}, minimal block size: {}",
                   totalCellArea, minimalBlockSize);
    std::unordered_map<odb::dbInst*, size_t> instToBlockId;
    auto blocks = hierarchicalClustering(block->getTopModule(),minimalBlockSize);
    // Create a mapping from inst to block id and construct nodeinfo vec
    size_t blockIdx = 0;
    std::vector<Block> nodeInfo;
    while(!blocks.empty())
    {
        auto block = blocks.extract(blocks.begin());
        if(block.value().getType() == BlockType::SOFT_BLOCK)
        {
            // For soft blocks, we need to consider the whitespace ratio
            size_t area = block.value().getTotalInstArea();
            size_t softBlockArea = static_cast<size_t>(area / (1 - softBlockWhitespaceRatio));
            block.value().setBlockArea(softBlockArea);
        }
        else
        {
            // For hard blocks, set the area to the total instance area
            block.value().setBlockArea(block.value().getTotalInstArea());
        }
        for(auto inst : block.value().getInsts())
        {
            instToBlockId[inst] = blockIdx;
        }
        nodeInfo.push_back(std::move(block.value()));
        blockIdx++;
    }

    // traverse all nets and create hyperedges if the net is connected to multiple blocks
    std::vector<std::set<size_t>> nets;
    std::vector<odb::dbNet*> netInfo;
    for (auto net : block->getNets())
    {
        std::set<size_t> connectedBlockIds;
        for (auto iterm : net->getITerms())
        {
            auto inst = iterm->getInst();
            if (inst)
            {
                auto id = instToBlockId.find(inst);
                if(id != instToBlockId.end())
                {
                    connectedBlockIds.insert(id->second);
                }
            }
        }
        // should I consider bterms??
        // TODO: handle bterms(package pins)
        if (connectedBlockIds.size() > 1) {
            for(auto blockId : connectedBlockIds)
            {
                nodeInfo[blockId].addNet(net);
            }
            nets.push_back(std::move(connectedBlockIds));
            netInfo.push_back(net);
        }
    }

    // Create the hypergraph
    return HyperGraph<odb::dbNet*, Block>(
        nodeInfo,
        nets,
        netInfo
    );
}


}