#include "init_gl.hh"

bool initGlew() {
    // glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Glew init failed:`" << glewGetErrorString(err)
            << "`!\n";
        return false;
    }

    return true;
}

static inline void printGlfwVersion() {
    printf("Compiled against GLFW %i.%i.%i\n",
            GLFW_VERSION_MAJOR,
            GLFW_VERSION_MINOR,
            GLFW_VERSION_REVISION);
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Running against GLFW %i.%i.%i\n", major, minor, revision);
}

bool initGlfw(GLFWwindow* &window) {
    printGlfwVersion();

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit()) {
        std::cerr << "Glfw init failed !\n";
        return false;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        std::cerr << "Glfw windows creation failed !\n";
        return false;
    }

    glfwMakeContextCurrent(window);

    return true;
}

bool initGl() {
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    return true;
}

bool initOpenglAndContext(GLFWwindow* &window) {
    return initGlfw(window) && initGlew() && initGl();
}
