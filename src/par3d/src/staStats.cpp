#include "staStats.h"

#include "db_sta/dbNetwork.hh"
#include "sta/Units.hh"
#include "sta/Parasitics.hh"
#include "sta/Sdc.hh"
#include "odb/db.h"

namespace par3d{

using utl::PAR3D;
using sta::PinSet;
using sta::Pin;


void report_sta_units(sta::dbSta* sta, utl::Logger* logger)
{
    auto unit = sta->units();
    logger->report("Capacitance Unit: {}", unit->find("capacitance")->suffix());
    logger->report("Resistance Unit: {}", unit->find("resistance")->suffix());
    logger->report("Time Unit: {}", unit->find("time")->suffix());
}

void report_parasitic_annotation(sta::dbSta* sta)
{
    sta->reportParasiticAnnotation(
        true,
        sta->cmdCorner()
    );
}

void report_constraint(odb::dbDatabase* db, sta::dbNetwork* network, sta::dbSta* sta, utl::Logger* logger)
{
    std::vector<Pin*> constrained_pins;
    std::vector<Pin*> unconstrained_pins;

    odb::dbSet<odb::dbBTerm> bterms = db->getChip()->getBlock()->getBTerms();
    for(auto bterm : bterms) {
        logger->info(PAR3D, 1, "BTerm name: {}", bterm->getName());
        Pin* pin = network->dbToSta(bterm);
        if (pin == nullptr) {
            continue;
        }
        if (network->isTopLevelPort(pin)) {
            if (sta->sdc()->isConstrained(pin)) {
                constrained_pins.push_back(pin);
            } else {
                unconstrained_pins.push_back(pin);
            }
        }
    }
    // report constrained pins
    logger->report("Constrained pins:");
    logger->report("  {} constrained pins", constrained_pins.size());
    for (auto pin = constrained_pins.begin(); pin != constrained_pins.end(); ++pin) {
        logger->report("  {}", network->name(*pin));
    }
    // report unconstrained pins
    logger->report("Unconstrained pins:");
    logger->report("  {} unconstrained pins", unconstrained_pins.size());
    for (auto pin = unconstrained_pins.begin(); pin != unconstrained_pins.end(); ++pin) {
        logger->report("  {}", network->name(*pin));
    }
}

}