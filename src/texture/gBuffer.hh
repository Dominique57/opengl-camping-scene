// Created by dominique on 1/16/22.

#pragma once

#include <wrappers/glWrapper.hh>
#include <wrappers/glmWrapper.hh>
#include <temp/program.hh>

class GBuffer {

public:
    GBuffer(int screen_w, int screen_h);

    void use();
    void unuse();

public:
    GLuint getPositionTexId() const;

    GLuint getNormalTexId() const;

    GLuint getAlbedoTexId() const;

    GLuint getFboId() const;

protected:
    GLuint fboId;

    GLuint positionTexId;

    GLuint normalTexId;
    GLuint albedoTexId;

    GLuint rboId;
};

