#pragma once

#include <memory>
#include <vector>
#include <map>

#include "../BackEnd/BackEnd.h"
#include "../Game/GameObject.h"

#include "../Core/Graphics/PostProcess.h"
#include "../Core/Graphics/Shader.h"

#include "../Physics/DynamicGameObject.h"
#include "../Physics/StaticGameObject.h"


namespace Renderer {
    void init();
    void compileAndSetupShaders();
    void initShadowBuffers();
    void renderPass(Shader& shader);
    void renderPointLightShadow(glm::vec3 pointLightPos, std::vector<StaticGameObject>& staticGameObjects, std::vector<DynamicGameObject>& dynamicGameObjects);
    void renderShadowsToDepthMap();
    void renderStaticGameObjectShadows(std::vector<StaticGameObject>& staticGameObjects);
    void renderStaticGameObjects(std::vector<StaticGameObject>& staticGameObjects, Shader& shader);
    void renderDynamicGameObjectShadows(std::vector<DynamicGameObject>& dynamicGameObjects);
    void renderDynamicGameObjects(std::vector<DynamicGameObject>& dynamicGameObjects, Shader& shader);

    Shader& getShaderByName(const char* name);

    unsigned int getCubeMap();
    unsigned int getFBO();
};

