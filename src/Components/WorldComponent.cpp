#include "../../include/Components/WorldComponent.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"
#include "../../include/Event.h"
#include "../../include/EventDispatcher.h"

#include "../../include/Components/ChunkComponent.h"

void WorldComponent::getChunkCoords(glm::ivec3 worldPos, glm::ivec2& chunkPos, glm::ivec3& localPos) {
	chunkPos = glm::ivec2(worldPos.x, worldPos.z) / Configuration::CHUNK_SIZE;

	int cx = worldPos.x % Configuration::CHUNK_SIZE;
	int cz = worldPos.z % Configuration::CHUNK_SIZE;
	if (cx < 0)
		cx = Configuration::CHUNK_SIZE - abs(cx);
	if (cz < 0)
		cz = Configuration::CHUNK_SIZE - abs(cz);

	if (worldPos.x < 0 && cx != 0)
		chunkPos.x -= 1;

	if (worldPos.z < 0 && cz != 0)
		chunkPos.y -= 1;

	localPos = glm::ivec3(cx, worldPos.y, cz);
}

entt::entity WorldComponent::getChunk(glm::ivec2 chunk) const {
	//try {
		return chunksLookup.at(chunk);
	/*}
	catch (std::out_of_range) {
		return entt::entity();
	}*/
}

Block WorldComponent::getBlock(entt::registry& registry, glm::ivec3 position) const {
	glm::ivec2 chunkPosition;
	glm::ivec3 localPosition;

	getChunkCoords(position, chunkPosition, localPosition);

	auto entity = getChunk(chunkPosition);

	ChunkComponent& chunk = registry.get<ChunkComponent>(entity);

	std::unique_lock blockLock(*chunk.blockMutex);
	Block block = chunk.blocks[localPosition.x][localPosition.y][localPosition.z];
	blockLock.unlock();

	return block;
}

void WorldComponent::setBlock(entt::registry& registry, glm::ivec3 position, Block block) {
	glm::ivec2 chunkPosition;
	glm::ivec3 localPosition;

	getChunkCoords(position, chunkPosition, localPosition);

	auto entity = getChunk(chunkPosition);
	ChunkComponent& chunk = registry.get<ChunkComponent>(entity);

	std::unique_lock blockLock(*chunk.blockMutex);
	chunk.blocks[localPosition.x][localPosition.y][localPosition.z] = block;
	blockLock.unlock();

	BlockChangedEvent blockChangedEvent;
	blockChangedEvent.position = position;

	EventDispatcher::raiseEvent(&blockChangedEvent);
}

void WorldComponent::addChunk(entt::entity entity, glm::ivec2 position) {
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