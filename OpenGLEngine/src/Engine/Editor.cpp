#include "Editor.h"

namespace Editor {
    PxRigidActor* _selectedActor = nullptr;

    std::vector<StaticGameObject>* _staticGameObjects;
    std::vector<DynamicGameObject>* _dynamicGameObjects;
}



void Editor::init(std::vector<StaticGameObject>* staticGameObjects, std::vector<DynamicGameObject>* dynamicGameObjects) {
    _staticGameObjects = staticGameObjects;
    _dynamicGameObjects = dynamicGameObjects;
}

PxRigidActor* Editor::selectObject(Camera& camera) {
    double d_x, d_y;
    glfwGetCursorPos(BackEnd::getWindow(), &d_x, &d_y);
    float x = (float)d_x;
    float y = (float)d_y;

    glm::vec3 dir = Util::GetMouseRay(camera.getProjectionMatrix(), camera.getViewMatrix(), BackEnd::getScreenWidth(), BackEnd::getScreenHeight(), (int)x, (int)y);

    PxRaycastBuffer hit;
    if (Physics::getScene()->raycast(Util::PxVec3FromGlmVec3(camera._cameraPos), Util::PxVec3FromGlmVec3(dir), 1000, hit)) {
        _selectedActor = hit.block.actor;
    }
    return nullptr;
}

void Editor::saveMap(std::string mapName) {
    Scene::saveEntireScene(mapName);
}

void Editor::processInput(Camera& camera) {

    if (Mouse::buttonWentDown(GLFW_MOUSE_BUTTON_LEFT)) {
        Editor::selectObject(camera);
    }
    
    // SAVE THE MAP
    if (Keyboard::key(GLFW_KEY_LEFT_CONTROL) && Keyboard::keyWentDown(GLFW_KEY_S)) {
        saveMap("assets/maps/createdMaps/test.json");
    }

    if (_selectedActor == nullptr) {
        return;
    }

    if (Keyboard::keyWentDown(GLFW_KEY_LEFT)) {
        PxTransform t = _selectedActor->getGlobalPose();
        t.p.x += 2;
        _selectedActor->setGlobalPose(t);
    }
    if (Keyboard::keyWentDown(GLFW_KEY_RIGHT)) {
        PxTransform t = _selectedActor->getGlobalPose();
        t.p.x -= 2;
        _selectedActor->setGlobalPose(t);
    }
    if (Keyboard::keyWentDown(GLFW_KEY_UP)) {
        PxTransform t = _selectedActor->getGlobalPose();
        t.p.y += 2;
        _selectedActor->setGlobalPose(t);
    }
    if (Keyboard::keyWentDown(GLFW_KEY_DOWN)) {
        PxTransform t = _selectedActor->getGlobalPose();
        t.p.y -= 2;
        _selectedActor->setGlobalPose(t);
    }
}
