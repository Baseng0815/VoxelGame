/*
#include "../../include/Systems/LightRenderSystem.h"

#include "../../include/Event.h"

void LightRenderSystem::handleFramebufferSize(Event* e) {
    FramebufferSizeEvent fbsE = *e->get<FramebufferSizeEvent>();
    glViewport(0, 0, fbsE.width, fbsE.height);
    m_gBuffer.resize(fbsE.width, fbsE.height);
}

LightRenderSystem::LightRenderSystem(SystemManager* systemManager)
    : System(systemManager, context) {
        m_light.color = glm::vec3(1, 1, 1);
        m_light.position = glm::vec3(10, 10, 10);
    }

void LightRenderSystem::update(int dt) {

}
*/
