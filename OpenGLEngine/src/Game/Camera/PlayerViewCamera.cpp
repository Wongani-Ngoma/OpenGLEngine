#include "PlayerViewCamera.h"

PlayerViewCamera::PlayerViewCamera() : Camera() {
}

PlayerViewCamera::PlayerViewCamera(glm::vec3 position, CameraMode mode) : Camera(position), _mode(mode), _thirdPersonYaw(0.0f), _thirdPersonPitch(0.0f){
}

float PlayerViewCamera::getYaw() {
    if (_mode == CameraMode::FIRST_PERSON) {
        Camera::_yaw = _firstPersonYaw;
    }
    else if (_mode == CameraMode::THIRD_PERSON) {
        Camera::_yaw = _firstPersonYaw;
    }
    return Camera::getYaw();

}

float PlayerViewCamera::getPitch() {
    if (_mode == CameraMode::FIRST_PERSON) {
        Camera::_pitch = _firstPersonPitch;
    }
    else if (_mode == CameraMode::THIRD_PERSON) {
        Camera::_pitch = _firstPersonPitch;
    }
    return Camera::getPitch();

}

void PlayerViewCamera::updateCameraDirection(double dx, double dy) {
    // first update the view angles then the camera vectors, then the actual position, 
    // if we dont follow this here order shit movement will be jaggared

    if (_mode == CameraMode::FIRST_PERSON) {
        _firstPersonYaw += dx * _sensitivity;
        _firstPersonPitch += dy * _sensitivity;

        if (_firstPersonPitch > 89.0f) {
            _firstPersonPitch = 89.0f;
        }
        else if (_firstPersonPitch < -89.0f) {
            _firstPersonPitch = -89.0f;
        }
    }
    else if (_mode == CameraMode::THIRD_PERSON) {
        _thirdPersonYaw += dx * _sensitivity;
        _thirdPersonPitch += dy * _sensitivity;

        if (_thirdPersonPitch > 89.0f) {
            _thirdPersonPitch = 89.0f;
        }
        else if (_thirdPersonPitch < -89.0f) {
            _thirdPersonPitch = -89.0f;
        }
    }
    updateCameraVectors();
}

void PlayerViewCamera::updateCameraPos(glm::vec3 pos) {

    if (_mode == CameraMode::FIRST_PERSON) {
        _cameraPos = pos;
    } else if (_mode == CameraMode::THIRD_PERSON) {
        _cameraPos = pos - _cameraFront * 30.0f;
    }

}

void PlayerViewCamera::updateCameraVectors() {
    glm::vec3 direction;

    float yaw = _mode == CameraMode::FIRST_PERSON ? _firstPersonYaw : _thirdPersonYaw;
    float pitch = _mode == CameraMode::FIRST_PERSON ? _firstPersonPitch : _thirdPersonPitch;

    direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    _cameraFront = glm::normalize(direction);
    _cameraRight = glm::normalize(glm::cross(_cameraFront, _worldUp));
    _cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraFront));
}

void PlayerViewCamera::updateCameraZoom(float dy) {
    if (_zoom >= 1.0f && _zoom <= 45.0f) {
        _zoom -= dy;
    }
    else if (_zoom < 1.0f) {
        _zoom = 1.0f;
    }
    else { //zoom > 45.0f
        _zoom = 45.0f;
    }
}

void PlayerViewCamera::switchMode() {
    _mode = (_mode == CameraMode::FIRST_PERSON) ? CameraMode::THIRD_PERSON : CameraMode::FIRST_PERSON;
}
