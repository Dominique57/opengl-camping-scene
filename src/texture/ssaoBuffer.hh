// Created by dominique on 1/17/22.

#pragma once

#include <wrappers/glWrapper.hh>
#include <wrappers/glmWrapper.hh>
#include <random>


class SsaoBuffer {
public:
    SsaoBuffer(int screen_w, int screen_h);

    GLuint getNoiseTexId() const;

    GLuint getFboId() const;

    GLuint getColorTexId() const;

    GLuint getBlurFboId() const;

    GLuint getBlurColorTexId() const;

    const std::vector<glm::vec3> &getKernel() const;

private:
    void createNoiseTexture();

    void createRandomKernel();

protected:
    GLuint fboId;
    GLuint colorTexId;

    GLuint blurFboId;
    GLuint blurColorTexId;

    GLuint noiseTexId;

    std::vector<glm::vec3> kernel;
};

