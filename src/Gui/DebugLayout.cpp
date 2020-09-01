#include "../../include/Gui/GUI.h"
#include "../../include/Gui/DebugLayout.h"

#include "../../include/Gui/Text.h"
#include "../../include/Resources/ResourceManager.h"

#include <sstream>
#include <iomanip>

std::string DebugLayout::toStr(float value, int precision)
{
    std::stringstream ss;
    ss << round(value * pow(10, precision)) / pow(10, precision);
    return ss.str();
}

DebugLayout::DebugLayout(GUI &gui)
    : Layout {"layout_debugpanel", gui},
    m_textFps {new Text("text_fps", 0.5f)},
    m_textRenderTime {new Text("text_renderTime", 0.5f)},
    m_textChunkCount {new Text("text_chunkCount", 0.5f)},
    m_textCameraVectors {new Text("text_cameraVectors", 0.5f)},
    m_textCameraScalars {new Text("text_cameraScalars", 0.5f)},
    m_textChunkPosition {new Text("text_cameraScalars", 0.5f)}
{
    m_properties.constraints.height = RelativeConstraint(1);
    setStackMode(STACK_VERTICAL, true, false);

    m_textFps->properties().constraints.x = AbsoluteConstraint(0);
    m_textFps->properties().padding.top = 10;
    m_textFps->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textFps);

    m_textRenderTime->properties().constraints.x = AbsoluteConstraint(0);
    m_textRenderTime->properties().padding.top = 10;
    m_textRenderTime->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textRenderTime);

    m_textChunkCount->properties().constraints.x = AbsoluteConstraint(0);
    m_textChunkCount->properties().padding.top = 10;
    m_textChunkCount->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textChunkCount);

    m_textCameraVectors->properties().constraints.x = AbsoluteConstraint(0);
    m_textCameraVectors->properties().padding.top = 10;
    m_textCameraVectors->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textCameraVectors);

    m_textCameraScalars->properties().constraints.x = AbsoluteConstraint(0);
    m_textCameraScalars->properties().padding.top = 10;
    m_textCameraScalars->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textCameraScalars);

    m_textChunkPosition->properties().constraints.x = AbsoluteConstraint(0);
    m_textChunkPosition->properties().padding.top = 10;
    m_textChunkPosition->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textChunkPosition);
}

void DebugLayout::setValues(const DebugLayoutInfo &info) {
    m_textFps->setString(toStr(info.fps) + " FPS");
    m_textRenderTime->setString(toStr(info.rendertime) + "us avg. frametime");
    m_textChunkCount->setString(toStr(info.chunkCount) + " active chunks");
    m_textCameraVectors->setString("Position, Facing: [" + toStr(info.playerPosition.x) + ", " + toStr(info.playerPosition.y) + ", " + toStr(info.playerPosition.z) + "], [ " +
                                   toStr(info.playerFront.x) + ", " + toStr(info.playerFront.y) + ", " + toStr(info.playerFront.z) + "]");
    m_textCameraScalars->setString("Yaw, Pitch, FOV: [" + toStr(info.yaw) + ", " + toStr(info.pitch) + ", " + toStr(info.fov) + "]");
    m_textChunkPosition->setString("Chunk: [" + toStr(info.chunkX) + ", " + toStr(info.chunkZ) + "]");
}
