#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../BackEnd/BackEnd.h"

#include "../../Input/Keyboard.h"
#include "../../Input/Mouse.h"
#include "../../Input/Joystick.h"

class Camera {
public:
    
    Camera();
    Camera(glm::vec3 position);

    glm::mat4x4 getProjectionMatrix();
    glm::mat4x4 getViewMatrix();
    void increaseSpeed();
    void decreaseSpeed();
    float getYaw();
    float getPitch();
    float getZoom();

    glm::vec3 _cameraPos;
    glm::vec3 _cameraFront;
    glm::vec3 _cameraUp;
    glm::vec3 _cameraRight;
    glm::vec3 _worldUp;

    float _yaw;
    float _pitch;
    float _speed;
    float _zoom;
    float _near;
    float _far;

private:

protected:
    void updateCameraVectors();
    float _sensitivity;
};

#endif