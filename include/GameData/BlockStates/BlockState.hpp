#pragma once
#include "../BlockIds.hpp"

struct BlockState;

struct BlockState {
    BlockId blockId;

    bool outdated = true;

    static constexpr BlockState empty() {
        return BlockState{BlockId::BLOCK_AIR, false};
    }

    bool operator==(const BlockState& state) const {
        return blockId == state.blockId;
    }
};
