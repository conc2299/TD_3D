#include "Block.h"

namespace par3d{

Block::Block(BlockType type, std::string name): type(type),blockName(name)
{
    totalInstArea = 0;
    blockArea = 0;
    numPins = 0;
    minimalHeight = SIZE_MAX;
    minimalWidth = SIZE_MAX;
}

// void Block::setModuleDimension(size_t w, size_t h)
// {
//     width = w;
//     height = h;
// }

void Block::mergeBlock(const Block& other)
{
    if (this->type == BlockType::HARD_BLOCK || other.type == BlockType::HARD_BLOCK)
    {
        throw std::runtime_error("Cannot merge hard block with another block at present");
    }
    // merge insts
    totalInstArea += other.totalInstArea;
    insts.insert(other.insts.begin(), other.insts.end());
    // merge nets
    // traverse all nets in the other block,
    // if the net does not appear the current block, add it
    // if the net appears in the current block, delete the net if the it has 2 pins
    for (auto net : other.nets) {
        if (nets.find(net) == nets.end()) {
            nets.insert(net);
            numPins++;
        } else {
            // if the net is already in the current block, check if it has 2 pins
            if (net->getITerms().size() + net->getBTerms().size() <= 2) {
                nets.erase(net);
                numPins -= 2;
            }
        }
    }
}

void Block::addInst(odb::dbInst* inst)
{
    insts.insert(inst);
    totalInstArea += inst->getMaster()->getArea();
    if (inst->getMaster()->getWidth() < minimalWidth) {
        minimalWidth = inst->getMaster()->getWidth();
    }
    if (inst->getMaster()->getHeight() < minimalHeight) {
        minimalHeight = inst->getMaster()->getHeight();
    }
}

void Block::addNet(odb::dbNet* net)
{
    nets.insert(net);
    numPins++;
}

bool Block::contains(odb::dbInst* inst) const
{
    return std::find(insts.begin(), insts.end(), inst) != insts.end();
}

}