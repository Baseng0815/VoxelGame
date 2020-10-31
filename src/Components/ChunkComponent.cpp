#include "../../include/Components/ChunkComponent.hpp"

#include "../../include/GameData/BlockStates/WaterBlockState.hpp"

void ChunkComponent::setBlock(int x, int y, int z, const Block& block) {
    // type
    int typeIndex = -1;
    int index = 0;
    while (index < blockData.size() && typeIndex == -1) {
        if (block.type == blockData[index]) {
            typeIndex = index;
        }
        index++;
    }

    if (typeIndex == -1) {
        blockData.emplace_back(block.type);
        typeIndex = index;
    }

    // state
    int stateIndex = -1;
    if (block.state == nullptr) {
        stateIndex = 0;
    }
    else if (*(block.state) == WaterBlockState{}) {
        stateIndex = 1;
    }
    else {
        int oldStateIndex = blocks[x][y][z] & (0xFFFF0000 >> 16);
        if (oldStateIndex > 1) {
            stateIndex = oldStateIndex;
        }
        else {
            stateIndex = blockStates.size();
            blockStates.push_back(block.state);
        }
    }

    blocks[x][y][z] = (stateIndex << 16) | typeIndex;
}

const Block ChunkComponent::getBlock(int x, int y, int z) const {
    int blockTypeId = blocks[x][y][z] & 0x0000FFFF;
    int blockStateId = (blocks[x][y][z] & 0xFFFF0000) >> 16;

    return Block{blockData[blockTypeId], blockStates[blockStateId]};
}

Block ChunkComponent::getBlock(int x, int y, int z) {
    int blockTypeId = blocks[x][y][z] & 0x0000FFFF;
    int blockStateId = blocks[x][y][z] >> 16;

    return Block{blockData[blockTypeId], blockStates[blockStateId]};
}

template<typename s_type>
void ChunkComponent::setBlockState(int x, int y, int z, s_type* state) {
    // if (state == BlockState::empty()) {
    //     blocks[x][y][z] = blocks[x][y][z] & 0x0000FFFF;
    // }
    // else {
    //     int stateIndex = blocks[x][y][z] >> 4;

    //     s_type* newState = new s_type;
    //     std::copy<s_type, s_type>(*state, *newState);

    //     if (stateIndex != 0) {
    //         blockStates[stateIndex] = newState;
    //     }
    //     else {
    //         blockStates.emplace_back(newState);
    //         blocks[x][y][z] |= (blockStates.size() - 1) << 4;
    //     }
    // }
}

template<typename s_type>
const s_type* ChunkComponent::getBlockState(int x, int y, int z) const {
    int stateIndex = blocks[x][y][z] >> 16;
    return reinterpret_cast<s_type*>(blockStates[stateIndex]);
}

template<typename s_type>
s_type* ChunkComponent::getBlockState(int x, int y, int z) {
    int stateIndex = blocks[x][y][z] >> 16;
    return reinterpret_cast<s_type*>(blockStates[stateIndex]);
}

/*#include "../../include/Configuration.hpp"
#include "../../include/Events/Event.hpp"
#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/WorldGeneration/WorldGenerator.hpp"

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
