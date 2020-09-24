#pragma once
#include "../GameData/BlockIds.hpp"
#include "../Resources/Geometry.hpp"

struct ItemComponent {
    BlockId blockId;

    Geometry *geometry;
};