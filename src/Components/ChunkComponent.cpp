/*
#include "../../include/Components/ChunkComponent.h"

#include "../../include/Configuration.h"
#include "../../include/Block.h"
#include "../../include/WorldGeneration/WorldGenerator.h"
#include "../../include/Events/Event.h"
#include "../../include/Events/EventDispatcher.h"

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
