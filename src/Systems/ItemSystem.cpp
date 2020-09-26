#include "../../include/Systems/ItemSystem.hpp"

#include "../../include/Components/ItemComponent.hpp"
#include "../../include/Components/MeshRenderComponent.hpp"
#include "../../include/Components/PlayerComponent.hpp"
#include "../../include/Components/TransformationComponent.hpp"
#include "../../include/Components/VelocityComponent.hpp"

#include "../../include/Events/EventDispatcher.hpp"
#include "../../include/GameData/BlockTemplate.hpp"
#include "../../include/Resources/ResourceManager.hpp"

void ItemSystem::handleBlockChanged(const BlockChangedEvent& e) {
    if (e.prevBlock != BlockId::BLOCK_AIR) {
        spawnItem(e.position, e.prevBlock);
    }
}

void ItemSystem::handlePlayerMoved(const EntityMovedEvent& e) {
    glm::vec3 position = e.newPos;

    const auto& view = m_registry.view<ItemComponent, TransformationComponent>();
    for (const auto& entity : view) {
        const TransformationComponent& itemTransform = m_registry.get<TransformationComponent>(entity);
        const glm::vec3& itemPosition = itemTransform.getPosition();
        if (glm::length(itemPosition - position) < .75f) {
            m_invalidItems.push(entity);
        } } }

void ItemSystem::_update(int dt) {
    while (!m_invalidItems.empty()) {
        const entt::entity& entity = m_invalidItems.front();

        destroyItem(entity);
        m_invalidItems.pop();
    }
}

void ItemSystem::spawnItem(const glm::vec3& position, const BlockId& block) {
    entt::entity entity = m_registry.create();

    m_registry.emplace<ItemComponent>(entity, block);
    m_registry.emplace<TransformationComponent>(entity, position, glm::vec3{0.f}, glm::vec3{1.0f});
    m_registry.emplace<VelocityComponent>(entity, glm::vec3{0}, glm::vec3{0.0f, 0.0f, 1.0f});

    // TODO: Create item geometry
    // MeshRenderComponent renderComponent = MeshRenderComponent{};
    // renderComponent.material = ResourceManager::getResource<Material>(MATERIAL_CHUNK_BLOCKS);

    // create geometry
    // std::vector<Vertex> vertices = std::vector<Vertex>{};
    // std::vector<unsigned int> indices = std::vector<unsigned int>{}

    // vertices.emplace_back(glm::vec3{0.125f, 0.125f, 0.125f}, glm::vec3{-1, 0, 0});
}

void ItemSystem::destroyItem(const entt::entity& entity) {
    m_registry.destroy(entity);
}

ItemSystem::ItemSystem(entt::registry& registry) : System{registry, 0} {
    m_blockChangedHandle =
        EventDispatcher::onBlockChange.subscribe([&](const BlockChangedEvent& e) { handleBlockChanged(e); });

    m_playerMovedHandle = EventDispatcher::onEntityMoved.subscribe([&](const EntityMovedEvent& e) {
        if (m_registry.any<PlayerComponent>(e.entity)) {
            handlePlayerMoved(e);
        }
    });
}
