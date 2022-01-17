// Created by dominique on 1/17/22.

#include "ssaoBuf.hh"

SsaoBuf::SsaoBuf(int screen_w, int screen_h)
    : fboId(0), colorTexId(0), blurFboId(0), blurColorTexId(0), noiseTexId(0), kernel{}
{
    // Create FBO
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    // Create fbo grayscale texture output
    glGenTextures(1, &colorTexId);
    glBindTexture(GL_TEXTURE_2D, colorTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screen_w, screen_h, 0,
                 GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexId, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::invalid_argument("SsaoBuffer::fbo: FrameBuffer not complete !");

    // Create Blur Fbo
    glGenFramebuffers(1, &blurFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFboId);
    // Create blur fbo grayscale texture output
    glGenTextures(1, &blurColorTexId);
    glBindTexture(GL_TEXTURE_2D, blurColorTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screen_w, screen_h, 0,
                 GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurColorTexId, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::invalid_argument("SsaoBuffer::blurFbo: FrameBuffer not complete !");

    // Reset current framebuffer pointer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    createNoiseTexture();
    createRandomKernel();
}

void SsaoBuf::createNoiseTexture() {
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
    std::default_random_engine generator;

    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }

    glGenTextures(1, &noiseTexId);
    glBindTexture(GL_TEXTURE_2D, noiseTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0,
                 GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void SsaoBuf::createRandomKernel() {
    kernel.clear();

    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;

    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = glm::lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        kernel.push_back(sample);
    }
}

GLuint SsaoBuf::getNoiseTexId() const {
    return noiseTexId;
}

GLuint SsaoBuf::getFboId() const {
    return fboId;
}

GLuint SsaoBuf::getColorTexId() const {
    return colorTexId;
}

GLuint SsaoBuf::getBlurFboId() const {
    return blurFboId;
}

GLuint SsaoBuf::getBlurColorTexId() const {
    return blurColorTexId;
}