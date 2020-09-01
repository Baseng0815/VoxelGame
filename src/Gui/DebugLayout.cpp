#include "../../include/Gui/GUI.h"
#include "../../include/Gui/DebugLayout.h"

#include "../../include/Gui/Text.h"
#include "../../include/Resources/ResourceManager.h"

DebugLayout::DebugLayout(GUI &gui)
    : Layout {"layout_debugpanel", gui},
    m_textFps {new Text("text_fps", 0.5f)},
    m_textRenderTime {new Text("text_renderTime", 0.5f)},
    m_textChunkCount {new Text("text_chunkCount", 0.5f)}
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
    /*
       t = this->addWidget<Text>("text_fps");
       t->getProperties().constraints.x = CenterConstraint();
       t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
       t->setString("3) Memory count");

       t = this->addWidget<Text>("text_fps");
       t->getProperties().constraints.x = CenterConstraint();
       t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
       t->setString("4) Number of allocations/more memory stats");

       t = this->addWidget<Text>("text_fps");
       t->getProperties().constraints.x = CenterConstraint();
       t->setFont(ResourceManager::getResource<Font>("fontKoruri"));
       t->setString("5) Camera position and direction");
       */
}

void DebugLayout::setValues(int fps, int rendertime, int chunkCount) {
    m_textFps->setString(std::to_string(fps) + " FPS");
    m_textRenderTime->setString(std::to_string(rendertime) + "us avg. frametime");
    m_textChunkCount->setString(std::to_string(chunkCount) + " active chunks");
}
