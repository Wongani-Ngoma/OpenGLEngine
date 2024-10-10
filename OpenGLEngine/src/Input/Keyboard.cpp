#include "Keyboard.h"

//The key and keyChanged static variables in keyboard.h(or any static variables in general) can only be initilized in a .cpp file
//GLFW_KEY_LAST is the number of keys in system i think
bool Keyboard::keys[GLFW_KEY_LAST] = { 0 }; 
bool Keyboard::keysChanged[GLFW_KEY_LAST] = { 0 };

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_RELEASE) {
        if (!keys[key]) {
            keys[key] = true;
        }
    }
    else {
        keys[key] = false;
    }

    keysChanged[key] = action != GLFW_REPEAT;
}

bool Keyboard::key(int key) {
    return keys[key];
}

bool Keyboard::keyChanged(int key) {
    bool ret = keysChanged[key];
    keysChanged[key] = false;
    return ret;
}

bool Keyboard::keyWentUp(int key) {
    return !keys[key] && keyChanged(key);
}

bool Keyboard::keyWentDown(int key) {
    return keys[key] && keyChanged(key);
}
