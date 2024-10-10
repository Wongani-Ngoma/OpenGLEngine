#include "SkyBox.h"

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(std::string name) {

    float skyboxVertices[] = {
    -1.0, -1.0,  1.0, //      7--------6
     1.0, -1.0,  1.0, //     /|       /|
     1.0, -1.0, -1.0, //    4--------5 |
    -1.0, -1.0, -1.0, //    | |      | |
    -1.0,  1.0,  1.0, //    | 3------|-2
     1.0,  1.0,  1.0, //    |/       |/
     1.0,  1.0, -1.0, //    0--------1
    -1.0,  1.0, -1.0
    };

    unsigned int skyboxIndices[] = {
        // right
        1, 2, 6,
        6, 5, 1,
        //left
        0, 4, 7,
        7, 3, 0,
        //Top
        4, 5, 6,
        6, 7, 4,
        //bottom
        0, 3, 2,
        2, 1, 0,
        //front
        0, 1, 5,
        5, 4, 0,
        //back
        3, 7, 6,
        6, 2, 3,
    };

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    skyboxTexture = AssetManager::getCubemapTexturePtr(name.c_str());
    if (skyboxTexture == nullptr) {
        skyboxTexture = AssetManager::loadCubemapTexture(name.c_str());
        if (skyboxTexture == nullptr) {
            std::cout << "Error could not load cubemap: " << name << "\n";
        }
    }
}

SkyBox::~SkyBox() {
    cleanUp();
}

void SkyBox::render(Shader shader, Camera camera) {

    glDisable(GL_CULL_FACE); // just to be sure

    shader.activate();
    // skyboxes need lsightly different view matrices, a mat3 is creatd first then it is inserted in a mat4, the 4th
    // row in the view matrix will be 0 always, so as the camera moves, the skybox wont be translated, only rotate
    glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camera._cameraPos, camera._cameraPos + camera._cameraFront, camera._cameraUp)));
    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)Screen::SCR_WIDTH / (float)Screen::SCR_HEIGHT, camera._near, camera._far);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(skyboxVAO);
    skyboxTexture->bind();
    
    // The sky box is at the very limit, we need this to be less than or equal, or else the skybox will be discared
    // you can just completely disable depth mask
    glDepthMask(GL_FALSE);
    //glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // switch depth testing tho default gl_less
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}

void SkyBox::setCubemap(std::string newCubemap) {
    skyboxTexture->cleanUp();
    skyboxTexture->load(newCubemap);
}

void SkyBox::cleanUp()
{
}
