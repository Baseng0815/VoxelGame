#pragma once

#include "../GameData/BlockStates/BlockState.hpp"
#include <glm/glm.hpp>

struct BlockStateComponent {
    glm::vec3 blockPosition;

    BlockState state = BlockState::empty();    
};