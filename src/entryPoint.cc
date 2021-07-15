#include "entryPoint.hh"
#include <iostream>
#include <ostream>
#include "wrappers/glmWrapper.hh"
#include "wrappers/glWrapper.hh"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <temp/vao.hh>
#include <temp/lightManager.hh>
#include "temp/camera.hh"
#include "temp/init_gl.hh"
#include "temp/program.hh"
#include "temp/objLoader.hh"
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
Camera camera(0, 0, 20);
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


int run() {
    if (!initOpenglAndContext(window))
        return -1;

    // Input callbacks
    glfwSetKeyCallback(window, handleKey);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, handleMouseMove);
    // Reset cursor to center of the screen
    int screen_w, screen_h;
    glfwGetWindowSize(window, &screen_w, &screen_h);
    glfwSetCursorPos(window, (double)screen_w / 2, (double)screen_h / 2);

    auto lightManager = LightManager();
    auto l1 = lightManager.addLight({ 0, 2, 30 }, { 1, 1, 1 });
    auto l2 = lightManager.addLight({ 0, 2, -30 }, { 0.8, 0.1, 0.1 });
    lightManager.updateLights();

    std::vector<std::string> faces
            {
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
        delete skyboxShader;
        return 1;
    }
    skyboxShader->use();
    skybox.bindToProgram(*skyboxShader);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // load models
    // -----------
    Model grass("textures/grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");
    Model tree("textures/tree/TreeSet4/Tree 02/Tree.obj");
    Models models{};

    models.addModel(grass,
                    "vert/obj_vertex_shader.glsl",
                    "frag/obj_fragment_shader.glsl");

    models.addModel(tree,
                    "vert/obj_vertex_shader.glsl",
                    "frag/obj_fragment_shader.glsl");


    // tweak models
    // -----------
    models.translateModel(0, glm::vec3(0.0f, -20.0f, 0.0f));
    models.scaleModel(0, glm::vec3(0.2f, 0.2f, 0.2f));
    models.rotateModel(0, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    models.translateModel(1, glm::vec3(0.0f, -18.0f, 0.0f));
    models.scaleModel(1, glm::vec3(3.0f, 3.0f, 3.0f));

    TEST_OPENGL_ERROR()

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        models.setUniformMat4(0, "transform_matrix", camera.getTransform(), false);
        models.setUniformMat4(1, "transform_matrix", camera.getTransform(), false);
        skyboxShader->setUniformMat4("transform_matrix", camera.getTransform(), true);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TEST_OPENGL_ERROR()

        skyboxShader->use();
        skybox.draw();

        models.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
//    delete tree_shader;
//    delete grass_shader;
    delete skyboxShader;
    return 0;
}
