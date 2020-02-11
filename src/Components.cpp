#include "../include/Components.h"

glm::mat4 TransformationComponent::getModelMatrix() const {
    glm::mat4 rotationMatrix =
        glm::rotate(rotation.z, glm::vec3(0, 0, 1)) *
        glm::rotate(rotation.y, glm::vec3(0, 1, 0)) *
        glm::rotate(rotation.x, glm::vec3(1, 0, 0));

    // order of operations
    // - translate - scale - rotate -
    return glm::translate(position) * glm::scale(scale) * rotationMatrix;
}

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