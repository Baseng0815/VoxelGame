#include "../../include/Components/ChunkComponent.h"

ChunkComponent::ChunkComponent(ChunkComponent&& other) {
    blocks = other.blocks;

    buffersInitialized = other.buffersInitialized;
    verticesOutdated = other.verticesOutdated;
    buffersOutdated = other.buffersOutdated;

    vertices = other.vertices;
    indices = other.indices;
}

ChunkComponent& ChunkComponent::operator=(const ChunkComponent& other) {
    blocks = other.blocks;

    buffersInitialized = other.buffersInitialized;
    verticesOutdated = other.verticesOutdated;
    buffersOutdated = other.buffersOutdated;

    vertices = other.vertices;
    indices = other.indices;

    return *this;
}
