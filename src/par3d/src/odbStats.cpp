#include "odbStats.h"

#include "odb/db.h"
#include "odb/dbSet.h"

using utl::PAR3D;

void show_part_layer(odb::dbDatabase* db, utl::Logger* logger) {
    logger->report("=====Partition Layer Information:=====");
    odb::dbBlock* block = db->getChip()->getBlock();
    for(auto bterm : block->getBTerms()) {
        auto name = bterm->getName();
        auto layer = odb::dbIntProperty::find(bterm, "partition_id");
        if (layer) {
            logger->report("BTerm: {} Layer: {}", name, layer->getValue());
        } else {
            logger->report("BTerm: {} Layer: not found", name);
        }
    }
    for(auto inst : block->getInsts()) {
        auto name = inst->getName();
        auto layer = odb::dbIntProperty::find(inst, "partition_id");
        if (layer) {
            logger->report("Inst: {} Layer: {}", name, layer->getValue());
        } else {
            logger->report("Inst: {} Layer: not found", name);
        }
    }
}

void odb_info(odb::dbDatabase* db, utl::Logger* logger) {
    odb::dbBlock* block = db->getChip()->getBlock();
    odb::dbSet<odb::dbInst> insts = block->getInsts();
    odb::dbSet<odb::dbBTerm> bterms = block->getBTerms();
    odb::dbSet<odb::dbNet> nets = block->getNets();
    odb::dbSet<odb::dbRow> rows = block->getRows();

    logger->report("=====ODB Information:=====");
    logger->report("Number of instances: {}", insts.size());
    for(auto inst : insts) {
        logger->report("Instance name: {}", inst->getName());
        logger->report("    Instance type: {}", inst->getMaster()->getName());
        logger->report("    Instance width: {}", inst->getBBox()->getDX());
        logger->report("    Instance height: {}", inst->getBBox()->getDY());
        logger->report("    Placement status : {}", inst->getPlacementStatus().getString());
    }
    logger->report("Number of pins: {}", bterms.size());
    for(auto pin : bterms) {
        logger->report("Pin name: {}", pin->getName());
        logger->report("    pin width: {}", pin->getBBox().dx());
        logger->report("    pin height: {}", pin->getBBox().dy());
    }
    logger->report("Number of nets: {}", nets.size());
    for(auto net : nets) {
        logger->report("Net name: {}", net->getName());
        logger->report("    Net bbox: {}", net->getTermBBox());
        logger->report("    Net term count: {}", net->getTermCount());
    }
    logger->report("Number of rows: {}", rows.size());
    for(auto row : rows) {
        logger->report("Row name: {}", row->getName());
        logger->report("    Row bbox: {}", row->getBBox());
        logger->report("    Site Spacing: {}", row->getSpacing());
        logger->report("    Site Count: {}", row->getSiteCount());
    }
}


