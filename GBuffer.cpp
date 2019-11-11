#include "pch.h"

#include "GBuffer.h"

void GBuffer::init() {
	void resize(int width, int height);
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	// position color buffer
	glGenTextures(1, &m_gPos);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPos, 0);

	// normal color buffer
	glGenTextures(1, &m_gNorm);
	glBindTexture(GL_TEXTURE_2D, m_gNorm);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNorm, 0);

	// color + specular color buffer
	glGenTextures(1, &m_gAlbedo);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedo, 0);

	// tell color attachments used for rendering
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// depth buffer
	glGenRenderbuffers(1, &m_rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Framebuffer not complete.");
}

void GBuffer::resize(int width, int height) {
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, m_gNorm);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

void GBuffer::bindFramebuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
}

void GBuffer::bindTextures() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPos);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNorm);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedo);
}
