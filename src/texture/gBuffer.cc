// Created by dominique on 1/16/22.

#include "gBuffer.hh"


GBuffer::GBuffer(int screen_w, int screen_h)
    : fboId(0), positionTexId(0), normalTexId(0), albedoTexId(0), rboId(0)
{
    // Create FBO
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    // Create textures
    glGenTextures(1, &positionTexId);
    glBindTexture(GL_TEXTURE_2D, positionTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_w, screen_h, 0,
                 GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, positionTexId, 0);

    glGenTextures(1, &normalTexId);
    glBindTexture(GL_TEXTURE_2D, normalTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screen_w, screen_h, 0,
                 GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexId, 0);

    glGenTextures(1, &albedoTexId);
    glBindTexture(GL_TEXTURE_2D, albedoTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_w, screen_h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoTexId, 0);

    // Set FBO's output textures: position, normal color in resp. 0, 1, 2 location
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    // Set RBO(depth) in FBO
    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_w, screen_h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);

    // Check fbo completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::invalid_argument("GBuffer: FrameBuffer not complete !");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GBuffer::getFboId() const {
    return fboId;
}

void GBuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
}

void GBuffer::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GBuffer::getPositionTexId() const {
    return positionTexId;
}

GLuint GBuffer::getNormalTexId() const {
    return normalTexId;
}

GLuint GBuffer::getAlbedoTexId() const {
    return albedoTexId;
}
