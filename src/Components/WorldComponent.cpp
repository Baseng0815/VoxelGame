#include "../../include/Components/WorldComponent.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"
#include "../../include/Event.h"
#include "../../include/EventDispatcher.h"
#include "../../include/Utility.h"

#include "../../include/Components/ChunkComponent.h"

entt::entity WorldComponent::getChunk(glm::vec2 chunk) const {	
	return chunksLookup.at(chunk);
}

Block WorldComponent::getBlock(entt::registry& registry, glm::vec3 position) const {
	glm::vec2 chunkPosition;
	glm::vec3 localPosition;

	chunkPosition = GetChunk(position, localPosition);

	auto entity = getChunk(chunkPosition);

	ChunkComponent& chunk = registry.get<ChunkComponent>(entity);

	std::unique_lock blockLock(*chunk.blockMutex);
	Block block = chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z];
	blockLock.unlock();

	return block;
}

void WorldComponent::setBlock(entt::registry& registry, glm::vec3 position, Block block) {
	glm::vec2 chunkPosition;
	glm::vec3 localPosition;

	chunkPosition = GetChunk(position, localPosition);

	auto entity = getChunk(chunkPosition);
	ChunkComponent& chunk = registry.get<ChunkComponent>(entity);

	std::unique_lock blockLock(*chunk.blockMutex);
	chunk.blocks[(int)localPosition.x][(int)localPosition.y][(int)localPosition.z] = block;
	blockLock.unlock();

	BlockChangedEvent blockChangedEvent;
	blockChangedEvent.position = position;

	EventDispatcher::raiseEvent(&blockChangedEvent);
}

void WorldComponent::addChunk(entt::entity entity, glm::vec2 position) {
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