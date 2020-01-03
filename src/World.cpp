#include "../include/Block.h"
#include "../include/World.h"
#include "../include/EventDispatcher.h"

#include <iostream>

void World::handleEnterChunk(Event* e) {
    EnterChunkEvent event = *e->get<EnterChunkEvent>();
    int dx = event.newX - event.oldX;
    int dy = event.newY - event.oldY;

    // remove chunks which have gone out of range
    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
        if (std::abs(event.newX - it->first.x) > Definitions::CHUNK_PRELOAD_SIZE ||
            std::abs(event.newY - it->first.y) > Definitions::CHUNK_PRELOAD_SIZE)
            it->second.queueDestruction();
    }

    // insert new chunks which have come into range
    glm::vec2 pos;
    for (int x = event.newX - Definitions::CHUNK_PRELOAD_SIZE; x <= event.newX + (int)Definitions::CHUNK_PRELOAD_SIZE; x++) {
        for (int y = event.newY - Definitions::CHUNK_PRELOAD_SIZE; y <= event.newY + (int)Definitions::CHUNK_PRELOAD_SIZE; y++) {
            pos.x = x; pos.y = y;
            if (!m_chunks.count(pos))
                m_chunks.insert(std::make_pair(pos, Chunk(pos)));
        }
    }
}

void World::init(WorldType worldType) {
    m_generator.init(worldType);
    Chunk::setWorldGenerator(&m_generator);

    EventDispatcher::addCallback(std::bind(&World::handleEnterChunk, this, _1), ENTER_CHUNK_EVENT);
}

World::~World() {
    for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it) {
        while (!it->second.destructionReady()) {}
        it->second.cleanUp();
    }
}

void World::update(int dt) {
    auto it = m_chunks.begin();
    while (it != m_chunks.end()) {
        if (it->second.destructionReady()) {
            it->second.cleanUp();
            it = m_chunks.erase(it);
        } else {
            it->second.update();
            ++it;
        }
    }
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

int World::chunkCount() const {
	return m_chunks.size();
}
