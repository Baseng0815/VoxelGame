#include "../../../include/GameData/BlockStates/BlockStates.hpp"

#include "../../../include/Components/BlockStateComponent.hpp"

std::unordered_map<glm::vec3, entt::entity, std::hash<glm::vec3>> BlockStates::m_states = std::unordered_map<glm::vec3, entt::entity, std::hash<glm::vec3>>{};

template<BlockId id>
BlockStateComponent<id>& BlockStates::getBlockState<id>(const entt::registry& registry, const glm::vec3& blockPos) {
    if(m_states.contains(blockPos)) {
        entt::entity stateEntity = m_states[blockPos];
        return registry.get<BlockStateComponent<id>>(entity);
    }
}

template<BlockId id>
void setBlockState(const glm::vec3& blockPos, const BlockState<id>& state) {
    entt::entity entity;
    if(m_states.contains(blockPos)) {
        entity = m_states[blockPos];
    }
}

