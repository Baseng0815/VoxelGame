#include "../../include/Components/ChunkComponent.h"

ChunkComponent::ChunkComponent(int chunkX, int chunkZ) :
    chunkX(chunkX), chunkZ(chunkZ) {}

ChunkComponent::ChunkComponent(ChunkComponent&& other) {
    blocks = other.blocks;

    buffersInitialized = other.buffersInitialized;
    verticesOutdated = other.verticesOutdated;
    buffersOutdated = other.buffersOutdated;

    vertices = other.vertices;
    indices = other.indices;

    chunkX = other.chunkX;
    chunkZ = other.chunkZ;
}

ChunkComponent& ChunkComponent::operator=(const ChunkComponent& other) {
    blocks = other.blocks;

    buffersInitialized = other.buffersInitialized;
    verticesOutdated = other.verticesOutdated;
    buffersOutdated = other.buffersOutdated;

    vertices = other.vertices;
    indices = other.indices;

    chunkX = other.chunkX;
    chunkZ = other.chunkZ;

    return *this;
}

std::ostream& operator<<(std::ostream& stream, const ChunkComponent& chunk) {
    stream << "buffersInitialized: " << chunk.buffersInitialized << " buffersOutdated: " << chunk.buffersOutdated << " verticesOutdated: " << chunk.verticesOutdated;
    return stream;

}
