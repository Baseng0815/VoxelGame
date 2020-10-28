#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>

#include "../BlockIds.hpp"
#include "BlockState.hpp"


template <BlockId id>
struct BlockStateComponent<id>;

class BlockStates {
  private:
    static std::unordered_map<glm::vec3, entt::entity, std::hash<glm::vec3>> m_states;

  public:
    template<BlockId id>
    static BlockStateComponent<id>& getBlockState(const entt::registry& registry, const glm::vec3& blockPos);

    template <BlockId id>
    static void setBlockState(const glm::vec3& blockPos, const BlockState<id>& state);
};