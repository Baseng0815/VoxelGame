#pragma once
#include "System.h"

struct CameraComponent;
struct CollisionComponent;
struct PlayerComponent;
struct TransformationComponent;
struct CursorEvent;

class CollisionSystem : public System {
private:
    CallbackHandle<const CursorEvent&> m_cursorMoveHandle;

    void _update(int dt) override;

    void updatePlayerLookAtBlock(PlayerComponent& player, TransformationComponent& transform, CameraComponent& camera) const;
    void checkCollisions(entt::entity first, entt::entity secnd);
public:
    CollisionSystem(Registry_T& registry);
};