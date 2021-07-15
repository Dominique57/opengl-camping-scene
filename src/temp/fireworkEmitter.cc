// Created by dominique on 7/15/21.

#include "fireWorkEmitter.hh"
#include <random>

FireworkEmitter::FireworkEmitter(const glm::vec3 &position, const glm::vec3 &color,
                                 float rescaleFactor)
    : vaoId(0), emitterPos(position), particleColor(color), rescaleFactor(rescaleFactor) {
    glGenVertexArrays(1, &vaoId); TEST_OPENGL_ERROR()
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()

    glGenBuffers(1, &vboData); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, 2000 * sizeof(ParticleRender), nullptr,
                 GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR()

    glEnable(GL_PROGRAM_POINT_SIZE); TEST_OPENGL_ERROR()

    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()

}

void FireworkEmitter::bind(const program &program) {
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    {
        GLint posLoc = glGetAttribLocSafe(program.get_program(), "vPosition");
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleRender),
                              nullptr); TEST_OPENGL_ERROR()
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

    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}

void FireworkEmitter::update(double timePassed) {
    for (auto i = 0U; i < points.size(); ++i) {
        // If dead, remove the object with fast non-ordered deletion
        if (points[i].lifeLeft <= 0) {
            // Efficient removal (ie move last here and reset iteration by decrementing)
            points[i] = points.back();
            points.pop_back();
            i -= 1;
            continue;
        }

        // Update data
        points[i].lifeLeft -= 0.005;
        if (points[i].generateParticles) {
            points[i].direction.y -= 0.005;
            points[i].position += points[i].direction * 0.05f;
        } else {
            points[i].direction.y -= 0.0001;
            points[i].position += points[i].direction * 0.001f;
            points[i].color = points[i].color * 0.99f;
        }

        if (points[i].generateParticles) {
            const ParticleRender particle{
                    points[i].position,
                    points[i].direction * 0.2f - glm::vec3(0, 0.005, 0),
                    points[i].color * (1 - points[i].lifeLeft),
                    points[i].size * 0.9f,
                    points[i].lifeLeft * 0.9f,
                    false
            };
            points.push_back(particle);
        }
    }
}

void FireworkEmitter::draw() {
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBufferData(GL_ARRAY_BUFFER, 4000 * sizeof(ParticleRender), points.data(),
                 GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR()
    glDrawArrays(GL_POINTS, 0, points.size()); TEST_OPENGL_ERROR()
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void FireworkEmitter::emit(unsigned count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> unif(-1, 1);
    std::uniform_real_distribution<float> unifPos(0, 1);

    for (auto i = 0U; i < count; ++i) {
        ParticleRender particle{
            glm::vec3{0, 0, 0} + emitterPos,
            glm::normalize(glm::vec3{
                unif(gen), unifPos(gen), unif(gen)
            }),
            {0.4, 0.4, 0.4},
            0.3, 1, true
        };
        points.push_back(particle);
        particle.size = 1.5f;
        particle.color = particleColor;
        points.push_back(particle);
    }
}