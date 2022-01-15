// Created by dominique on 7/15/21.

#include "gpuParticleEmitter.hh"
#include <random>

GpuParticleEmitter::GpuParticleEmitter(const glm::vec3 &position, float rescaleFactor)
    : vaoId(0), vboData(0), emitterPos(position), rescaleFactor(rescaleFactor) {
    // Create and use vao
    glGenVertexArrays(1, &vaoId); TEST_OPENGL_ERROR()
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()

    // Create and use vbo
    glGenBuffers(1, &vboData); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, particleCount * sizeof(ParticleRender), nullptr,
                 GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR()

    // Enable options
    glEnable(GL_PROGRAM_POINT_SIZE); TEST_OPENGL_ERROR()

    // Disable everything
    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}

void GpuParticleEmitter::bind(const program &program) {
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    {
        GLint posLoc = glGetAttribLocSafe(program.get_program(), "vPosition");
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleRender),
                              (void*)offsetof(ParticleRender, position)); TEST_OPENGL_ERROR()
        glEnableVertexAttribArray(posLoc); TEST_OPENGL_ERROR()
    }
    {
        GLint posLoc = glGetAttribLocSafe(program.get_program(), "vColor");
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleRender),
                              (void*)offsetof(ParticleRender, color)); TEST_OPENGL_ERROR()
        glEnableVertexAttribArray(posLoc); TEST_OPENGL_ERROR()
    }
    {
        GLint posLoc = glGetAttribLocSafe(program.get_program(), "vSize");
        glVertexAttribPointer(posLoc, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleRender),
                              (void*)offsetof(ParticleRender, size)); TEST_OPENGL_ERROR()
        glEnableVertexAttribArray(posLoc); TEST_OPENGL_ERROR()
    }

    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}

void GpuParticleEmitter::init_particles() {
    auto points = std::vector<ParticleRender>{};
    points.reserve(particleCount);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> height(0, 5);
    std::normal_distribution<float> norm(0, 0.25);
    std::normal_distribution<float> normHigh(0, 0.75);

    for (auto i = 0U; i < particleCount; ++i) {
        float xrand = std::clamp(norm(gen), -0.5f, 0.5f);
        float yrand = height(gen);
        float zrand = std::clamp(norm(gen), -0.5f, 0.5f);
        float life = normHigh(gen);
        if (life < 0)
            life = -life;

        const ParticleRender particle{
            glm::vec3{xrand, yrand, zrand},
            1., {1, 0, 0}, life
        };
        points.push_back(particle);
    }

    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, particleCount * sizeof(ParticleRender), points.data(),
                 GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR()
    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}

void GpuParticleEmitter::update(const program& computeShader) {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 79, vboData);
    glDispatchCompute((particleCount + 255) / 256, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void GpuParticleEmitter::draw() {
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDrawArrays(GL_POINTS, 0, particleCount); TEST_OPENGL_ERROR()

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}