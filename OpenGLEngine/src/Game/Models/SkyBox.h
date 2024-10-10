#pragma once
#ifndef SKYBOX_H

#define SKYBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Core/Graphics/Shader.h"
#include "../../Core/Types/Texture2D.h"

#include "../../BackEnd/Screen.h"
#include "../../Game/Camera/Camera.h"
#include "../../Core/Types/CubemapTexture.h"

#include "../../Engine/AssetManager.h"

// PUT THE ENTIRE SKYBOX FUNTIONALITY IN A

class SkyBox {
public:
    SkyBox();
    SkyBox(std::string name);
    ~SkyBox();
    void setCubemap(std::string newCubemap);
    void render(Shader shader, Camera camera);
    void cleanUp();

private:
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    CubemapTexture* skyboxTexture;
};

#endif // !SKYBOX_H
