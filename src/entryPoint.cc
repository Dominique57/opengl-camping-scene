#include "entryPoint.hh"
#include <iostream>
#include <ostream>
#include "wrappers/glmWrapper.hh"
#include "wrappers/glWrapper.hh"
#include <GLFW/glfw3.h>
#include <vector>
#include <temp/lightManager.hh>
#include <temp/firePlace.hh>
#include "temp/camera.hh"
#include "temp/init_gl.hh"
#include "temp/program.hh"
#include "temp/stb_image.h"
#include "texture/skybox.hh"
#include "temp/model.hh"
#include "temp/models.hh"

#define KEY_FOREWARD GLFW_KEY_W
#define KEY_BACKWARDS GLFW_KEY_S
#define KEY_LEFT GLFW_KEY_A
#define KEY_RIGHT GLFW_KEY_D
#define KEY_UP GLFW_KEY_SPACE
#define KEY_DOWN GLFW_KEY_LEFT_CONTROL

GLFWwindow* window = nullptr;
Camera camera(10, -10, 30);
void handleKey(GLFWwindow* window, int key, int, int, int) {
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

int run() {
    if (!initOpenglAndContext(window))
        return -1;

    // Input callbacks
    glfwSetKeyCallback(window, handleKey);
    glfwSetCursorPosCallback(window, handleMouseMove);
    glfwSetScrollCallback(window, handleScroll);
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

    auto* skyboxShader = program::make_program_path("vert/vert_skybox.glsl", "frag/frag_skybox.glsl");
    if (!skyboxShader->isready()) {
        std::cerr << "Failed to build shader :\n" << skyboxShader->getlog() << '\n';
        return 1;
    }
    skyboxShader->use();
    skybox.bindToProgram(*skyboxShader);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
//    stbi_set_flip_vertically_on_load(true);

    // load models
    Model grass("textures/grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj",
                "vert/obj_vertex_shader.glsl", "frag/obj_fragment_shader.glsl",
                {1, 0.1, 32});
    Model tree("textures/tree/TreeSet4/Tree 02/Tree.obj",
               "vert/obj_vertex_shader.glsl", "frag/obj_fragment_shader.glsl",
               {0.8, 0.8, 256});
    Model bench("textures/bench/Lawka 2 - model.obj",
                "vert/obj_vertex_shader.glsl", "frag/obj_fragment_shader.glsl",
                {0.8, 0.8, 256});
    Model firewood("textures/firewood/Bonfire model 1.obj",
                   "vert/obj_vertex_shader.glsl", "frag/obj_fragment_shader.glsl",
                   {0.8, 0.8, 256});

    Models models{};
    models.addModel(grass);
    models.addModel(tree);
    models.addModel(bench);
    models.addModel(firewood);


    // tweak models
    // -----------
    models.translateModel(0, glm::vec3(0.0f, -20.0f, 0.0f));
    models.scaleModel(0, glm::vec3(1.f, 0.2f, 1.f));
    models.rotateModel(0, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    models.translateModel(1, glm::vec3(-3.0f, -18.0f, 0.0f));
    models.scaleModel(1, glm::vec3(5.0f, 5.0f, 5.0f));

    models.translateModel(2, glm::vec3(5.0f, -18.0f, -10.0f));
    models.scaleModel(2, glm::vec3(0.5f, 0.5f, 0.5f));
    models.rotateModel(2, -20.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    models.translateModel(3, glm::vec3(10.0f, -18.0f, 0.0f));
    models.scaleModel(3, glm::vec3(2.0f, 2.0f, 2.0f));

    TEST_OPENGL_ERROR()
    auto pointShader = program::make_program_path("vert/shaderPoints.glsl", "frag/shaderPoints.glsl");
    if (!pointShader->isready()) {
        std::cerr << pointShader->getlog();
        return 1;
    }
    auto firePlace = FirePlace({ 10, -17, 0 }, 3.f, lightManager);
    firePlace.bind(*pointShader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        models.setUniformMat4(0, "transform_matrix", camera.getTransform(), false);
        models.setUniformMat4(0, "view_matrix", camera.getView(), false);
        models.setUniformMat4(1, "transform_matrix", camera.getTransform(), false);
        models.setUniformMat4(1, "view_matrix", camera.getView(), false);
        models.setUniformMat4(2, "transform_matrix", camera.getTransform(), false);
        models.setUniformMat4(2, "view_matrix", camera.getView(), false);
        models.setUniformMat4(3, "transform_matrix", camera.getTransform(), false);
        models.setUniformMat4(3, "view_matrix", camera.getView(), false);
        skyboxShader->setUniformMat4("transform_matrix", camera.getTransform(), true);
        pointShader->setUniformMat4("transform_matrix", camera.getTransform(), true);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TEST_OPENGL_ERROR()

        skyboxShader->use();
        skybox.draw();

        models.draw();

        pointShader->use();
        firePlace.update();
        firePlace.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
