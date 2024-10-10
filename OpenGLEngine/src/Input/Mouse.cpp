#include "Mouse.h"

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scrollDX = 0;
double Mouse::scrollDY = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::cursorCallback(GLFWwindow* window, double _x, double _y) {
    x = _x;
    y = _y;

    if (firstMouse) { //If it's the first time the mouse is moved
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    dx = x - lastX;
    dy = lastY - y; //Inverted

    lastX = x;
    lastY = y;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action != GLFW_RELEASE) {
        if (!buttons[button]) {
            buttons[button] = true;
        }
    }
    else {
        buttons[button] = false;
    }

    buttonsChanged[button] = action != GLFW_REPEAT; //If the button is repeated, the button has not changed, thats what this means
}

void Mouse::mouseWheelCallback(GLFWwindow* windows, double dx, double dy) {
    scrollDX = dx;
    scrollDY = dy;
}

void Mouse::updateOffsets() {

}

double Mouse::getMouseX() {
    return x;
}

double Mouse::getMouseY() {
    return y;
}

//Once the mouse is moved and the callback is called, dx & dy get updated, but once the mouse is stationary, dx & dy should be zero
//but since the mouse is not moving the callback will not be called, the callback will not be called hence dy & dx will not be 
//and the program will think the mouse is still moving, so when these methods are called, dx & dy should be set to zero in case the
//mouse stops moving
double Mouse::getDX() {
    double _dx = dx;
    //dx = 0;
    return _dx;
}

double Mouse::getDY() {
    double _dy = dy;
    //dy = 0;
    return _dy;
}

void Mouse::resetOffsets() {
    dx = 0;
    dy = 0;
}

double Mouse::getScrollDX() {
    double dx = scrollDX;
    scrollDX = 0;
    return dx;
}

double Mouse::getScrollDY() {
    double dy = scrollDY;
    scrollDY = 0;
    return dy;
}

bool Mouse::button(int button) {
    return buttons[button];
}

bool Mouse::buttonChanged(int button) {
    bool ret = buttonsChanged[button];
    buttonsChanged[button] = false;
    return ret;
}

bool Mouse::buttonWentUp(int button) {
    return !buttons[button] && buttonChanged(button); //retturn true if the button was down (!) and it has changed, making it up
}

bool Mouse::buttonWentDown(int button) {
    return buttons[button] && buttonChanged(button);
}
