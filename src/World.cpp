#include "../include/World.hpp"

#include "../include/Events/EventDispatcher.hpp"

#include "../include/Components/ChunkComponent.hpp"

std::unordered_map<glm::ivec2, entt::entity, std::hash<glm::ivec2>> World::chunksLookup;
std::unordered_map<glm::ivec3, entt::entity, std::hash<glm::ivec3>> World::blockStates;

ChunkComponent& World::getChunk(entt::registry& registry, const glm::vec2& chunk) {
    return registry.get<ChunkComponent>(chunksLookup[glm::floor(chunk)]);
}

bool World::chunkCreated(const glm::vec2& chunk) {
    return chunksLookup.contains(glm::floor(chunk));
}

Block World::getBlock(entt::registry& registry, const glm::vec3& position) {
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    if (!chunkCreated(chunkPosition)) {
        return Block{};
    }

    const ChunkComponent& chunk = getChunk(registry, chunkPosition);

    Block block = chunk.getBlock(localPosition.x, localPosition.y, localPosition.z);

    return block;
}

void World::setBlock(entt::registry& registry, Block block)
{
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(block.position);

    if (chunkCreated(chunkPosition)) {
        ChunkComponent& chunk = getChunk(registry, chunkPosition);

        BlockId prevBlock = chunk.getBlock(localPosition.x, localPosition.y, localPosition.z).type;

        chunk.setBlock(block);
        chunk.state |= ChunkComponent::States::VERTICES_OUTDATED;

        BlockChangedEvent blockChangedEvent{nullptr, block.position, prevBlock, block.type};
        EventDispatcher::raiseEvent(blockChangedEvent);
    }
}

void World::addChunk(entt::entity entity, const glm::vec2& position)
{ chunksLookup[position] = entity; }

void World::removeChunk(entt::entity entity)
{
    auto it = chunksLookup.begin();

    while (it != chunksLookup.end() && (*it).second != entity) {
        it++;
    }

    if (it != chunksLookup.end()) {
        chunksLookup.erase(it);
    }
}
