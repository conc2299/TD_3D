#include "SequencePair.h"

#include <algorithm>

namespace par3d{

SequencePairTwoLayers::SequencePairTwoLayers(
    HyperGraph<odb::dbNet*, Block>& hypergraph,
    size_t seed
) : generator_(seed), hypergraph_(hypergraph)
{
    positiveSeq1.resize(hypergraph.getNumNodes());
    negativeSeq1.resize(hypergraph.getNumNodes());
    positiveSeq2.resize(hypergraph.getNumNodes());
    negativeSeq2.resize(hypergraph.getNumNodes());
    blockAR_.resize(hypergraph_.getNumNodes());
    positions_.resize(hypergraph.getNumNodes());
}

void SequencePairTwoLayers::initialize() {
    std::iota(positiveSeq1.begin(), positiveSeq1.end(), 0);
    std::iota(negativeSeq1.begin(), negativeSeq1.end(), 0);
    std::iota(positiveSeq2.begin(), positiveSeq2.end(), 0);
    std::iota(negativeSeq2.begin(), negativeSeq2.end(), 0);
    std::shuffle(positiveSeq1.begin(), positiveSeq1.end(), generator_);
    std::shuffle(positiveSeq2.begin(), positiveSeq2.end(), generator_);
    std::shuffle(negativeSeq1.begin(), negativeSeq1.end(), generator_);
    std::shuffle(negativeSeq2.begin(), negativeSeq2.end(), generator_);
    std::fill(blockAR_.begin(), blockAR_.end(), 1.0); // Initialize aspect ratios to 1.0
}

double SequencePairTwoLayers::evaluateCost() {
    packFloorplan();
}

void SequencePairTwoLayers::packFloorplan(){
    std::fill(positions_.begin(), positions_.end(), Position(0.0, 0.0));

    // calculate the x position of each block
    // here we use a simple algorithm that runs in O(n^2) time
    // TODO: a more efficient algorithm with longest commen subsequence might be used
    // later.
    
    std::vector<int> negSeqPos1(negativeSeq1.size());
    for (int i = 0; i < negativeSeq1.size(); i++) {
        negSeqPos1[negativeSeq1[i]] = i;
    }

    // Calculate the x position of each block in the first layer
    // nodes id in [map[n].first,map[n+1].first) should be placed at map[n].second 
    std::map<int,double> intervalCumulatedWidth;
    for (size_t i = 0; i < positiveSeq1.size(); i++) {
        int blockId = positiveSeq1[i];
        int negPos = negSeqPos1[blockId];
        auto find = std::lower_bound(intervalCumulatedWidth.rbegin(), intervalCumulatedWidth.rend(), negPos,
                         [](const auto& a, const auto& b) { return a.first > b; });
        double cumulatedWidth = find == intervalCumulatedWidth.rend() ? 0.0 : find->second;
        positions_[blockId].first = cumulatedWidth;
        double currentCumulatedWidth = cumulatedWidth + getBlockWidth(blockId);
        intervalCumulatedWidth[negPos] = currentCumulatedWidth;
        auto deleteIt = intervalCumulatedWidth.find(negPos);
        // remove intervals that are smaller than the current cumulated width
        ++deleteIt;
        while (deleteIt != intervalCumulatedWidth.end() && deleteIt->second <= currentCumulatedWidth){
            deleteIt = intervalCumulatedWidth.erase(deleteIt);
        }
    }
}

void SequencePairTwoLayers::printFloorplan() const {
    for (size_t i = 0; i < positions_.size(); i++) {
        std::cout << "Block " << i << ": (" << positions_[i].first << ", " << positions_[i].second << ")\n";
    }
}

double SequencePairTwoLayers::getBlockWidth(size_t blockId) {
    size_t area = hypergraph_.getNodeInfo(blockId).getArea();
    return std::sqrt(area * blockAR_[blockId]);
}

double SequencePairTwoLayers::getBlockHeight(size_t blockId) {
    size_t area = hypergraph_.getNodeInfo(blockId).getArea();
    return std::sqrt(area / blockAR_[blockId]);
}

void SequencePairTwoLayers::perturb() {
    // Randomly swap two blocks in the positive sequence of layer 1
}

void SequencePairTwoLayers::restoreBest() {
    positiveSeq1 = bestPositiveSeq1;
    negativeSeq1 = bestNegativeSeq1;
    positiveSeq2 = bestPositiveSeq2;
    negativeSeq2 = bestNegativeSeq2;
    blockAR_ = bestBlockAR_;
}

void SequencePairTwoLayers::restorePrev() {
    positiveSeq1 = prevPositiveSeq1;
    negativeSeq1 = prevNegativeSeq1;
    positiveSeq2 = prevPositiveSeq2;
    negativeSeq2 = prevNegativeSeq2;
    blockAR_ = prevBlockAR_;
}

bool SequencePairTwoLayers::isValid() {
    // Check if the sequences are valid
    return true;
}

}