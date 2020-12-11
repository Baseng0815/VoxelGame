#include "../../include/Gui/GUI.hpp"
#include "../../include/Gui/DebugLayout.hpp"

#include "../../include/Gui/Text.hpp"
#include "../../include/Resources/ResourceManager.hpp"

#include <sstream>
#include <iomanip>

std::string DebugLayout::ftoStr(float value, int precision)
{
    std::stringstream ss;
    ss << round(value * pow(10, precision)) / pow(10, precision);
    return ss.str();
}

std::string DebugLayout::btoStr(bool value)
{
    if (value) {
        return "true";
    } else {
        return "false";
    }
}

DebugLayout::DebugLayout(GUI &gui)
    : Layout {"layout_debugpanel", gui},
    m_textFps {new Text("text_fps")},
    m_textRenderTime {new Text("text_renderTime")},
    m_textChunkCount {new Text("text_chunkCount")},
    m_textCameraVectors {new Text("text_cameraVectors")},
    m_textCameraScalars {new Text("text_cameraScalars")},
    m_textChunkPosition {new Text("text_cameraScalars")},
    m_textInAir {new Text("text_inAir")},
    m_textIsFlying {new Text("text_isFlying")},
    m_textGravityApplies {new Text("text_gravityApplies")}
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

    m_textInAir->properties().constraints.x = AbsoluteConstraint(0);
    m_textInAir->properties().margin.top = 10;
    m_textInAir->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textInAir);

    m_textIsFlying->properties().constraints.x = AbsoluteConstraint(0);
    m_textIsFlying->properties().margin.top = 10;
    m_textIsFlying->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textIsFlying);

    m_textGravityApplies->properties().constraints.x = AbsoluteConstraint(0);
    m_textGravityApplies->properties().margin.top = 10;
    m_textGravityApplies->setFont(ResourceManager::getResource<Font>(FONT_KORURI));
    this->addWidget(m_textGravityApplies);
}

void DebugLayout::setValues(const DebugLayoutInfo &info) {
    m_textFps->setString(ftoStr(info.fps) + " FPS");
    m_textRenderTime->setString(ftoStr(info.rendertime) + "us avg. frametime");
    m_textChunkCount->setString(ftoStr(info.chunkCount) + " active chunks");
    m_textCameraVectors->setString("Position: [" + ftoStr(info.playerPosition.x) + ", " + ftoStr(info.playerPosition.y) + ", " + ftoStr(info.playerPosition.z) + "], Facing [ " +
                                   ftoStr(info.playerFront.x) + ", " + ftoStr(info.playerFront.y) + ", " + ftoStr(info.playerFront.z) + "]");
    m_textCameraScalars->setString("Yaw: [" + ftoStr(info.yaw) + "], Pitch: [ " + ftoStr(info.pitch) + "], FOV [" + ftoStr(info.fov) + "]");
    m_textChunkPosition->setString("Chunk: [" + ftoStr(info.chunkX) + ", " + ftoStr(info.chunkZ) + "]");
    m_textInAir->setString("In air: [" + btoStr(info.inAir) + "]");
    m_textIsFlying->setString("Is flying: [" + btoStr(info.isFlying) + "]");
    m_textChunkPosition->setString("Gravity applies: [" + btoStr(info.gravityApplies) + "]");
}
