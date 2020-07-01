#include "../../include/Components/ChunkComponent.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/Events/Event.h"
#include "../../include/Events/EventDispatcher.h"

ChunkComponent::ChunkComponent(std::mutex* blockMutex, int chunkX, int chunkZ) :
	blockMutex(blockMutex), chunkX(chunkX), chunkZ(chunkZ) {
}

ChunkComponent::ChunkComponent(const ChunkComponent& other) :
	blocks(other.blocks), biomes(other.biomes), blockMutex(other.blockMutex), verticesOutdated((bool)other.verticesOutdated),
	threadActiveOnSelf((bool)other.threadActiveOnSelf), chunkX(other.chunkX), chunkZ(other.chunkZ) {
}

ChunkComponent& ChunkComponent::operator=(const ChunkComponent& other) {
	blocks = other.blocks;
	blockMutex = other.blockMutex;
	biomes = other.biomes;

	verticesOutdated = (bool)other.verticesOutdated;

	threadActiveOnSelf = (bool)other.threadActiveOnSelf;

	chunkBlocksCreated = (bool)other.chunkBlocksCreated;

	chunkX = other.chunkX;
	chunkZ = other.chunkZ;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk) {
	stream << "chunkX: " << chunk.chunkX << " chunkZ: " << chunk.chunkZ << " buffersInitialized: "
		<< chunk.verticesOutdated << " thread active: " << chunk.threadActiveOnSelf;
	return stream;

}

