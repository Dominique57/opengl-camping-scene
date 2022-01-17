// Created by dominique on 1/17/22.

#pragma once

#include <wrappers/glWrapper.hh>
#include <wrappers/glmWrapper.hh>
#include <random>


class SsaoBuf {
public:
    SsaoBuf(int screen_w, int screen_h);

    GLuint getNoiseTexId() const;

    GLuint getFboId() const;

    GLuint getColorTexId() const;

    GLuint getBlurFboId() const;

    GLuint getBlurColorTexId() const;

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

