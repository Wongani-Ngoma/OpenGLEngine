#include "Screen.h"

unsigned int Screen::SCR_HEIGHT = 600;
unsigned int Screen::SCR_WIDTH = 800;

unsigned int Screen::SCR_POS_X = 500;
unsigned int Screen::SCR_POS_Y = 100;

void Screen::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

Screen::Screen() :
    window(nullptr),
    isFullScreen(false) {

}

bool Screen::init() {
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (!window) {
        //window not created
        return false;
    }

    glfwMakeContextCurrent(window);

    return true;
}

void Screen::setParameters() {
    glViewport(100, 100, SCR_WIDTH, SCR_WIDTH);

    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void Screen::update() {
    glClearColor(pow(0.1f, gamma), pow(0.1f, gamma), pow(0.1f, gamma), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Screen::newFrame() {
    glfwSwapBuffers(window);
}

bool Screen::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(window, shouldClose);
}

void Screen::setWindowTitle(std::string& title) {
    glfwSetWindowTitle(window, title.c_str());
}

void Screen::toogleFullScreenWindowed() {
    if (!isFullScreen) {
        SCR_WIDTH = 1366;
        SCR_HEIGHT = 768;
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, SCR_WIDTH, SCR_HEIGHT, 60);
        isFullScreen = true;
    }
    else {
        SCR_WIDTH = 800;
        SCR_HEIGHT = 600;
        glfwSetWindowMonitor(window, NULL, SCR_POS_X, SCR_POS_Y, SCR_WIDTH, SCR_HEIGHT, 60);
        isFullScreen = false;
    }
}
