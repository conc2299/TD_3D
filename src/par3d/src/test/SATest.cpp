#include "../SACore.h"

#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

class Position
{
public:
    Position(double x, double y) : x(x), y(y) {}
    double x;
    double y;
    double distanceTo(const Position& other) const {
        return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
    friend std::ostream& operator<<(std::ostream& os,const Position& pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class TSP : public par3d::Representation
{
public:
    TSP(const std::vector<Position>& cities, size_t seed)
        : cities_(cities), generator_(seed){}
    void initialize() override;
    double evaluateCost() override;
    void perturb() override;
    void restoreBest() override;
    void restorePrev() override;
    bool isValid() override;
    void printBestPath() const {
        std::cout << "Best Path: ";
        for (int city : best_path_) {
            std::cout << cities_[city] << " ";
        }
        std::cout << "\nBest Cost: " << best_cost << std::endl;
    }
private:
    std::vector<Position> cities_;
    std::mt19937 generator_;
    std::vector<int> path_;
    std::vector<int> prev_path_;
    double best_cost;
    std::vector<int> best_path_;
};

void TSP::initialize() {
    path_.resize(cities_.size());
    for (size_t i = 0; i < cities_.size(); ++i) {
        path_[i] = i;
    }
    std::shuffle(path_.begin(), path_.end(), generator_);
    best_path_ = path_;
    best_cost = evaluateCost();
}

double TSP::evaluateCost() {
    double cost = 0.0;
    for (size_t i = 0; i < path_.size() - 1; ++i) {
        size_t next_index = (i + 1) % path_.size();
        cost += cities_[path_[i]].distanceTo(cities_[path_[next_index]]);
    }
    if (cost < best_cost) {
        best_cost = cost;
        best_path_ = path_;
    }
    return cost;
}

void TSP::perturb() {
    prev_path_ = path_;
    std::uniform_int_distribution<size_t> dist(0, path_.size() - 1);
    size_t i = dist(generator_);
    size_t j = dist(generator_);
    std::swap(path_[i], path_[j]);
}

void TSP::restoreBest() {
    path_ = best_path_;
}

void TSP::restorePrev() {
    path_ = prev_path_;
}

bool TSP::isValid() {
    // In this simple case, we assume the path is always valid
    return true;
}

int main() {
    std::vector<Position> cities;
    std::mt19937 rng(0); // Seed for reproducibility
    std::uniform_int_distribution<int> dist(0, 20);
    for (int i = 0; i < 10; ++i) {
        cities.emplace_back(dist(rng), dist(rng));
    }

    TSP tsp(cities, 42);
    par3d::SACore sa_core(&tsp, 100.0, 0.995, 1.0, 50, 42);
    sa_core.run();
    tsp.printBestPath();
    return 0;
}