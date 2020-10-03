#include "../../include/Gui/GUI.hpp"
#include "../../include/Gui/DebugLayout.hpp"

#include "../../include/Gui/Text.hpp"
#include "../../include/Resources/ResourceManager.hpp"

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
    m_textFps {new Text("text_fps")},
    m_textRenderTime {new Text("text_renderTime")},
    m_textChunkCount {new Text("text_chunkCount")},
    m_textCameraVectors {new Text("text_cameraVectors")},
    m_textCameraScalars {new Text("text_cameraScalars")},
    m_textChunkPosition {new Text("text_cameraScalars")}
{
    m_properties.constraints.height = RelativeConstraint(1.0f);
    m_properties.constraints.width = RelativeConstraint(0.5f);
    m_properties.isVisible = false;
    setStackMode(STACK_VERTICAL);

    m_textFps->properties().constraints.x = AbsoluteConstraint(0);
    m_textFps->properties().margin.top = 10;
    m_textFps->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textFps);

    m_textRenderTime->properties().constraints.x = AbsoluteConstraint(0);
    m_textRenderTime->properties().margin.top = 10;
    m_textRenderTime->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textRenderTime);

    m_textChunkCount->properties().constraints.x = AbsoluteConstraint(0);
    m_textChunkCount->properties().margin.top = 10;
    m_textChunkCount->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textChunkCount);

    m_textCameraVectors->properties().constraints.x = AbsoluteConstraint(0);
    m_textCameraVectors->properties().margin.top = 10;
    m_textCameraVectors->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textCameraVectors);

    m_textCameraScalars->properties().constraints.x = AbsoluteConstraint(0);
    m_textCameraScalars->properties().margin.top = 10;
    m_textCameraScalars->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textCameraScalars);

    m_textChunkPosition->properties().constraints.x = AbsoluteConstraint(0);
    m_textChunkPosition->properties().margin.top = 10;
    m_textChunkPosition->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textChunkPosition);
}

void DebugLayout::setValues(const DebugLayoutInfo &info) {
    m_textFps->setString(toStr(info.fps) + " FPS");
    m_textRenderTime->setString(toStr(info.rendertime) + "us avg. frametime");
    m_textChunkCount->setString(toStr(info.chunkCount) + " active chunks");
    m_textCameraVectors->setString("Position: [" + toStr(info.playerPosition.x) + ", " + toStr(info.playerPosition.y) + ", " + toStr(info.playerPosition.z) + "], Facing [ " +
                                   toStr(info.playerFront.x) + ", " + toStr(info.playerFront.y) + ", " + toStr(info.playerFront.z) + "]");
    m_textCameraScalars->setString("Yaw: [" + toStr(info.yaw) + "], Pitch: [ " + toStr(info.pitch) + "], FOV [" + toStr(info.fov) + "]");
    m_textChunkPosition->setString("Chunk: [" + toStr(info.chunkX) + ", " + toStr(info.chunkZ) + "]");
}
