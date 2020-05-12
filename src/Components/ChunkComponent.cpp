#include "../../include/Components/ChunkComponent.h"
#include "../../include/Configuration.h"
#include "../../include/Block.h"
#include "../../include/Event.h"
#include "../../include/EventDispatcher.h"

std::unordered_map<glm::vec2, entt::entity, HashFunction> ChunkComponent::chunksLookup 
	= std::unordered_map<glm::vec2, entt::entity, HashFunction>();

ChunkComponent::ChunkComponent(int chunkX, int chunkZ) :
	chunkX(chunkX), chunkZ(chunkZ) {
}

ChunkComponent::ChunkComponent(const ChunkComponent& other) :
	blocks(other.blocks), blockMutex(other.blockMutex), verticesOutdated((bool)other.verticesOutdated),
	threadActiveOnSelf((bool)other.threadActiveOnSelf), chunkX(other.chunkX), chunkZ(other.chunkZ) {
}

ChunkComponent& ChunkComponent::operator=(const ChunkComponent& other) {
	blocks = other.blocks;
	blockMutex = other.blockMutex;

	verticesOutdated = (bool)other.verticesOutdated;

	threadActiveOnSelf = (bool)other.threadActiveOnSelf;

	chunkX = other.chunkX;
	chunkZ = other.chunkZ;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk) {
	stream << "chunkX: " << chunk.chunkX << " chunkZ: " << chunk.chunkZ << " buffersInitialized: "
		<< chunk.verticesOutdated << " thread active: " << chunk.threadActiveOnSelf;
	return stream;

}

entt::entity ChunkComponent::getChunk(int chunkX, int chunkZ) {
	glm::vec2 key = glm::vec2(chunkX, chunkZ);

	return chunksLookup[key];
}

Block ChunkComponent::getBlock(entt::registry& registry, int x, int y, int z) {
	int chunkX = x / Configuration::CHUNK_SIZE;
	int chunkZ = z / Configuration::CHUNK_SIZE;
	if (x < 0)
		chunkX -= 1;

	if (z < 0)
		chunkZ -= 1;

	int cx = x % Configuration::CHUNK_SIZE;	
	if (cx < 0)
		cx = Configuration::CHUNK_SIZE - abs(cx);

	int cy = y;
	int cz = z % Configuration::CHUNK_SIZE;
	if (cz < 0)
		cz = Configuration::CHUNK_SIZE - abs(cz);

	auto entity = getChunk(chunkX, chunkZ);
	ChunkComponent& chunk = registry.get<ChunkComponent>(entity);

	std::unique_lock blockLock(*chunk.blockMutex);
	Block block = chunk.blocks[cx][cy][cz];
	blockLock.unlock();

	return block;
}

void ChunkComponent::setBlock(entt::registry& registry, int x, int y, int z, Block block) {
	int chunkX = x / Configuration::CHUNK_SIZE;
	int chunkZ = z / Configuration::CHUNK_SIZE;
	if (x < 0)
		chunkX -= 1;

	if (z < 0)
		chunkZ -= 1;

	int cx = x % Configuration::CHUNK_SIZE;
	if (cx < 0)
		cx = Configuration::CHUNK_SIZE - abs(cx);

	int cy = y;
	int cz = z % Configuration::CHUNK_SIZE;
	if (cz < 0)
		cz = Configuration::CHUNK_SIZE - abs(cz);

	auto entity = getChunk(chunkX, chunkZ);
	ChunkComponent& chunk = registry.get<ChunkComponent>(entity);

	std::unique_lock blockLock(*chunk.blockMutex);
	chunk.blocks[cx][cy][cz] = block;
	blockLock.unlock();

	BlockChangedEvent blockChangedEvent;
	blockChangedEvent.blockX = x;
	blockChangedEvent.blockY = y;
	blockChangedEvent.blockZ = z;

	EventDispatcher::raiseEvent(&blockChangedEvent);
}

void ChunkComponent::addChunk(entt::entity entity, int chunkX, int chunkZ) {
	chunksLookup[glm::vec2(chunkX, chunkZ)] = entity;
}

void ChunkComponent::removeChunk(entt::entity entity) {
	auto it = chunksLookup.begin();

	while (it != chunksLookup.end() && (*it).second != entity) {
		it++;
	}

	if (it != chunksLookup.end()) {
		chunksLookup.erase(it);
	}
}

