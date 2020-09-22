#include "System.h"

#include "../GameData/BlockIds.h"
#include <glm/glm.hpp>
#include <queue>

struct BlockChangedEvent;
struct EntityMovedEvent;

class ItemSystem : public System {
  private:
    CallbackHandle<const BlockChangedEvent&> m_blockChangedHandle;
    void handleBlockChanged(const BlockChangedEvent& e);
    CallbackHandle<const EntityMovedEvent&> m_playerMovedHandle;
    void handlePlayerMoved(const EntityMovedEvent& e);

    std::queue<entt::entity> m_invalidItems;

    void _update(int dt) override;

    void spawnItem(const glm::vec3& position, const BlockId& block);
    void destroyItem(const entt::entity& entitiy);

  public:
    ItemSystem(entt::registry& registry);
};