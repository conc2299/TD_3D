#include "SACore.h"

#include <random>
#include <iostream>

namespace par3d {

void SACore::run()
{
    // Initialize the FP representation
    representation_->initialize();

    // Set up the random number generator
    std::mt19937 generator(seed_);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double current_temp = init_temp_;
    double best_cost = representation_->evaluateCost();
    double current_cost = best_cost;

    for (int iter = 0; current_temp >= min_temp_; ++iter) {
        for (int perturb = 0; perturb < num_perturb_per_step_; ++perturb) {
            representation_->perturb();
            double new_cost = representation_->evaluateCost();

            if (new_cost < current_cost || 
                distribution(generator) < exp((current_cost - new_cost) / current_temp)) {
                current_cost = new_cost;
                if (current_cost < best_cost) {
                    best_cost = current_cost;
                }
            } else {
                representation_->restorePrev();
            }
        }
        std::cout << "Iteration " << iter << ": Temp = " << current_temp << ", Current Cost = " << current_cost 
                  << ", Best Cost = " << best_cost << std::endl; 

        // Cool down the temperature
        current_temp *= temp_decay_;
    }

    // Finalize the FP representation
    representation_->restoreBest();
}

}