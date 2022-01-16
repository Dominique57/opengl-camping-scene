// Created by dominique on 7/15/21.

#include "particleEmitter.hh"
#include <random>

ParticleEmitter::ParticleEmitter(const glm::vec3 &position, float rescaleFactor)
    : vaoId(0), vboData(0), emitterPos(position), rescaleFactor(rescaleFactor) {
    // Create and use vao
    glGenVertexArrays(1, &vaoId); TEST_OPENGL_ERROR()
    glBindVertexArray(vaoId); TEST_OPENGL_ERROR()

    // Create and use vbo
    glGenBuffers(1, &vboData); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, vboData); TEST_OPENGL_ERROR()
    glBufferData(GL_ARRAY_BUFFER, 2000 * sizeof(ParticleRender), nullptr,
                 GL_DYNAMIC_DRAW); TEST_OPENGL_ERROR()

    // Enable options
    glEnable(GL_PROGRAM_POINT_SIZE); TEST_OPENGL_ERROR()

    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}

void ParticleEmitter::bind(const program &program) {
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

    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}

void ParticleEmitter::update(double timePassed) {
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
        points[i].lifeLeft -= timePassed;
        // Set position
        points[i].position.y += 0.01;
        {
            auto emitterPosPlane = glm::vec3(0);
            auto particlePosPlane = points[i].position;
            particlePosPlane.y = 0;
            auto particleDir = glm::normalize(emitterPosPlane - particlePosPlane);
            auto length = glm::length(emitterPosPlane - particlePosPlane);
            if (points[i].position.y < 1.f)
                length = -length;
            else if (points[i].position.y < 2.f)
                length *= points[i].position.y;
            points[i].position.x += particleDir.x * length * 0.0025f;
            points[i].position.z += particleDir.z * length * 0.0025f;
        }

        // Set colors
        glm::vec3 refPos = points[i].position;
        refPos.y -= 0.4f;
        refPos.x *= 2.0f;
        refPos.z *= 2.0f;
        float distance = glm::length(refPos - glm::vec3(0));
        static auto lerpColors = std::vector<std::pair<float, glm::vec3>>{
            { 0, { 0.35, 0.3, 0.3 } },
            { 0.1, { 0.35, 0.3, 0.3 } },
            { 0.2, { 0.4, 0.2, 0.15 } },
            { 0.3, { 0.2, 0.1, 0.0 } },
            { 0.8, { 0.2, 0.07, 0.0 } },
            { 1.4, { 0.1, 0.02, 0.0 } },
            { 1.8f, { 0, 0, 0 } },
            { 2.2f, { 0, 0, 0 } },
            { 3.2f, { 0.15, 0.15, 0.15 } },
            { 4, { 0.05, 0.05, 0.05 } },
        };
        {
            auto j = 1U;
            for (; j < lerpColors.size(); ++j)
                if (distance < lerpColors[j].first)
                    break;
            points[i].color = glm::lerp(
                    lerpColors[j - 1].second, lerpColors[j].second,
                    distance - lerpColors[j - 1].first / (lerpColors[j].first - lerpColors[j - 1].first)
            );
        }
        // Set size
        auto lifeSpent = 1 - points[i].lifeLeft;
        if (lifeSpent < 0.5)
            points[i].size = glm::lerp(1.6f, 1.2f, lifeSpent * 2.f);
        else if (lifeSpent < 0.8)
            points[i].size = glm::lerp(1.2f, 1.0f, (lifeSpent - 0.5f) * 3.33f);
        else
            points[i].size = glm::lerp(1.0f, 0.8f, (lifeSpent - 0.8f) * 5.f);
    }
}

void ParticleEmitter::draw(const program &program) {
    // Set transform
    program.setUniformVec4("transform_pos_size", glm::vec4(emitterPos, rescaleFactor), true);

    // Draw
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

    glBindVertexArray(0); TEST_OPENGL_ERROR()
    glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR()
}

void ParticleEmitter::emit(unsigned count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> norm(0, 0.25);
    std::normal_distribution<float> normHigh(0, 0.75);

    for (auto i = 0U; i < count; ++i) {
        float xrand = std::clamp(norm(gen), -0.5f, 0.5f);
        float zrand = std::clamp(norm(gen), -0.5f, 0.5f);
        float life = std::abs(normHigh(gen));

        const ParticleRender particle{
            {xrand, 0, zrand},
            {1, 0, 0}, 1, life
        };
        points.push_back(particle);
    }
}