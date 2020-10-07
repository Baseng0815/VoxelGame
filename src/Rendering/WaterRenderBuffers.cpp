#include "../../include/Rendering/WaterRenderBuffers.hpp"

#include "../../include/Resources/Shader.hpp"

#include <iostream>

WaterRenderbuffers::WaterRenderbuffers(int width, int height) {
    create(width, height);
}

WaterRenderbuffers::~WaterRenderbuffers() {
    destroy();
}

void WaterRenderbuffers::create(int width, int height) {
    glGenFramebuffers(1, &m_sceneBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_sceneBuffer);    

    glGenTextures(1, &m_sceneTexture);
    glBindTexture(GL_TEXTURE_2D, m_sceneTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_sceneTexture, 0);

    glGenRenderbuffers(1, &m_sceneDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_sceneDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    // glGenTextures(1, &m_sceneDepthTexture);
    // glBindTexture(GL_TEXTURE_2D, m_sceneDepthTexture);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT24, GL_UNSIGNED_BYTE, nullptr);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_sceneDepthTexture, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_sceneDepthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "refraction buffer incomplete!" << std::endl;
    }

    glGenFramebuffers(1, &m_waterBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_waterBuffer);    

    glGenTextures(1, &m_waterTexture);
    glBindTexture(GL_TEXTURE_2D, m_waterTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_waterTexture, 0);

    glGenRenderbuffers(1, &m_waterDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_waterDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    // glGenTextures(1, &m_waterDepthTexture);
    // glBindTexture(GL_TEXTURE_2D, m_waterDepthTexture);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT24, GL_UNSIGNED_BYTE, nullptr);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_waterDepthTexture, 0);    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_waterDepthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "reflection buffer incomplete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WaterRenderbuffers::destroy() {
    glDeleteTextures(1, &m_waterTexture);
    glDeleteTextures(1, &m_sceneTexture);
    glDeleteFramebuffers(1, &m_waterBuffer);
    glDeleteFramebuffers(1, &m_sceneBuffer);
}

void WaterRenderbuffers::bind(const RenderbufferId &renderBuffer) const {
    if (renderBuffer == RenderbufferId::RENDERBUFFER_WATER) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_waterBuffer);
    }
    else if (renderBuffer == RenderbufferId::RENDERBUFFER_SCENE) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_sceneBuffer);
    }
    else { // default
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void WaterRenderbuffers::bindTextures() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sceneTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_waterTexture);

    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, m_sceneDepthTexture);

    // glActiveTexture(GL_TEXTURE3);
    // glBindTexture(GL_TEXTURE_2D, m_waterDepthTexture);
}

void WaterRenderbuffers::clear() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_waterBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_sceneBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterRenderbuffers::resize(int width, int height) {
    destroy();
    create(width, height);
}
