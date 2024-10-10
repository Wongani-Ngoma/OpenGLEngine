#include "BackEnd.h"

namespace BackEnd {
    static GLFWwindow* _window;

    static unsigned int _windowedScrWidth = 800;
    static unsigned int _windowedScrHeight = 700;

    static unsigned int _windowedScrPosX = 20;
    static unsigned int _windowedScrPosY = 20;

    static unsigned int _currentScrWidth = _windowedScrWidth;
    static unsigned int _currentScrHeight = _windowedScrHeight;

    static bool _isFullScreen = false;
    static bool _shouldWindowClose = false;
    static float _gamma = 2.2; //should be the same as that in FrameBuffer shader
}

bool BackEnd::init() {
    glfwSetErrorCallback(&glfwError);

    if (!glfwInit()) {
        std::cout << "GLFW did not initilize\n";
        return 0;
    }

#ifdef __APPLE__ //For macOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

    _window = glfwCreateWindow(_windowedScrWidth, _windowedScrHeight, "OpenGL", NULL, NULL);
    if (!_window) {
        std::cout << "Could not create window\n";
        glfwTerminate();
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); //for debugging
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return false;
    }

    // print info about this machine's graphics card
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    std::cout << "\nGPU: " << renderer << "\n";
    std::cout << "GL version: " << major << "." << minor << "\n\n";

    setParameters();

    return true;
}


void BackEnd::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    _currentScrWidth = width;
    _currentScrHeight = height;
}

void BackEnd::setParameters() {
    glViewport(0, 0, _currentScrWidth, _currentScrHeight);

    glfwSetKeyCallback(_window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(_window, Mouse::cursorCallback);
    glfwSetMouseButtonCallback(_window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(_window, Mouse::mouseWheelCallback);
    glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void BackEnd::update() {
    glClearColor(pow(0.1f, _gamma), pow(0.1f, _gamma), pow(0.1f, _gamma), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void BackEnd::newFrame() {
    glfwSwapBuffers(_window);
}

bool BackEnd::shouldWindowClose() {
    return _shouldWindowClose;
}

void BackEnd::setShouldWindowClose(bool shouldClose) {
    _shouldWindowClose = true;
}

void BackEnd::setWindowTitle(std::string& title) {
    glfwSetWindowTitle(_window, title.c_str());
}

bool BackEnd::isWindowed() {
    return !_isFullScreen;
}

void BackEnd::toogleFullScreen() {
    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());;
    _currentScrWidth = vidmode->width;
    _currentScrHeight = vidmode->height;
    glfwSetWindowMonitor(_window, glfwGetPrimaryMonitor(), 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
    glViewport(0, 0, _currentScrWidth, _currentScrHeight);
    _isFullScreen = true;
}

void BackEnd::toogleWindowed() {
    glfwSetWindowMonitor(_window, NULL, _windowedScrPosX, _windowedScrPosY, _windowedScrWidth, _windowedScrHeight, 0);
    _currentScrWidth = _windowedScrWidth;
    _currentScrHeight = _windowedScrHeight;
    glViewport(0, 0, _currentScrWidth, _currentScrHeight);
    _isFullScreen = false;
}

GLFWwindow* BackEnd::getWindow() {
    return _window;
}

float BackEnd::getAspectRatio() {
    return (float)_currentScrWidth / (float)_currentScrHeight;
}

int BackEnd::getScreenWidth() {
    return _currentScrWidth;
}

int BackEnd::getScreenHeight() {
    return _currentScrHeight;
}

void BackEnd::glfwError(int id, const char* description) {
    std::cout << description << std::endl;
}


void BackEnd::checkErrors(const char* file) {
    std::string desc;
    int error = glGetError();
    switch (error) {
    case 1280:
        desc = "OpenGL Error in " + std::string(file) + ": \n\tCode: " + std::to_string(error) + "\n\tDescription: " + "an enumeration parameter is not legal.\n\n";
        break;
    case 1281:
        desc = "OpenGL Error in " + std::string(file) + ":\n\tCode: " + std::to_string(error) + "\n\tDescription: " + "a value parameter is not legal.\n\n";
        break;
    case 1282:
        desc = "OpenGL Error in " + std::string(file) + ":\n\tCode: " + std::to_string(error) + "\n\tDescription: " + "state for a command is not legal for its given parameters.\n\n";
        break;
    case 1283:
        desc = "OpenGL Error in " + std::string(file) + ":\n\tCode: " + std::to_string(error) + "\n\tDescription: " + "a stack pushing operation caused stack overflow..\n\n";
        break;
    case 1284:
        desc = "OpenGL Error in " + std::string(file) + ":\n\tCode: " + std::to_string(error) + "\n\tDescription: " + "a stack popping operation occured while the stack is at its lowest point.\n\n";
        break;
    case 1285:
        desc = "OpenGL Error in " + std::string(file) + ":\n\tCode: " + std::to_string(error) + "\n\tDescription: " + "a memory allocation operation cannot allocate(enough) memory.\n\n";
        break;
    case 1286:
        desc = "OpenGL Error in " + std::string(file) + ":\n\tCode: " + std::to_string(error) + "\n\tDescription: " + "reading or writing to a framebuffer that is not complete.\n\n";
        break;
    default:
        break;
    }

    std::cout << desc;
}
