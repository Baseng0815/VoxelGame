#include "../include/World.h"

#include "../include/Events/EventDispatcher.h"

#include "../include/Components/ChunkComponent.h"

std::unordered_map<glm::vec2, entt::entity, Utility::HashFunctionVec2> World::chunksLookup;

entt::entity World::getChunk(const glm::vec2& chunk) {
    // .at() macht bounds-checking, was man nicht braucht; einfach nichts Falsches eingeben
    return chunksLookup[chunk];
}

bool World::chunkCreated(const glm::vec2& chunk) {
    return chunksLookup.contains(chunk);
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

    chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z] = block;

    BlockChangedEvent blockChangedEvent;
    blockChangedEvent.position = position;

    EventDispatcher::raiseEvent(blockChangedEvent);
}

void World::addChunk(entt::entity entity, const glm::vec2& position) {
    chunksLookup[position] = entity;
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
