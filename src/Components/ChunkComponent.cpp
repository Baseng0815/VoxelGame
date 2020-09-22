/*
#include "../../include/Components/ChunkComponent.hpp"

#include "../../include/Configuration.hpp"
#include "../../include/Block.hpp"
#include "../../include/WorldGeneration/WorldGenerator.hpp"
#include "../../include/Events/Event.hpp"
#include "../../include/Events/EventDispatcher.hpp"

ChunkComponent::ChunkComponent(std::shared_mutex* blockMutex, int chunkX, int chunkZ) :
    blockMutex {blockMutex}, chunkX {chunkX}, chunkZ {chunkZ}
{ }

ChunkComponent::ChunkComponent(const ChunkComponent& other) :
    blocks {other.blocks}, biomes {other.biomes}, blockMutex {other.blockMutex}, verticesOutdated { other.verticesOutdated.load()},
    threadActiveOnSelf { other.threadActiveOnSelf.load()}, chunkX {other.chunkX}, chunkZ {other.chunkZ}
{ }

ChunkComponent& ChunkComponent::operator=(const ChunkComponent& other)
{
    if (this != &other) {
        blockMutex = other.blockMutex;
        chunkX = other.chunkX;
        chunkZ = other.chunkZ;

        blocks = other.blocks;
        biomes = other.biomes;

        verticesOutdated    = other.verticesOutdated.load();
        threadActiveOnSelf  = other.threadActiveOnSelf.load();
    }

    return *this;
}
*/
