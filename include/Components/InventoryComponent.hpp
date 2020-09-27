#pragma once
#include "../GameData/BlockIds.hpp"
#include <vector>

struct InventoryComponent {
    int slotsCount;
    std::vector<std::pair<BlockId, int>> slots;

    inline InventoryComponent(int slotsCount)
        : slotsCount{slotsCount}, slots{slotsCount} {
    }
};