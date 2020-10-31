#pragma once

#include "OreData.hpp"
#include "BlockIds.hpp"

#include <string>

struct BlockTemplate {
    std::string name;
    bool isSolid;
    bool isTransparent;

    // texture IDs
    int tid_py, tid_px, tid_nx, tid_pz, tid_nz, tid_ny;

    BlockId id;
    OreData oreData;
};
