#ifndef ROOM_H
#define ROOM_H

// wtf
#include "../../Physics/StaticGameObject.h"
#include "../../Physics/StaticGameObject.h"
#include "../../Game/Camera/Camera.h"
#include "../../Input/Keyboard.h"
#include "../../Core/Graphics/Shader.h"


#include "../../Engine/AssetManager.h"

class Room {
public:
    Room();
    Room(const char* wallPath, glm::vec3 pos);
    void update();
    void render(Shader& shader);

    float roomLength = 30;
private:
    StaticGameObject walls[5];
};

#endif
