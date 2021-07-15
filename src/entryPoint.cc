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

    /* Make the window's context current */
//    auto* program = program::make_program_path("vert/shader2.glsl", "frag/shader2.glsl");
//    if (!program->isready()) {
//        std::cerr << "Failed to build shader :\n" << program->getlog() << '\n';
//        delete program;
//        return 1;
//    }
//    program->use();

//    auto objLoader = ObjLoader();
////    auto objData = objLoader.loadObj("cube.obj");
//    auto objData = objLoader.loadObj("teapot_norm.obj");
//    auto vao = Vao();
//    vao.addObjData(objData);
//    vao.bindToProgram(*program, "vPosition", "vNormal");

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

    // build and compile shaders
    // -------------------------
    auto* grass_shader = program::make_program_path("vert/obj_vertex_shader.glsl", "frag/obj_fragment_shader.glsl");
    if (!grass_shader->isready()) {
        std::cerr << "Failed to build shader :\n" << grass_shader->getlog() << '\n';
        delete grass_shader;
        return 1;
    }

    auto* tree_shader = program::make_program_path("vert/obj_vertex_shader.glsl", "frag/obj_fragment_shader.glsl");
    if (!tree_shader->isready()) {
        std::cerr << "Failed to build shader :\n" << tree_shader->getlog() << '\n';
        delete tree_shader;
        return 1;
    }

    // load models
    // -----------
    Model grass("textures/grass/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");
    Model tree("textures/tree/TreeSet2/TreeSet.blend");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        grass_shader->setUniformMat4("transform_matrix", camera.getTransform(), false);
        tree_shader->setUniformMat4("transform_matrix", camera.getTransform(), false);
//        program->setUniformVec3("cameraPos", camera.viewCameraPos(), false);
        skyboxShader->setUniformMat4("transform_matrix", camera.getTransform(), true);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TEST_OPENGL_ERROR()

//        program->use();
//        vao.draw();

        skyboxShader->use();
        skybox.draw();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -20.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        TEST_OPENGL_ERROR()
        grass_shader->setUniformMat4("model", model, true);
        grass_shader->use();
        grass.draw(grass_shader);

        glm::mat4 model_tree = glm::mat4(1.0f);
        model_tree = glm::translate(model_tree, glm::vec3(0.0f, -18.8f, 0.0f)); // translate it down so it's at the center of the scene
        model_tree = glm::scale(model_tree, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        model_tree = glm::rotate(model_tree, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        TEST_OPENGL_ERROR()
        tree_shader->setUniformMat4("model", model_tree, true);
        tree_shader->use();
        tree.draw(tree_shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    delete tree_shader;
    delete grass_shader;
    delete skyboxShader;
    return 0;
}
