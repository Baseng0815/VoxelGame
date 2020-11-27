#pragma once
#include "System.hpp"

struct PlayerComponent;
struct VelocityComponent;
struct CameraComponent;
struct EntityMovedEvent;
struct RigidBodyComponent;

// handles actual player movement
// -> velocity calculation, NOT position!! (this is done by physics)
class PlayerMovementSystem : public System {
private:
    entt::entity playerEntity;

    CallbackHandle<const EntityMovedEvent &> m_playerMovedHandle;
    void handlePlayerMoved(const EntityMovedEvent &e) const;

    void _update(int dt) override;

    void updatePlayerSpeed(PlayerComponent& player, VelocityComponent& velocity, const CameraComponent& camera, const RigidBodyComponent& rigidBody) const;
public:
    PlayerMovementSystem(Registry_T& registry);
};
