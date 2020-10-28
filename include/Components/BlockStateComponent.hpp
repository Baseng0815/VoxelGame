#pragma once

#include "../GameData/BlockStates/BlockState.hpp"
#include <glm/glm.hpp>

template<BlockId id>
struct BlockStateComponent {
    glm::vec3 blockPosition;

    BlockState<id> state;
};