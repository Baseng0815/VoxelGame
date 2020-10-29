#include "../include/World.hpp"

#include "../include/Events/EventDispatcher.hpp"

#include "../include/Components/ChunkComponent.hpp"
#include "../include/Components/BlockStateComponent.hpp"

std::unordered_map<glm::ivec2, entt::entity, std::hash<glm::ivec2>> World::chunksLookup;
std::unordered_map<glm::ivec3, entt::entity, std::hash<glm::ivec3>> World::blockStates;

entt::entity World::getChunk(const glm::vec2& chunk) {    
    return chunksLookup[glm::floor(chunk)];
}

bool World::chunkCreated(const glm::vec2& chunk) {
    return chunksLookup.contains(glm::floor(chunk));
}

Block World::getBlock(const entt::registry* registry, const glm::vec3& position) {
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    if (!chunkCreated(chunkPosition)) {
        return Block();
    }

    auto entity = getChunk(chunkPosition);

    const ChunkComponent& chunk = registry->get<ChunkComponent>(entity);

    Block block = chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z];

    return block;
}

void World::setBlock(entt::registry* registry, const glm::vec3& position, Block block) {
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    auto entity = getChunk(chunkPosition);
    ChunkComponent& chunk = registry->get<ChunkComponent>(entity);

    BlockId prevBlock = chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z].type;

    chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z] = block;
    chunk.verticesOutdated = true;

    BlockChangedEvent blockChangedEvent{nullptr, position, prevBlock, block.type};
    EventDispatcher::raiseEvent(blockChangedEvent);
}

BlockStateComponent& World::getBlockState(entt::registry& registry, const glm::vec3& blockPos) {
    if(blockStates.contains(blockPos)) {
        entt::entity stateEntity = blockStates[blockPos];
        return registry.get<BlockStateComponent>(stateEntity);
    }
    else
        throw std::exception{};
}

void World::setBlockState(entt::registry& registry, const glm::vec3& blockPos, BlockState& state) {
    if(blockStates.contains(blockPos)) {
        entt::entity stateEntity = blockStates[blockPos];
        BlockStateComponent& stateComponent = registry.get<BlockStateComponent>(stateEntity);
        stateComponent.state = state;
    }
    else {
        entt::entity stateEntity = registry.create();
        blockStates.emplace(blockPos, stateEntity);

        registry.emplace<BlockStateComponent>(stateEntity, blockPos, state);
    }
}

void World::addChunk(entt::entity entity, const glm::vec2& position) {
    chunksLookup[glm::floor(position)] = entity;
}

void World::removeChunk(entt::entity entity) {
    auto it = chunksLookup.begin();

    while (it != chunksLookup.end() && (*it).second != entity) {
        it++;
    }

    if (it != chunksLookup.end()) {
        chunksLookup.erase(it);
    }
}
