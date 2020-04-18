#include "../../include/Components/ChunkComponent.h"

ChunkComponent::ChunkComponent(int chunkX, int chunkZ) :
    chunkX(chunkX), chunkZ(chunkZ) {}

ChunkComponent::ChunkComponent(const ChunkComponent& other) :
    blocks(other.blocks), blockMutex(other.blockMutex), verticesOutdated((bool)other.verticesOutdated),
    threadActiveOnSelf((bool)other.threadActiveOnSelf), chunkX(other.chunkX), chunkZ(other.chunkZ) {}

ChunkComponent& ChunkComponent::operator=(const ChunkComponent& other) {
    blocks = other.blocks;
    blockMutex = other.blockMutex;

    verticesOutdated    = (bool)other.verticesOutdated;

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
