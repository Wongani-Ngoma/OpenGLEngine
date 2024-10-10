#pragma once

#include "../Game/Game.h"
#include "../Physics/DynamicGameObject.h"
#include "../Physics/StaticGameObject.h"
#include "../Game/Camera/Camera.h"
#include "../BackEnd/BackEnd.h"
#include "../Util.hpp"


namespace Editor {
    void init(std::vector<StaticGameObject>* staticGameObjects, std::vector<DynamicGameObject>* dynamicGameObjects);
    PxRigidActor* selectObject(Camera& camera);
    void saveMap(std::string mapName);
    void processInput(Camera& camera);
};

