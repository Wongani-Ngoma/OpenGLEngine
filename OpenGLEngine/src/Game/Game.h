#pragma once
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <memory>

#include "../Input/Keyboard.h"
#include "../Input/Joystick.h"
#include "../Input/Mouse.h"

#include "Camera/DebugCamera.h"

#include "Models/SkyBox.h"
#include "Models/Axes.hpp"
#include "Models/Gun.h"
#include "Models/Room.h"
#include "../Core/Graphics/FrameBuffer.hpp"

#include "../Engine/Scene.h"
#include "../MapLoader/JsonObjectLoader.h"

#include "../Core/Graphics/Light.h"
#include "Player/Player.h"
#include "../BackEnd/BackEnd.h"
#include "../Engine/Renderer.h"
#include "../Core/Graphics/PostProcess.h"
#include "../Physics/StaticGameObject.h"
#include "GameObject.h"

class Game {
public:

    Game();
    static void init();
    static void updateGameObjects();
    static void setTitle(float deltaTime, float counter);
    static void setTransformation(Shader& shader, Camera& camera);
    static void renderShadows(Shader& shader);
    static void renderEverything(Shader& shader, Camera& camera);
    static void shadowRenderPass();
    static void renderPass();
    static void renderLights(Shader& shader, Camera camera);
    static void renderGameObjects(Shader& shader, Camera camera);
    static void processInput(float dt);

    static Camera& getActiveCamera();
    static void shutdown();

    // soem of these sould be private
    static DebugCamera _debugCamera;
    static std::vector<StaticGameObject> _staticGameObjects;
    static std::vector<DynamicGameObject> _dynamicGameObjects;

private:
    //models
    static Room room;
    static Player player;
    static StaticGameObject ground;
    static DynamicGameObject box, box2;
    static Gun gun;
    
    static Joystick joystick;
    static SkyBox skyBox;
    

    static DirLight dirLight;
    static SpotLight spotLight;
    static PointLight pointLight, pointLight2;
    static glm::mat4 lightProjection;
    static Axes axes;

    static bool _debugMode;

    static FrameBuffer fbo;

    
};

#endif // !GAME_H