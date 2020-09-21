#pragma once

#include "OreData.h"
#include "BlockIds.h"

#include <string>

struct BlockTemplate {
    std::string name;
    bool isSolid;

    // texture IDs
    int tid_py, tid_px, tid_nx, tid_pz, tid_nz, tid_ny;

    BlockId id;
    OreData oreData;
};
