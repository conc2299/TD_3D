#pragma once

namespace par3d{

class Representation
{
public:
    virtual void initialize() = 0;
    virtual double evaluateCost() = 0;
    virtual void perturb() = 0;
    virtual void restoreBest() = 0;
    virtual void restorePrev() = 0;
    virtual bool isValid() = 0;
};

class SACore
{
public:
    SACore(
        Representation* representation,
        double init_temp,
        double temp_decay,
        double min_temp,
        int num_perturb_per_step,
        unsigned seed
    )
        : representation_(representation),
          init_temp_(init_temp),
          temp_decay_(temp_decay),
          min_temp_(min_temp),
          num_perturb_per_step_(num_perturb_per_step),
          seed_(seed)
    {
    }

    void run();

private:
    Representation* representation_;
    double init_temp_;
    double temp_decay_;
    double min_temp_;
    int num_perturb_per_step_;
    unsigned seed_;

};

}