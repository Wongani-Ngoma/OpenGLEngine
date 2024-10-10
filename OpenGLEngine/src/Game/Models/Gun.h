#ifndef GUN_HPP
#define GUN_HPP

#include "../../Physics/DynamicGameObject.h"
#include "../../Game/Camera/Camera.h"
#include "../../Input/Keyboard.h"
#include "../../Core/Graphics/Shader.h"

#include "../../Engine/AssetManager.h"

class Gun  {
public:
    Gun();
    Gun(glm::vec3 pos, glm::vec3 rot, std::string path);

    void render(Shader shader, Camera camera);

    // updates the position of ADS and shoulder sight based on the current camera's position
    void updatePositions(Camera camera);
    void updatePositions(glm::vec3 position, glm::vec3 direction);

    void processInput();

    Model* model;

    float xShoulderMultiplier = 0.6f;
    float yShoulderMultiplier = -1.5f;
    float zShoulderMultiplier = 2.0;

    float xADSMultiplier = 0.0f;
    float yADSMultiplier = -1.0f;

    bool hasAimedDownSight = false;

private:

    glm::vec3 sholderPos;
    glm::vec3 aimDownSightPos;
    glm::vec3 currentPos;


    int index = -1;
};

#endif // GUN_HPP
