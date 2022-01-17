// Created by dominique on 1/17/22.

#include "dopBuffer.hh"

DopBuffer::DopBuffer(int screen_w, int screen_h)
    : fboId(0), colorTexId(0)
{
    // Create FBO
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    glGenTextures(1, &colorTexId);
    glBindTexture(GL_TEXTURE_2D, colorTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_w, screen_h, 0,
                 GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexId, 0);

    // Check fbo completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::invalid_argument("GBuffer: FrameBuffer not complete !");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint DopBuffer::getFboId() const {
    return fboId;
}

GLuint DopBuffer::getColorTexId() const {
    return colorTexId;
}