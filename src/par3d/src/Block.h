#pragma once

#include <vector>
#include <set>
#include <string>

#include "odb/db.h"

namespace par3d{

enum BlockType
{
    HARD_BLOCK,
    SOFT_BLOCK
};

class Block
{
public:
    // constructors
    Block(
        BlockType type,
        std::string blockName
    );

    // mutators
    void setBlockArea(size_t area) { blockArea = area; }
    void mergeBlock(const Block& other);
    void addInst(odb::dbInst* inst);
    void addNet(odb::dbNet* net);

    // accessors
    double getMinimalAspectRatio() const;
    double getMaximalAspectRatio() const;
    size_t getNumPins() const {return numPins; };
    std::string getName() const { return blockName; }
    BlockType getType() const { return type; }
    std::string getTypeName() const
    {
        return type == HARD_BLOCK ? "HARD_BLOCK" : "SOFT_BLOCK";
    }
    size_t getTotalInstArea() const { return totalInstArea; }
    size_t getArea() const { return blockArea; }
    // size_t getWidth() const {}
    // size_t getHeight() const {}
    std::set<odb::dbInst*>& getInsts() {return insts;};
    bool contains(odb::dbInst* inst) const;
    bool operator<(const Block& other) const
    {
        return totalInstArea < other.totalInstArea;
    }
private:
    const BlockType type;
    const std::string blockName;
    size_t totalInstArea;
    size_t blockArea;
    size_t numPins;  // number of pins in the block
    std::set<odb::dbInst*> insts; // instances in the block
    std::set<odb::dbNet*> nets; // nets connected to the block
    size_t minimalWidth ; // minimal allowed width
    size_t minimalHeight ; // minimal allowed height
};

// class BlockSmallerThan
// {
// public:
//     bool operator()(const Block& a, const Block& b) const
//     {
//         return a.getArea() < b.getArea();
//     }
// };

}