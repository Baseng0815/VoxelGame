#pragma once

#include "System.hpp"
#include "../Resources/Geometry.hpp"

class Shader;

struct KeyEvent;
struct EnterChunkEvent;
struct CursorEvent;
struct EntityMovedEvent;

// renders various debug info when in debug mode (F3) independently from other rendering systems
class DebugRenderSystem : public System {
    private:
        // TODO maybe don't make independent and use a mesh renderer?
        // if so, somehow remove the entity temporarily or add an active switch to the mesh renderer?
        const Shader *m_mvpColorShader;
        entt::entity m_cameraEntity;

        // chunk boundaries
        Geometry m_chunkBoundaries;
        glm::mat4 m_chunkModelMatrix {1.f};

        // coordinate system
        Geometry m_coordinateSystem;
        glm::mat4 m_coordinateSystemModelMatrix {1.f};
        void recalculateCoordinateSystemModelMatrix();

        CallbackHandle<const KeyEvent&> m_keyEventHandle;
        void handleKeys(const KeyEvent&);
        CallbackHandle<const EnterChunkEvent&> m_enterChunkHandle;
        void handleEnterChunk(const EnterChunkEvent&);
        CallbackHandle<const CursorEvent&> m_cursorHandle;
        CallbackHandle<const EntityMovedEvent&> m_entityMovedHandle;

        bool m_isEnabled {false};

        void _update(int dt) override;

    public:
        DebugRenderSystem(Registry_T &registry);
};
