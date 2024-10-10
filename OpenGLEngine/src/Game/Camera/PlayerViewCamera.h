#pragma once
#ifndef PLAYER_VIEW_CAMERA_H
#define PLAYER_VIEW_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

enum class CameraMode {
    FIRST_PERSON,
    THIRD_PERSON
};

class PlayerViewCamera : public Camera {
public:

    PlayerViewCamera();
    PlayerViewCamera(glm::vec3 position, CameraMode mode = CameraMode::FIRST_PERSON);

    float getYaw();
    float getPitch();

    void updateCameraDirection(double dx, double dy);
    void updateCameraPos(glm::vec3 pos);
    void updateCameraVectors();
    void updateCameraZoom(float dt);

    void switchMode();

private:

    CameraMode _mode;

    float _firstPersonYaw;
    float _firstPersonPitch;
    // lacking good names, these are view attributes in third person mode
    float _thirdPersonYaw;
    float _thirdPersonPitch;

};

#endif // !PLAYER_VIEW_CAMERA_H