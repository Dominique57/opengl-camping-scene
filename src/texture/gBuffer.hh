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

protected:
    GLuint fboId;

    GLuint positionTexId;
public:
    GLuint getPositionTexId() const;

    GLuint getNormalTexId() const;

    GLuint getAlbedoTexId() const;

protected:
    GLuint normalTexId;
    GLuint albedoTexId;

    GLuint rboId;
};

