#include "../../include/Systems/ItemSystem.hpp"

#include "../../include/Components/CollisionComponent.hpp"
#include "../../include/Components/InventoryComponent.hpp"
#include "../../include/Components/ItemComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/RigidBodyComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"

#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/BlockTemplate.hpp"
#include "../../include/Rendering/Vertex.hpp"
#include "../../include/Resources/ResourceManager.hpp"

void ItemSystem::handleBlockChanged(const BlockChangedEvent& e) {
	if (e.prevBlock != BlockId::BLOCK_AIR) {
		spawnItem(e.position, e.prevBlock);
	}
}

void ItemSystem::handlePlayerMoved(const EntityMovedEvent& e) {
	const CollisionComponent& playerCollisionComponent = m_registry.get<CollisionComponent>(e.entity);
	const TransformationComponent& playerTransformation = m_registry.get<TransformationComponent>(e.entity);
	Math::Cuboid playerCollision = playerCollisionComponent.transform(playerTransformation);

	const auto& view = m_registry.view<ItemComponent, TransformationComponent, CollisionComponent>();
	for (const auto& entity : view) {
		const CollisionComponent& itemCollisionComponent = m_registry.get<CollisionComponent>(entity);
		const TransformationComponent& itemTransform = m_registry.get<TransformationComponent>(entity);
		const ItemComponent& item = m_registry.get<ItemComponent>(entity);

		Math::Cuboid itemCollision = itemCollisionComponent.transform(itemTransform);

		if (playerCollision.intersects(itemCollision)) {
			InventoryComponent& inventory = m_registry.get<InventoryComponent>(e.entity);
			int slot = Utility::getInventorySlot(inventory, item.blockId);

			if (slot == -1) {
				return;
			}

			if (inventory.slots[slot].first == BlockId::BLOCK_AIR) {
				inventory.slots[slot].first = item.blockId;
			}

			inventory.slots[slot].second++;
			m_invalidItems.push(entity);
		}
	}
}

void ItemSystem::_update(int dt) {
	while (!m_invalidItems.empty()) {
		const entt::entity& entity = m_invalidItems.front();

		destroyItem(entity);
		m_invalidItems.pop();
	}
}

void ItemSystem::spawnItem(const glm::vec3& position, const BlockId& block) {
	std::cout << "create item at: " << position << std::endl;
	entt::entity entity = m_registry.create();

	std::vector<Vertex> vertices = std::vector<Vertex>{};
	std::vector<unsigned int> indices = std::vector<unsigned int>{};
	BlockUVs blockUVs = m_atlas[block];

	createItemGeometry(vertices, indices, blockUVs);

	glm::vec3 offset = glm::vec3{ rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX };

	ItemComponent& item = m_registry.emplace<ItemComponent>(entity, block, new Geometry{});
	m_registry.emplace<TransformationComponent>(entity, position + offset, glm::vec3{ 0.f }, glm::vec3{ 1.0f });
	VelocityComponent& velocity = m_registry.emplace<VelocityComponent>(entity, glm::vec3{ 0 }, glm::vec3{ 0.0f, 0.01f, 0.0f });
	m_registry.emplace<MeshRenderComponent>(entity, ResourceManager::getResource<Material>(MATERIAL_CHUNK_BLOCKS_CULLED), item.geometry);
	m_registry.emplace<RigidBodyComponent>(entity, 0.1f, false);
	m_registry.emplace<CollisionComponent>(entity, glm::vec3{ -0.1f, -0.5f, -0.1f }, 0.2f, 0.2f, 0.6f);

	item.geometry->fillBuffers(vertices, indices);
}

void ItemSystem::destroyItem(const entt::entity& entity) {
	ItemComponent& item = m_registry.get<ItemComponent>(entity);
	std::cout << "picked up item! blockId: " << (size_t)item.blockId << std::endl;

	delete item.geometry;

	m_registry.destroy(entity);
}

void ItemSystem::createItemGeometry(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const BlockUVs& uvs) {
	// TODO put all of this data into an external file and use scale matrices with default block geometry
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, 0.1f, -0.1f}, glm::vec3{-1, 0, 0}, uvs[4][0] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, -0.1f, 0.1f}, glm::vec3{-1, 0, 0}, uvs[4][1] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, 0.1f, 0.1f}, glm::vec3{-1, 0, 0}, uvs[4][2] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, -0.1f, -0.1f}, glm::vec3{-1, 0, 0}, uvs[4][3] });

	vertices.emplace_back(Vertex{ glm::vec3{0.1f, 0.1f, 0.1f}, glm::vec3{1, 0, 0}, uvs[2][0] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, -0.1f, -0.1f}, glm::vec3{1, 0, 0}, uvs[2][1] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, 0.1f, -0.1f}, glm::vec3{1, 0, 0}, uvs[2][2] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, -0.1f, 0.1f}, glm::vec3{1, 0, 0}, uvs[2][3] });

	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, -0.1f, 0.1f}, glm::vec3{0, -1, 0}, uvs[5][0] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, -0.1f, -0.1f}, glm::vec3{0, -1, 0}, uvs[5][1] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, -0.1f, 0.1f}, glm::vec3{0, -1, 0}, uvs[5][2] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, -0.1f, -0.1f}, glm::vec3{0, -1, 0}, uvs[5][3] });

	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, 0.1f, -0.1f}, glm::vec3{0, -1, 0}, uvs[0][0] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, 0.1f, 0.1f}, glm::vec3{0, -1, 0}, uvs[0][1] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, 0.1f, -0.1f}, glm::vec3{0, -1, 0}, uvs[0][2] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, 0.1f, 0.1f}, glm::vec3{0, -1, 0}, uvs[0][3] });

	vertices.emplace_back(Vertex{ glm::vec3{0.1f, 0.1f, -0.1f}, glm::vec3{0, 0, -1}, uvs[3][0] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, -0.1f, -0.1f}, glm::vec3{0, 0, -1}, uvs[3][1] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, 0.1f, -0.1f}, glm::vec3{0, 0, -1}, uvs[3][2] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, -0.1f, -0.1f}, glm::vec3{0, 0, -1}, uvs[3][3] });

	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, 0.1f, 0.1f}, glm::vec3{0, 0, -1}, uvs[1][0] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, -0.1f, 0.1f}, glm::vec3{0, 0, -1}, uvs[1][1] });
	vertices.emplace_back(Vertex{ glm::vec3{0.1f, 0.1f, 0.1f}, glm::vec3{0, 0, -1}, uvs[1][2] });
	vertices.emplace_back(Vertex{ glm::vec3{-0.1f, -0.1f, 0.1f}, glm::vec3{0, 0, -1}, uvs[1][3] });

	// add indices reserve some space to prevent reallocations
	for (int i = 0; i < 6; i++) {
		constexpr int offsets[] = { 0, 1, 2, 0, 3, 1 };

		for (int j = 0; j < 6; j++) {
			indices.emplace_back(i * 4 + offsets[j]);
		}
	}
}

ItemSystem::ItemSystem(entt::registry& registry, const TextureAtlas& atlas)
	: System{ registry, 0 }, m_atlas{ atlas } {
	m_blockChangedHandle = EventDispatcher::onBlockChange.subscribe([&](const BlockChangedEvent& e) {
		handleBlockChanged(e);
		});

	m_playerMovedHandle = EventDispatcher::onEntityMoved.subscribe([&](const EntityMovedEvent& e) {
		if (m_registry.any<PlayerComponent>(e.entity)) {
			handlePlayerMoved(e);
		}
		});
}
