#pragma once
#ifndef DEBUG_CAMERA_H
#define DEBUG_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "../../Input/Keyboard.h"
#include "../../Input/Mouse.h"
#include "../../Input/Joystick.h"
#include "../../Physics/Physics.h"

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum NavigationType {
    ALL_AXES,
    X_AND_Z_ONLY

};

class DebugCamera : public Camera {
public:
 
    DebugCamera();
    DebugCamera(glm::vec3 position, NavigationType nt = NavigationType::ALL_AXES);
    void updateCameraDirection(double dx, double dy);
    void updateCameraPos(CameraDirection direction, float dt);
    void updateCameraPosXbox(glm::vec3 dir, float dt); //The implementation of this one's different from the other one
    void updateCameraZoom(float dy);

    float getZoom();

    glm::mat4x4 getViewMatrix();

    void switchNaviationType();
    void processInput(Joystick& joystick, float dt);

private:

    void updateCameraVectors();

    NavigationType _navigationType;

    unsigned int maxRaycastDistance = 10000;
};

#endif 

