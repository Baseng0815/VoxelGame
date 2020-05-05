#include "../include/CollisionSystem.h"
#include "../include/Components/ChunkComponent.h"
#include "../include/Components/CollisionComponent.h"
#include "../include/Components/TransformationComponent.h"
#include "../include/SystemManager.h"
#include "../include/Block.h"

CollisionSystem::CollisionSystem(SystemManager* systemManager, SharedContext* context)
	: System(systemManager, context) {
}

void CollisionSystem::init() {

}

void CollisionSystem::update(int dt) {
	auto chunksView = m_systemManager->getRegistry().view<ChunkComponent>();

	auto view = m_systemManager->getRegistry().view<TransformationComponent, CollisionComponent>();
	for (auto entity : chunksView) {
		ChunkComponent& chunk = chunksView.get(entity);

		for (auto entity : view) {
			auto& transform = view.get<TransformationComponent>(entity);
			int chunkX = transform.position.x / Definitions::CHUNK_SIZE;
			int chunkZ = transform.position.z / Definitions::CHUNK_SIZE;

			if (chunk.chunkX == chunkX && chunk.chunkZ == chunkZ) {
				Block block = chunk.blocks[(int)transform.position.x % Definitions::CHUNK_SIZE][(int)transform.position.y][(int)transform.position.z % Definitions::CHUNK_SIZE];

				if (block.type != BlockType::BLOCK_AIR) {
					transform.position.y++;
				}
			}
		}
	}
}

