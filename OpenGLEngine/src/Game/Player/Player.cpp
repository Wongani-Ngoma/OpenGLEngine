#include "Player.h"

Player::Player() {
    
}

void Player::init(glm::vec3 position) {
    _model._material = Physics::getPhysics()->createMaterial(1.0, 0.3, 0); // static, dynamic, restitution
    _model = DynamicGameObject("assets/models/Human.obj", Transformation(position, glm::vec3(1.0), glm::vec3(1.0)));
    _offset = glm::vec3(0.0, 7.0, 0.0);
    _movementMultiplier = 15.0f;
    _camera = PlayerViewCamera(position + _offset, CameraMode::FIRST_PERSON);

    _gun = Gun(glm::vec3(0.0), glm::vec3(0.0), "assets/models/guns/shotgun.obj");

}

void Player::update() {
    _model.update();
    _position = _model.getModel()->_transformation.position;
    _camera.updateCameraPos(_position + _offset);

    glm::vec3 direction, front, right, up;
    direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    direction.y = sin(glm::radians(_pitch));
    direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    _gun.updatePositions(_position, direction);

}

void Player::shoot() {
    //Physics::rayCast(player.getCamera()._cameraPos, player.getCamera()._)
}

void Player::jump()
{
}

void Player::render(Shader& shader) {
    _model.getModel()->render(shader);
    _gun.render(shader, _camera);
}

void Player::processInput() {
    if (Keyboard::key(GLFW_KEY_W)) {
        _model.getPxRigidDynamic()->addForce(Util::PxVec3FromGlmVec3(_camera._cameraFront * _movementMultiplier));
    }
    if (Keyboard::key(GLFW_KEY_S)) {
        _model.getPxRigidDynamic()->addForce(Util::PxVec3FromGlmVec3(-_camera._cameraFront * _movementMultiplier));
    }
    if (Keyboard::key(GLFW_KEY_A)) {
        _model.getPxRigidDynamic()->addForce(Util::PxVec3FromGlmVec3(-_camera._cameraRight * _movementMultiplier));
    }
    if (Keyboard::key(GLFW_KEY_D)) {
        _model.getPxRigidDynamic()->addForce(Util::PxVec3FromGlmVec3(_camera._cameraRight * _movementMultiplier));
    }

    if (Keyboard::keyWentDown(GLFW_KEY_C)) {
        _camera.switchMode();
    }

    //Mouse for direction
    double dx = Mouse::getDX(), dy = Mouse::getDY();
    _yaw += dx;
    _pitch += dy;

    if (_pitch > 89.0f) {
        _pitch = 89.0f;
    }
    else if (_pitch < -89.0f) {
        _pitch = -89.0f;
    }

    if ((dx != 0) || (dy != 0)) {
        _camera.updateCameraDirection(dx, dy); // this calls updatecamera vectors, which is expensive as fuck,put this bastard in porcessInput, calling it only when there's mouse movement
    }

    double scrollDy = Mouse::getScrollDY();
    if (scrollDy != 0) {
        _camera.updateCameraZoom(scrollDy);
    }

    _gun.processInput();
    
}

DynamicGameObject& Player::getModel() {
    return _model;
}

glm::vec3 Player::getPosition() {
    return _model.getModel()->_transformation.position;
}

float Player::getYaw() {
    return _yaw;
}

float Player::getPitch() {
    return _pitch;
}
