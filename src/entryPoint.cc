#include "entryPoint.hh"
#include <iostream>
#include <ostream>
#include "wrappers/glmWrapper.hh"
#include "wrappers/glWrapper.hh"
#include <GLFW/glfw3.h>
#include <vector>
#include <temp/vao.hh>
#include <temp/lightManager.hh>
#include "temp/camera.hh"
#include "temp/init_gl.hh"
#include "temp/program.hh"
#include "temp/objLoader.hh"
#include "temp/stb_image.h"

void addObjects(const program &program) {
    // Vertex array
    GLfloat points[] = {
            0.0f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f, // pos
            1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f, // rgb
            0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f, // normals
    };

    // Create VAO
    GLuint vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Create vbo
    GLuint vbo_id = 0;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, 3 * 9 * sizeof(GLfloat),
                 points, GL_STATIC_DRAW);

    // Activate VBO position reading order
    GLint posLoc = glGetAttribLocSafe(program.get_program(), "vPosition");
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0,
                          nullptr);
    glEnableVertexAttribArray(posLoc);

    // Activate VBO color reading order
    GLint colorLoc = glGetAttribLocSafe(program.get_program(), "vColor");
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0,
                          (void*)(3 * 3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colorLoc);

    // Activate VBO normals reading order
    GLint normLoc = glGetAttribLocSafe(program.get_program(), "vNormal");
    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 0,
                          (void*)(2 * 3 * 3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(normLoc);
}

void addVariables(const program &program) {
    auto prog = program.get_program();
    // Setup uniform variables

    // Set light position
    GLint listPosLoc = glGetUniformLocSafe(prog, "lightPos");
    glProgramUniform3f(prog, listPosLoc, 0.25f, 0.25f, 1.f);

    // Set light color
    GLint lightColorLoc = glGetUniformLocSafe(prog, "lightColor");
    glProgramUniform3f(prog, lightColorLoc, 1.f, 1.f, 1.f);

    // Set transform matrix
    /*
    auto translateMat = glm::mat4(1.f);
    auto viewMat = glm::lookAt(
        glm::vec3(0, 0, 4),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    auto projectMat = glm::perspective(
        glm::radians(45.0f),
        1.0f,
        0.1f,
        100.f
    );
    auto transormMat = projectMat * viewMat * translateMat;
    GLint transformMatLoc = glGetUniformLocSafe(prog, "transform_matrix");
    glProgramUniformMatrix4fv(prog, transformMatLoc, 1, GL_FALSE,
            &transormMat[0][0]);
    */
}

#define KEY_FOREWARD GLFW_KEY_W
#define KEY_BACKWARDS GLFW_KEY_S
#define KEY_LEFT GLFW_KEY_A
#define KEY_RIGHT GLFW_KEY_D
#define KEY_UP GLFW_KEY_SPACE
#define KEY_DOWN GLFW_KEY_LEFT_CONTROL

GLFWwindow* window = nullptr;
Camera camera(0, 0, 4);
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

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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
    auto* program = program::make_program_path("vert/shader2.glsl", "frag/shader2.glsl");
    if (!program->isready()) {
        std::cerr << "Failed to build shader :\n" << program->getlog() << '\n';
        delete program;
        return 1;
    }

    auto objLoader = ObjLoader();
    auto objData = objLoader.loadObj("test.obj");
    auto objDatatea = objLoader.loadObj("teapot.obj");
    for(auto& vertice : objDatatea.vertices)
        vertice += 10.0;
    auto vao = Vao();
    vao.addObjData(objData);
    vao.addObjData(objDatatea);
    vao.bindToProgram(*program, "vPosition");

    auto lightManager = LightManager();
    auto l1 = lightManager.addLight({ 1, 1, 1 }, { 0, 1, 0 });
    auto l2 = lightManager.addLight({ 1, 0, 0 }, { 0, 0, 1 });
    lightManager.updateLights();

    // Load skybox

    float skyboxVertices[] = {
            // positions
            -50.0f,  50.0f, -50.0f,
            -50.0f, -50.0f, -50.0f,
            50.0f, -50.0f, -50.0f,
            50.0f, -50.0f, -50.0f,
            50.0f,  50.0f, -50.0f,
            -50.0f,  50.0f, -50.0f,

            -50.0f, -50.0f,  50.0f,
            -50.0f, -50.0f, -50.0f,
            -50.0f,  50.0f, -50.0f,
            -50.0f,  50.0f, -50.0f,
            -50.0f,  50.0f,  50.0f,
            -50.0f, -50.0f,  50.0f,

            50.0f, -50.0f, -50.0f,
            50.0f, -50.0f,  50.0f,
            50.0f,  50.0f,  50.0f,
            50.0f,  50.0f,  50.0f,
            50.0f,  50.0f, -50.0f,
            50.0f, -50.0f, -50.0f,

            -50.0f, -50.0f,  50.0f,
            -50.0f,  50.0f,  50.0f,
            50.0f,  50.0f,  50.0f,
            50.0f,  50.0f,  50.0f,
            50.0f, -50.0f,  50.0f,
            -50.0f, -50.0f,  50.0f,

            -50.0f,  50.0f, -50.0f,
            50.0f,  50.0f, -50.0f,
            50.0f,  50.0f,  50.0f,
            50.0f,  50.0f,  50.0f,
            -50.0f,  50.0f,  50.0f,
            -50.0f,  50.0f, -50.0f,

            -50.0f, -50.0f, -50.0f,
            -50.0f, -50.0f,  50.0f,
            50.0f, -50.0f, -50.0f,
            50.0f, -50.0f, -50.0f,
            -50.0f, -50.0f,  50.0f,
            50.0f, -50.0f,  50.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    std::vector<std::string> faces
            {
                    "skybox/right.jpg",
                    "skybox/left.jpg",
                    "skybox/top.jpg",
                    "skybox/bottom.jpg",
                    "skybox/front.jpg",
                    "skybox/back.jpg"
            };
    unsigned int cubemapTexture = loadCubemap(faces);

    auto* skyboxShader = program::make_program_path("vert/vert_skybox.glsl", "frag/frag_skybox.glsl");
    if (!skyboxShader->isready()) {
        std::cerr << "Failed to build shader :\n" << skyboxShader->getlog() << '\n';
        delete skyboxShader;
        return 1;
    }

    // shader configuration
    // --------------------
    program->use();

    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        program->setUniformMat4("transform_matrix", camera.getTransform(), true);
        skyboxShader->setUniformMat4("transform_matrix", camera.getTransform(), true);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); TEST_OPENGL_ERROR()

        program->use();
        vao.draw();

        skyboxShader->use();
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    delete program;
    delete skyboxShader;
    return 0;
}
