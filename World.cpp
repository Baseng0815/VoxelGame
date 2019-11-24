#include "pch.h"

#include "Block.h"
#include "World.h"

void World::init(/*WorldType worldType*/) {
	//m_generator.init(worldType);

	for (int x = 0; x < 7; x++)
		for (int y = 0; y < 7; y++) {
			//m_generator.generateChunk(position, &c);

			Chunk chunk;
			;

			m_chunks.insert(std::make_pair(glm::vec2(x, y), Chunk()));
		}
}

World::~World() {
	for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
		it->second.cleanUp();
}

void World::update(int dt, glm::vec3 camPos) {
	glm::vec2 camPosCS = glm::vec2(std::floor(camPos.x / Definitions::CHUNK_SIZE),
		std::floor(camPos.z / Definitions::CHUNK_SIZE));

	if (!m_chunks.count(camPosCS))
		m_chunks.insert(std::make_pair(camPosCS, Chunk()));

	for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
		it->second.update();
}

Block World::getBlock(int x, int y, int z) const {
	return m_chunks.at(glm::vec2(x / Definitions::CHUNK_SIZE, z / Definitions::CHUNK_SIZE)).
		getBlock(x % Definitions::CHUNK_SIZE, y, z % Definitions::CHUNK_SIZE);
}

void World::setBlock(int x, int y, int z, Block block) {
	m_chunks[glm::vec2(x / Definitions::CHUNK_SIZE, z / Definitions::CHUNK_SIZE)].
		setBlock(x % Definitions::CHUNK_SIZE, y, z % Definitions::CHUNK_SIZE, block);
}

int World::getTerrainHeight(int x, int y) {
	int height = 0;
	while (getBlock(x, height, y).type != BlockType::BLOCK_AIR) height++;
	return height;
}
