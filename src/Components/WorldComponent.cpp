#include "../../include/Components/WorldComponent.h"

#include "../../include/Block.h"
#include "../../include/Utility.h"
#include "../../include/Events/Event.h"
#include "../../include/Configuration.h"
#include "../../include/Events/EventDispatcher.h"

#include "../../include/Components/ChunkComponent.h"

#include <iostream>

entt::entity WorldComponent::getChunk(const glm::vec2 &chunk) const {
    return chunksLookup.at(chunk);
}

bool WorldComponent::chunkCreated(const glm::vec2 &chunk) const {
    try {
        chunksLookup.at(chunk);
        return true;
    } catch (std::out_of_range e) {
        std::cout << "Error in chunk creation: " << e.what() << std::endl;
        return false;
    }
}

Block WorldComponent::getBlock(const entt::registry* registry, const glm::vec3 &position) const {
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    try {
        auto entity = getChunk(chunkPosition);

        const ChunkComponent& chunk = registry->get<ChunkComponent>(entity);

        if (!chunk.blocks) {
            throw std::out_of_range("blocks not created");
        }

        Block block = chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z];

        return block;
    }
    catch(std::out_of_range) {
        return Block();
    }
}

void WorldComponent::setBlock(entt::registry* registry, const glm::vec3 &position, Block block) {
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    auto entity = getChunk(chunkPosition);
    ChunkComponent& chunk = registry->get<ChunkComponent>(entity);

    chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z] = block;

    BlockChangedEvent blockChangedEvent;
    blockChangedEvent.position = position;

    EventDispatcher::raiseEvent(blockChangedEvent);
}

void WorldComponent::addChunk(entt::entity entity, const glm::vec2 &position) {
    chunksLookup[position] = entity;
}

void WorldComponent::removeChunk(entt::entity entity) {
    auto it = chunksLookup.begin();

    while (it != chunksLookup.end() && (*it).second != entity) {
        it++;
    }

    if (it != chunksLookup.end()) {
        chunksLookup.erase(it);
    }
}
