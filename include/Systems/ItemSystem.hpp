#include "System.hpp"

#include "../GameData/BlockIds.hpp"
#include "../Resources/TextureAtlas.hpp"

#include <glm/glm.hpp>
#include <queue>

struct BlockChangedEvent;
struct EntityMovedEvent;
struct Vertex;

class ItemSystem : public System {
  private:
    const TextureAtlas &m_atlas;

    CallbackHandle<const BlockChangedEvent &> m_blockChangedHandle;
    void handleBlockChanged(const BlockChangedEvent &e);
    CallbackHandle<const EntityMovedEvent &> m_playerMovedHandle;
    void handlePlayerMoved(const EntityMovedEvent &e);

    std::queue<entt::entity> m_invalidItems;

    void _update(int dt) override;

    void spawnItem(const glm::vec3 &position, const BlockId &block);
    void destroyItem(const entt::entity &entitiy);

    static void createItemGeometry(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, const BlockUVs &uvs);

  public:
    ItemSystem(entt::registry &registry, const TextureAtlas &atlas);
};