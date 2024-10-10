#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "../../Physics/DynamicGameObject.h"
#include "../../Input/Keyboard.h"
#include "../Camera/PlayerViewCamera.h"

#include "../Models/Gun.h"

class Player {
public:

    Player();
    void init(glm::vec3 position);
    void update();
    void shoot();
    void jump();
    void render(Shader& shader);
    void processInput();

    DynamicGameObject& getModel();
    glm::vec3 getPosition();
    float getYaw();
    float getPitch();

    Gun _gun;

    PlayerViewCamera _camera;

private:

    glm::vec3 _position;
    glm::vec3 _offset;

    float _movementMultiplier;
    DynamicGameObject _model;

    // where the player is looking at
    float _yaw;
    float _pitch;
};

#endif // PLAYER_H