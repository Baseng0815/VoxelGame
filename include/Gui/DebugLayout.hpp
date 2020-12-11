#pragma once

#include "Layout.hpp"

class GUI;

struct DebugLayoutInfo {
    int fps;
    int rendertime;
    int chunkCount;
    const glm::vec3 &playerPosition;
    const glm::vec3 &playerFront;
    float yaw;
    float pitch;
    float fov;
    int chunkX, chunkZ;
    bool inAir;
    bool isFlying;
    bool gravityApplies;
};

class DebugLayout : public Layout {
    private:
        Text *m_textFps, *m_textRenderTime, *m_textChunkCount,
             *m_textCameraVectors, *m_textCameraScalars, *m_textChunkPosition,
             *m_textInAir, *m_textIsFlying, *m_textGravityApplies;

        static std::string ftoStr(float value, int precision = 2);
        static std::string btoStr(bool value);

    public:
        DebugLayout(GUI &gui);

        void setValues(const DebugLayoutInfo &info);
};
