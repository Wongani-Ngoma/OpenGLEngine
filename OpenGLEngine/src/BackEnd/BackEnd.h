#pragma once

#ifndef BACKEND_H
#define BACKEND_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "../Input/Keyboard.h"
#include "../Input/Joystick.h"
#include "../Input/Mouse.h"

namespace BackEnd {
    bool init();
    void checkErrors(const char* file);

    void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    void setParameters(); //For setting some params like callabacks

    void update(); //called at the begining of each loop, it clears the screen and some bits
    void newFrame(); //called at the end of each loop, swaps buffers and polls events

    void setShouldWindowClose(bool shouldClose);
    void setWindowTitle(std::string& title);
    bool isWindowed();
    void toogleFullScreen();
    void toogleWindowed();

    GLFWwindow* getWindow();
    float getAspectRatio();
    int getScreenWidth();
    int getScreenHeight();

    static void glfwError(int id, const char* description);


    bool shouldWindowClose();
};

#endif // !BACKEND_H