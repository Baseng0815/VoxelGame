#include "../../include/Rendering/WaterRenderbuffers.hpp"

#include <iostream>

WaterRenderbuffers::WaterRenderbuffers(int width, int height) {
    create(width, height);
}

WaterRenderbuffers::~WaterRenderbuffers() {
    destroy();
}

void WaterRenderbuffers::create(int width, int height) {
    glGenFramebuffers(1, &m_refractionBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_refractionBuffer);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &m_refractionTexture);
    glBindTexture(GL_TEXTURE_2D, m_refractionTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_refractionTexture, 0);

    glGenRenderbuffers(1, &m_refractionDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_refractionDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_refractionDepthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "refraction buffer incomplete!" << std::endl;
    }

    glGenFramebuffers(1, &m_reflectionBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_reflectionBuffer);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &m_reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, m_reflectionTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_reflectionTexture, 0);

    glGenRenderbuffers(1, &m_reflectionDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_reflectionDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_reflectionDepthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "reflection buffer incomplete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WaterRenderbuffers::destroy() {
    glDeleteTextures(1, &m_reflectionTexture);
    glDeleteTextures(1, &m_refractionTexture);
    glDeleteFramebuffers(1, &m_reflectionBuffer);
    glDeleteFramebuffers(1, &m_refractionBuffer);
}

void WaterRenderbuffers::bind(const RenderbufferId &renderBuffer) const {
    if (renderBuffer == RenderbufferId::RENDERBUFFER_REFLECTION) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_reflectionBuffer);
    }
    else if (renderBuffer == RenderbufferId::RENDERBUFFER_REFRACTION) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_refractionBuffer);
    }
    else { // default
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void WaterRenderbuffers::bindTextures(unsigned int offset) const {
    glActiveTexture(GL_TEXTURE0 + offset);
    glBindTexture(GL_TEXTURE_2D, m_refractionTexture);

    glActiveTexture(GL_TEXTURE1 + offset);
    glBindTexture(GL_TEXTURE_2D, m_reflectionTexture);
}

void WaterRenderbuffers::clear() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_reflectionBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_refractionBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterRenderbuffers::resize(int width, int height) {
    destroy();
    create(width, height);
}
