#pragma once

#include "System.h"
#include "../Resources/Geometry.h"

class Shader;
struct KeyEvent;
struct EnterChunkEvent;

// renders various debug info when in debug mode (F3) independently from other rendering systems
class DebugRenderSystem : public System {
    private:
        // TODO maybe don't make independent and use a mesh renderer?
        const Shader *m_mvpColorShader;

        // chunk boundaries
        Geometry m_chunkBoundaries;
        glm::mat4 m_chunkModelMatrix {1.f};

        CallbackHandle<const KeyEvent&> m_keyEventHandle;
        void handleKeys(const KeyEvent&);
        CallbackHandle<const EnterChunkEvent&> m_enterChunkHandle;
        void handleEnterChunk(const EnterChunkEvent&);

        bool m_isEnabled {false};

        void _update(int dt) override;

    public:
        DebugRenderSystem(Registry_T &registry);

        void toggle();
};
