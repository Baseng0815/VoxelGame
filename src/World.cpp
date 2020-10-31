#include "../include/World.hpp"

#include "../include/Events/EventDispatcher.hpp"

#include "../include/Components/ChunkComponent.hpp"

std::unordered_map<glm::vec2, entt::entity, Utility::HashFunctionVec2> World::chunksLookup;

entt::entity World::getChunk(const glm::vec2& chunk)
{
    return chunksLookup[chunk];
}

bool World::chunkCreated(const glm::vec2& chunk)
{
    return chunksLookup.contains(chunk);
}

Block World::getBlock(const entt::registry* registry, const glm::vec3& position)
{
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    if (!chunkCreated(chunkPosition)) {
        return Block();
    }

    auto entity = getChunk(chunkPosition);

    const ChunkComponent& chunk = registry->get<ChunkComponent>(entity);

    Block block = chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z];

    return block;
}

void World::setBlock(entt::registry* registry, const glm::vec3& position, Block block)
{
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    auto entity = getChunk(chunkPosition);
    ChunkComponent& chunk = registry->get<ChunkComponent>(entity);

    BlockId prevBlock = chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z].type;

    chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z] = block;
    chunk.verticesOutdated = true;

    BlockChangedEvent blockChangedEvent{nullptr, position, prevBlock, block.type};
    EventDispatcher::raiseEvent(blockChangedEvent);
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
