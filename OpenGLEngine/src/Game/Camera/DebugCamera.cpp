#include "DebugCamera.h"

DebugCamera::DebugCamera() : Camera() {
}

DebugCamera::DebugCamera(glm::vec3 position, NavigationType nt) : Camera(position), _navigationType(nt) {
}

void DebugCamera::updateCameraDirection(double dx, double dy) {
    _yaw += dx;
    _pitch += dy;

    if (_pitch > 89.0f) {
        _pitch = 89.0f;
    }
    else if (_pitch < -89.0f) {
        _pitch = -89.0f;
    }
    updateCameraVectors();
}

void DebugCamera::updateCameraPos(CameraDirection direction, float dt) {
    float velocity = (float)dt * _speed;

    glm::vec3 directionToMoveTo;
    if (_navigationType == ALL_AXES) { // we can move wherever cameraFront is pointing to, even up or down
        directionToMoveTo = _cameraFront;
    }
    else if (_navigationType == X_AND_Z_ONLY) {
        directionToMoveTo = glm::vec3(_cameraFront.x, 0.0, _cameraFront.z);
        //normalize cause when the player cf.x & cf.y are both > 0, the directionToMoveTo vector becomes slightly more that unit
        directionToMoveTo = glm::normalize(directionToMoveTo);
    }

    switch (direction) {
    case CameraDirection::FORWARD:
        _cameraPos += directionToMoveTo * velocity;
        break;
    case CameraDirection::BACKWARD:
        _cameraPos -= directionToMoveTo * velocity;
        break;
    case CameraDirection::RIGHT:
        _cameraPos += _cameraRight * velocity;
        break;
    case CameraDirection::LEFT:
        _cameraPos -= _cameraRight * velocity;
        break;
    case CameraDirection::UP:
        _cameraPos += _worldUp * velocity; //using world up insted of cameraup means when you jump you go up relative to the world, not relative to your direction
        break;
    case CameraDirection::DOWN:
        _cameraPos -= _worldUp * velocity;
        break;
    }
}

void DebugCamera::updateCameraPosXbox(glm::vec3 dir, float dt) {
    float velocity = (float)_speed * dt;
    _cameraPos += (glm::vec3(dir.x, dir.y, dir.z) * velocity);
}

void DebugCamera::updateCameraZoom(float dy) {
    if (_zoom >= 1.0f && _zoom <= 180.0f) {
        _zoom -= dy;
    }
    else if (_zoom < 1.0f) {
        _zoom = 1.0f;
    }
    else { //zoom > 45.0f
        _zoom = 45.0f;
    }
}

float DebugCamera::getZoom()
{
    return _zoom;
}

glm::mat4x4 DebugCamera::getViewMatrix() {
    return glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
}

void DebugCamera::switchNaviationType() {
    _navigationType = (_navigationType == NavigationType::ALL_AXES) ? NavigationType::X_AND_Z_ONLY : NavigationType::ALL_AXES;
}

void DebugCamera::processInput(Joystick& joystick, float dt) {
    // move camera
    if (Keyboard::key(GLFW_KEY_UP)) {
        updateCameraPos(CameraDirection::FORWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_DOWN)) {
        updateCameraPos(CameraDirection::BACKWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_LEFT)) {
        updateCameraPos(CameraDirection::LEFT, dt);
    }
    if (Keyboard::key(GLFW_KEY_RIGHT)) {
        updateCameraPos(CameraDirection::RIGHT, dt);
    } 

    // teleport
    if (Keyboard::key(GLFW_KEY_SPACE)) {
        PxRaycastBuffer hit;
        if (Physics::getScene()->raycast(Util::PxVec3FromGlmVec3(_cameraPos), Util::PxVec3FromGlmVec3(_cameraFront), maxRaycastDistance, hit)) {
            _cameraPos = Util::GlmVec3FromPxVec3(hit.block.position);
        }

    }

    if (Keyboard::keyWentDown(GLFW_KEY_P)) {
        _speed -= 5;
    }

    if (Keyboard::keyWentDown(GLFW_KEY_O)) {
        _speed -= 5;
    }

    // select navigation type 
    if (Keyboard::keyWentDown(GLFW_KEY_CAPS_LOCK)) {
        switchNaviationType();
    }

    //keyboard for direction
    /*if (Keyboard::key(GLFW_KEY_W)) {
        updateCameraDirection(0.0, 0.25);
    }
    if (Keyboard::key(GLFW_KEY_S)) {
        updateCameraDirection(0.0, -0.25);
    }
    if (Keyboard::key(GLFW_KEY_A)) {
        updateCameraDirection(-0.25, 0.0);
    }
    if (Keyboard::key(GLFW_KEY_D)) {
        updateCameraDirection(0.25, 0.0);
    }
    */

    //Mouse for direction
    double dx = Mouse::getDX(), dy = Mouse::getDY();

    if (dx != 0 || dy != 0) {
        updateCameraDirection(dx * _sensitivity, dy * _sensitivity);
    }
    double scrollDy = Mouse::getScrollDY();
    if (scrollDy != 0) {
        updateCameraZoom(scrollDy);
    }

    //Joystick for direction
    dx = joystick.axesState(GLFW_JOYSTICK_AXES_RIGHT_STICK_X);
    dy = joystick.axesState(GLFW_JOYSTICK_AXES_RIGHT_STICK_Y);
    if ((dx != 0) || (dy != 0)) {
        updateCameraDirection(dx * _sensitivity, -dy * _sensitivity);
    }

    //Joystick for camera position
    dx = joystick.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
    dy = joystick.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);
    if (dx != 0 || dy != 0) {
        updateCameraPosXbox(glm::vec3(dx, 0.0f, dy), dt);
    }
}

void DebugCamera::updateCameraVectors() {
    glm::vec3 direction(0.0);
    direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    direction.y = sin(glm::radians(_pitch));
    direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    _cameraFront = glm::normalize(direction);
    _cameraRight = glm::normalize(glm::cross(_cameraFront, _worldUp));
    _cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraFront));
}
