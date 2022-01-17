#include "entryPoint.hh"
#include <random>
#include <iostream>
#include <ostream>
#include "wrappers/glmWrapper.hh"
#include "wrappers/glWrapper.hh"
#include <GLFW/glfw3.h>
#include <vector>
#include <manager/lightManager.hh>
#include <manager/firePlace.hh>
#include <player/camera.hh>
#include <manager/gpuParticleEmitter.hh>
#include <texture/gBuffer.hh>
#include <temp/quad.hh>
#include <texture/ssaoBuffer.hh>
#include "temp/init_gl.hh"
#include "temp/program.hh"
#include "texture/skybox.hh"
#include "model/model.hh"
#include "model/models.hh"
#include "manager/fireWorkEmitter.hh"

#define KEY_FOREWARD GLFW_KEY_W
#define KEY_BACKWARDS GLFW_KEY_S
#define KEY_LEFT GLFW_KEY_A
#define KEY_RIGHT GLFW_KEY_D
#define KEY_UP GLFW_KEY_SPACE
#define KEY_DOWN GLFW_KEY_LEFT_CONTROL

GLFWwindow* window = nullptr;
Camera camera(10, -10, 30);
bool enableFireWorks = false;
void handleKey(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    glm::vec3 moveOffset{ 0, 0, 0 };
    if (key == KEY_FOREWARD) {
        moveOffset.x = 1;
    } else if (key == KEY_BACKWARDS) {
        moveOffset.x = -1;
    } else if (key == KEY_RIGHT) {
        moveOffset.z = 1;
    } else if (key == KEY_LEFT) {
        moveOffset.z = -1;
    } else if (key == KEY_UP) {
        moveOffset.y = 1;
    } else if (key == KEY_DOWN) {
        moveOffset.y = -1;
    } else if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    } else if (key == GLFW_KEY_P) {
        std::cout << "[CAMERA position]: " << camera.viewCameraPos() << std::endl;
    } else if (key == GLFW_KEY_ENTER) {
        std::cout << scanCode << ":" << action << ":" << mods << std::endl;
        if (action == 1)
            enableFireWorks = !enableFireWorks;
        std::cout << enableFireWorks << std::endl;
    }

    camera.moveCamera(moveOffset);
}

void handleMouseMove(GLFWwindow* window, double xpos, double ypos) {
    int screen_w, screen_h;
    glfwGetWindowSize(window, &screen_w, &screen_h);
    xpos -= (double)screen_w / 2;
    ypos -= (double)screen_h / 2;
    camera.rotateCamera({ xpos, -ypos });
    glfwSetCursorPos(window, (double)screen_w / 2, (double)screen_h / 2);
}

void handleScroll(GLFWwindow*, double, double yoffset) {
    camera.alterMovementSpeed(yoffset / 10. + 1.f);
}

void handleFramebufferResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int run() {
    if (!initOpenglAndContext(window))
        return -1;

    // Input callbacks
    glfwSetKeyCallback(window, handleKey);
    glfwSetCursorPosCallback(window, handleMouseMove);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, handleScroll);
    glfwSetFramebufferSizeCallback(window, handleFramebufferResize);
    // Reset cursor to center of the screen
    int screen_w, screen_h;
    glfwGetWindowSize(window, &screen_w, &screen_h);
    glfwSetCursorPos(window, (double)screen_w / 2, (double)screen_h / 2);

    auto lightManager = LightManager();
//    lightManager.addLight(camera.viewCameraPos(), { 1, 1, 1 });
    lightManager.updateLights();

    std::vector<std::string> faces {
            "textures/skybox/grimm_night/right.png",
            "textures/skybox/grimm_night/left.png",
            "textures/skybox/grimm_night/top.png",
            "textures/skybox/grimm_night/bottom.png",
            "textures/skybox/grimm_night/front.png",
            "textures/skybox/grimm_night/back.png"
    };

    // skybox VAO
    Skybox skybox{faces};

    auto* skyboxShader = program::make_program_path({
        {"vert/vert_skybox.glsl", GL_VERTEX_SHADER, "VERTEX"},
        {"frag/frag_skybox.glsl", GL_FRAGMENT_SHADER, "FRAGMENT"},
    });

    if (!skyboxShader->isready()) {
        std::cerr << "Failed to build shader :\n" << skyboxShader->getlog() << '\n';
        return 1;
    }
    skyboxShader->use();
    skybox.bindToProgram(*skyboxShader);

    // Deferred rendering : geometry pass
    auto* objShader = program::make_program_path({
        {"vert/obj_deferred.glsl", GL_VERTEX_SHADER, "VERTEX"},
        {"frag/obj_deferred.glsl", GL_FRAGMENT_SHADER, "FRAGMENT"},
    });
    if (!objShader->isready()) {
        std::cerr << "Failed to build shader :\n" << objShader->getlog() << '\n';
        return 1;
    }

    // Deferred rendering : lighting pass
    auto* objLightShader = program::make_program_path({
        {"vert/obj_lighting.glsl", GL_VERTEX_SHADER, "VERTEX"},
        {"frag/obj_lighting.glsl", GL_FRAGMENT_SHADER, "FRAGMENT"},
    });
    if (!objLightShader->isready()) {
        std::cerr << "Failed to build shader :\n" << objLightShader->getlog() << '\n';
        return 1;
    }

    // Deferred rendering : ssao pass
    auto* ssaoShader = program::make_program_path({
        {"vert/ssao_pass.glsl", GL_VERTEX_SHADER, "VERTEX"},
        {"frag/ssao_pass.glsl", GL_FRAGMENT_SHADER, "FRAGMENT"},
    });
    if (!ssaoShader->isready()) {
        std::cerr << "Failed to build shader :\n" << ssaoShader->getlog() << '\n';
        return 1;
    }

    auto quad = Quad();

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    // stbi_set_flip_vertically_on_load(true);

    // load models
    Model grass("textures/grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj", *objShader, {1, 0.1, 32});
    Model tree("textures/tree/Tree.obj", *objShader, {0.8, 0.3, 256});
    Model bench("textures/bench/Lawka 2 - model.obj", *objShader, {0.8, 0.8, 256});
    Model firewood("textures/firewood/Bonfire model 1.obj", *objShader, {0.8, 0.8, 256});

    stbi_set_flip_vertically_on_load(true);
    Model backpack("textures/backpack/backpack.obj", *objShader, {0.8, 0.8, 256});
    stbi_set_flip_vertically_on_load(false);

    Model car("textures/car/RV Trailer Truck doors open.obj", *objShader, {0.8, 0.8, 256});

    Models models{};
    auto grass_id1 = models.addModel(grass);
    auto tree_id1 = models.addModel(tree);
    auto bench_id1 = models.addModel(bench);
    auto firewood_id1 = models.addModel(firewood);
    auto backpack_id1 = models.addModel(backpack);
    auto car_id1 = models.addModel(car);

    // tweak models
    // -----------
    models.translateModel(grass_id1, glm::vec3(0.0f, -20.0f, 0.0f));
    models.scaleModel(grass_id1, glm::vec3(1.f, 0.2f, 1.f));
    models.rotateModel(grass_id1, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    models.translateModel(tree_id1, glm::vec3(-15.0f, -18.0f, 5.0f));
    models.scaleModel(tree_id1, glm::vec3(5.0f, 5.0f, 5.0f));

    models.translateModel(bench_id1, glm::vec3(5.0f, -18.0f, -10.0f));
    models.scaleModel(bench_id1, glm::vec3(0.5f, 0.5f, 0.5f));
    models.rotateModel(bench_id1, -20.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    models.translateModel(firewood_id1, glm::vec3(5.0f, -18.0f, 5.0f));
    models.scaleModel(firewood_id1, glm::vec3(2.0f, 2.0f, 2.0f));

    models.translateModel(backpack_id1, glm::vec3(12.0f, -14.0f, -10.5f));
    models.rotateModel(backpack_id1, -40.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    models.rotateModel(backpack_id1, -10.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    models.translateModel(car_id1, glm::vec3(30.0f, -18.0f, -5.0f));
    models.rotateModel(car_id1, 75.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    models.scaleModel(car_id1, glm::vec3(0.04f, 0.04f, 0.04f));

    // tree generation
    // ---------------
    for (int i = 0; i < 20; ++i)
    {
        int id = models.addModel(tree);
        models.translateModel(id, glm::vec3(-50.0f * sin(i), -18.0f, -50.0f + 10 * cos(i)));
        float angle = (rand() / RAND_MAX) * 360;
        models.rotateModel(id, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        models.scaleModel(id, glm::vec3(5.0f, 5.0f, 5.0f));
    }

    auto pointShader = program::make_program_path({
        {"vert/shaderPoints.glsl", GL_VERTEX_SHADER, "VERTEX"},
        {"frag/shaderPoints.glsl", GL_FRAGMENT_SHADER, "FRAGMENT"},
    });
    if (!pointShader->isready()) {
        std::cerr << pointShader->getlog();
        return 1;
    }
    auto particleUpdateShader = program::make_program_path({
        {"compute/particle_update.glsl", GL_COMPUTE_SHADER, "COMPUTE"},
    });
    if (!particleUpdateShader->isready()) {
        std::cerr << particleUpdateShader->getlog();
        return 1;
    }
    auto gpuParticleEmitter = GpuParticleEmitter({0, -17, 5}, 3.f);
    gpuParticleEmitter.bind_fragment(*pointShader);
    gpuParticleEmitter.bind_compute(*particleUpdateShader);
    auto firePlace = FirePlace({ 5, -17, 5 }, 2.5f, lightManager);
    firePlace.bind(*pointShader);
    auto fireworkEmitter = FireworkEmitter({5, -2, 40}, {0, 0, 1}, 1.f, lightManager);
    fireworkEmitter.bind(*pointShader);

    auto gBuf = GBuffer(screen_w, screen_h);
    auto ssaoBuf = SsaoBuffer(screen_w, screen_h);

    objLightShader->use();
    objLightShader->setUniformInt("gPosition", 0);
    objLightShader->setUniformInt("gNormal", 1);
    objLightShader->setUniformInt("gAlbedo", 2);
    objLightShader->setUniformInt("ssaoOcclusion", 3);

    ssaoShader->use();
    ssaoShader->setUniformInt("gPosition", 0);
    ssaoShader->setUniformInt("gNormal", 1);
    ssaoShader->setUniformInt("noiseTex", 2);
    for (auto i = 0U; i < ssaoBuf.getKernel().size(); ++i) {
        auto name = std::string("kernel[") + std::to_string(i) + "]";
        ssaoShader->setUniformVec3(name.c_str(), ssaoBuf.getKernel()[i], false);
    }
    ssaoShader->setUniformVec2("noiseScale", {screen_w / 4.f, screen_h / 4.f});


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        objShader->setUniformMat4("projection_matrix", camera.getProjection(), false);
        objShader->setUniformMat4("view_matrix", camera.getView(), false);
        ssaoShader->setUniformMat4("projection_matrix", camera.getProjection(), false);
        objLightShader->setUniformMat4("view_matrix", camera.getView(), false);
//        objLightShader->setUniformVec3("cameraPos", camera.viewCameraPos(), false);
        skyboxShader->setUniformMat4("transform_matrix", camera.getTransform(), true);
        pointShader->setUniformMat4("transform_matrix", camera.getTransform(), true);

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TEST_OPENGL_ERROR()

        // Deferred rendering
        {
            // Fill GBuffer
            glBindFramebuffer(GL_FRAMEBUFFER, gBuf.getFboId());
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TEST_OPENGL_ERROR()

            objShader->use();
            models.draw();

            // Ssao pass
            glBindFramebuffer(GL_FRAMEBUFFER, ssaoBuf.getFboId());
            glClear(GL_COLOR_BUFFER_BIT);
            ssaoShader->use();
            {
                glActiveTexture(GL_TEXTURE0 + 0);
                glBindTexture(GL_TEXTURE_2D, gBuf.getPositionTexId()); TEST_OPENGL_ERROR()
                glActiveTexture(GL_TEXTURE0 + 1);
                glBindTexture(GL_TEXTURE_2D, gBuf.getNormalTexId()); TEST_OPENGL_ERROR()
                glActiveTexture(GL_TEXTURE0 + 2);
                glBindTexture(GL_TEXTURE_2D, ssaoBuf.getNoiseTexId()); TEST_OPENGL_ERROR()
                glActiveTexture(GL_TEXTURE0);
            }
            quad.draw();

            // Lighting pass
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            objLightShader->use();
            {
                glActiveTexture(GL_TEXTURE0 + 0);
                glBindTexture(GL_TEXTURE_2D, gBuf.getPositionTexId()); TEST_OPENGL_ERROR()
                glActiveTexture(GL_TEXTURE0 + 1);
                glBindTexture(GL_TEXTURE_2D, gBuf.getNormalTexId()); TEST_OPENGL_ERROR()
                glActiveTexture(GL_TEXTURE0 + 2);
                glBindTexture(GL_TEXTURE_2D, gBuf.getAlbedoTexId()); TEST_OPENGL_ERROR()
                glActiveTexture(GL_TEXTURE0 + 3);
                glBindTexture(GL_TEXTURE_2D, ssaoBuf.getColorTexId()); TEST_OPENGL_ERROR()
                glActiveTexture(GL_TEXTURE0);
            }
            quad.draw();

            // Depth buffer copy for foreward pass
            gBuf.use();
            glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuf.getFboId()); TEST_OPENGL_ERROR()
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); TEST_OPENGL_ERROR()
            glBlitFramebuffer(0, 0, screen_w, screen_h, 0, 0, screen_w, screen_h, GL_DEPTH_BUFFER_BIT, GL_NEAREST); TEST_OPENGL_ERROR()
            glBindFramebuffer(GL_FRAMEBUFFER, 0); TEST_OPENGL_ERROR()
        }

        skyboxShader->use();
        skybox.draw();

        pointShader->use();
        fireworkEmitter.update(0.004);
        static int fireWorkCount = 0;
        if (fireWorkCount <= 0 && enableFireWorks) {

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<unsigned>unif_int(50, 300);
            fireWorkCount = unif_int(gen);


            std::uniform_real_distribution<float> unif_flot(-5, 5);
            auto copyPos = fireworkEmitter.getPosition();
            fireworkEmitter.setPosition(fireworkEmitter.getPosition() +
                glm::vec3(unif_flot(gen), unif_flot(gen), unif_flot(gen)));

            const std::vector<glm::vec3> possiblesColors{
                {1, 0, 0},
                {1, 0, 1},
                {1, 1, 0},
                {0, 1, 0},
                {0, 1, 1},
            };
            std::uniform_int_distribution<unsigned> unif_index(0, possiblesColors.size());
            fireworkEmitter.setParticleColor(possiblesColors[unif_index(gen)]);

            fireworkEmitter.emit(20);
            fireworkEmitter.setPosition(copyPos);
        }
        if (enableFireWorks)
            fireWorkCount -= 1;
        fireworkEmitter.draw();

        firePlace.update();
        firePlace.draw(*pointShader);

        gpuParticleEmitter.draw(*pointShader);

        particleUpdateShader->use();
        gpuParticleEmitter.update();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
