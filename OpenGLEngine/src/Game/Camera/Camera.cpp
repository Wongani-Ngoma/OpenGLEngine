#include "Camera.h"
#include <iostream>

Camera::Camera() {
}

Camera::Camera(glm::vec3 position) :
    _cameraPos(position),
    _worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    _yaw(-270.0f),
    _pitch(0.0f),
    _speed(2.5f),
    _zoom(45.0f),
    _sensitivity(0.2f),
    _near(0.01),
    _far(1000.0),
    _cameraFront(glm::vec3(1.0f, 0.0f, 0.0f))
{
    updateCameraVectors();
}

void Camera::decreaseSpeed() {
    _speed -= (_speed < 2 ? 0 : 2);
}

float Camera::getYaw() {
    return _yaw;
}

float Camera::getPitch() {
    return _pitch;
}

float Camera::getZoom() {
    return _zoom;
}

glm::mat4x4 Camera::getProjectionMatrix() {
    float width = (float)BackEnd::getScreenWidth();
    float height = (float)BackEnd::getScreenHeight();

    return glm::perspective(glm::radians(_zoom), width / height, _near, _far);
}

glm::mat4x4 Camera::getViewMatrix() {
    return glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
}

void Camera::increaseSpeed() {
    _speed += 2;
}

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    
    direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    direction.y = sin(glm::radians(_pitch));
    direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    _cameraFront = glm::normalize(direction);
    _cameraRight = glm::normalize(glm::cross(_cameraFront, _worldUp));
    _cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraFront));
}