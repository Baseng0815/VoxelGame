#pragma once

#include "Layout.h"

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
};

class DebugLayout : public Layout {
    private:
        Text *m_textFps, *m_textRenderTime, *m_textChunkCount,
             *m_textCameraVectors, *m_textCameraScalars, *m_textChunkPosition;

        static std::string toStr(float value, int precision = 2);

    public:
        DebugLayout(GUI &gui);

        void setValues(const DebugLayoutInfo &info);
};
