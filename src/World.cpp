#include "../include/Events/Event.h"
#include "../include/Events/EventDispatcher.h"
#include "../include/Utility.h"

#include "../include/Components/ChunkComponent.h"

#include <iostream>

<<<<<<< HEAD:src/World.cpp
std::unordered_map<glm::vec2, entt::entity, HashFunction> World::chunksLookup;

entt::entity World::getChunk(glm::vec2 chunk) {
    return chunksLookup.at(chunk);
}

bool World::chunkCreated(glm::vec2 chunk) {
entt::entity WorldComponent::getChunk(const glm::vec2 &chunk) const {
    return chunksLookup.at(chunk);
}

bool WorldComponent::chunkCreated(const glm::vec2 &chunk) const {
    try {
        chunksLookup.at(chunk);
        return true;
    }
    catch (std::out_of_range e) {
        std::cout << "Error in chunk creation: " << e.what() << std::endl;
        return false;
    }
}

<<<<<<< HEAD:src/World.cpp
Block World::getBlock(const entt::registry* registry, glm::vec3 position) {
    glm::vec2 chunkPosition;
    glm::vec3 localPosition;

    chunkPosition = GetChunk(position, localPosition);
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
    catch (std::out_of_range) {
        return Block();
    }
}

<<<<<<< HEAD:src/World.cpp
void World::setBlock(entt::registry* registry, glm::vec3 position, Block block) {
    glm::vec2 chunkPosition;
    glm::vec3 localPosition;

    chunkPosition = GetChunk(position, localPosition);
void WorldComponent::setBlock(entt::registry* registry, const glm::vec3 &position, Block block) {
    auto [chunkPosition, localPosition] = Utility::GetChunkAndLocal(position);

    auto entity = getChunk(chunkPosition);
    ChunkComponent& chunk = registry->get<ChunkComponent>(entity);

    chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z] = block;

    BlockChangedEvent blockChangedEvent;
    blockChangedEvent.position = position;

    EventDispatcher::raiseEvent(blockChangedEvent);
}

<<<<<<< HEAD:src/World.cpp
void World::addChunk(entt::entity entity, glm::vec2 position) {
void WorldComponent::addChunk(entt::entity entity, const glm::vec2 &position) {
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
