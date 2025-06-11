#pragma once

#include <vector>
#include <random>
#include "SACore.h"
#include "Hypergraph.hpp"
#include "Block.h"
#include "odb/db.h"

namespace par3d{

using Position = std::pair<double, double>;  // x, y coordinates

class SequencePairTwoLayers : public Representation
{
public:
    SequencePairTwoLayers(
        HyperGraph<odb::dbNet*, Block>& hypergraph,
        size_t seed
    );
    void initialize() override;
    double evaluateCost() override;
    void perturb() override;
    void restoreBest() override;
    void restorePrev() override;
    bool isValid() override;
    void printFloorplan() const;


private:
    std::vector<int> positiveSeq1;
    std::vector<int> negativeSeq1;
    std::vector<int> positiveSeq2;
    std::vector<int> negativeSeq2;
    std::vector<double> blockAR_;  // aspect ratio of each block
    std::mt19937 generator_;

    void packFloorplan();
    double getBlockWidth(size_t blockId);
    double getBlockHeight(size_t blockId);
    HyperGraph<odb::dbNet*, Block>& hypergraph_;
    std::vector<Position> positions_;  // positions of the block lower-left corner in the floorplan

    //
    std::vector<int> bestPositiveSeq1;
    std::vector<int> bestNegativeSeq1;
    std::vector<int> bestPositiveSeq2;
    std::vector<int> bestNegativeSeq2;
    std::vector<double> bestBlockAR_;  // aspect ratio of each block
    double bestCost = 0.0;

    //
    std::vector<int> prevPositiveSeq1;
    std::vector<int> prevNegativeSeq1;
    std::vector<int> prevPositiveSeq2;
    std::vector<int> prevNegativeSeq2;
    std::vector<double> prevBlockAR_;  // aspect ratio of each block

};


}