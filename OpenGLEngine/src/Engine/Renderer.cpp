    #include "Renderer.h"
#include "../Physics/DynamicGameObject.h"
#include "../Physics/StaticGameObject.h"

namespace Renderer {
    std::map<std::string, Shader> shaders;

    // shadowy shit
    unsigned int shadowMapWidth = 1024, shadowMapHeight = 1024;
    unsigned int shadowMapFBO;
    unsigned int shadowMap;

    unsigned int pointShadowMapFBO;
    unsigned int depthCubemap;

    glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(10.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightProjection = orthgonalProjection * lightView;

    float farPlane = 100.0;
}

void Renderer::init() {
    compileAndSetupShaders();
    initShadowBuffers();
}

void Renderer::compileAndSetupShaders() {
    //Shaders
    shaders = {
        {"mainShader", Shader("assets/shaders/Object.vert", "assets/shaders/Object.frag")},
        {"shadowMapShader", Shader("assets/shaders/ShadowMap.vert", "assets/shaders/ShadowMap.frag")},
        {"shadowCubeMapShader", Shader("assets/shaders/ShadowCubeMap.vert", "assets/shaders/ShadowCubeMap.frag", "assets/shaders/ShadowCubeMap.geom")},
        {"framebufferShader", Shader("assets/shaders/Framebuffer.vert", "assets/shaders/Framebuffer.frag")},
        {"axesShader", Shader("assets/shaders/AxesShader.vert", "assets/shaders/AxesShader.frag")},
        {"skyboxShader", Shader("assets/shaders/Skybox.vert", "assets/shaders/Skybox.frag")}
    };

    shaders["skyboxShader"].activate();
    shaders["skyboxShader"].setInt("skybox", 0);

    shaders["framebufferShader"].activate();
    shaders["framebufferShader"].setInt("screenTexture", 0); // framebuffer will use the 0th texture
    shaders["framebufferShader"].setFloat("gamma", 2.2); // gamma value should be the same as that in framebuffer object
}

void Renderer::initShadowBuffers() {
    // for directional light shadows
    glGenFramebuffers(1, &shadowMapFBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float clampColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(10.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightProjection = orthgonalProjection * lightView;

    shaders["shadowMapShader"].activate();
    shaders["shadowMapShader"].setMat4("lightProjection", lightProjection);


    // for spotlight shadows
    glm::mat4 perspectiveProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 75.0f);
    glm::mat4 spotLightView = glm::lookAt(glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.0), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 spotLightProjection = perspectiveProjection * spotLightView;

    // for pointlight shadows
    glGenFramebuffers(1, &pointShadowMapFBO);
    glGenTextures(1, &depthCubemap);

    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::vec3 pointLightPos(5.0);

    //matrices for all 6 faces
    glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 75.0f);
    glm::mat4 shadowTransforms[] = {
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)),
    };

    Shader& shadowCubeMapShader = getShaderByName("shadowCubeMapShader");
    shadowCubeMapShader.activate();
    for (int i = 0; i < 6; i++) {
        shadowCubeMapShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    shadowCubeMapShader.set3Float("lightPos", pointLightPos);
    shadowCubeMapShader.setFloat("farPlane", 1000.0);

    
}

void Renderer::renderPass(Shader& shader) {
    
}

void Renderer::renderPointLightShadow(glm::vec3 pointLightPos, std::vector<StaticGameObject>& staticGameObjects, std::vector<DynamicGameObject>& dynamicGameObjects) {
    //matrices for all 6 faces
    glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
    glm::mat4 shadowTransforms[] = {
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
        shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)),
    };
    shaders["shadowCubeMapShader"].activate();
    for (int i = 0; i < 6; i++) {
        shaders["shadowCubeMapShader"].setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    shaders["shadowCubeMapShader"].set3Float("lightPos", pointLightPos);
    shaders["shadowCubeMapShader"].setFloat("farPlane", 75.0);

    renderStaticGameObjectShadows(staticGameObjects);
    renderDynamicGameObjectShadows(dynamicGameObjects);

}

void Renderer::renderStaticGameObjects(std::vector<StaticGameObject>& staticGameObjects, Shader& shader) {
    
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, BackEnd::getScreenWidth(), BackEnd::getScreenHeight());
    // set the framebuffer in PostProvess.cpp to be the active one so that all opengl calls should operate on
    // that framebuffer
    //PostProcess::setActiveFrameBuffer();

    for (int i = 0; i < staticGameObjects.size(); i++) {
        staticGameObjects[i].render(shader);
    }
}

void Renderer::renderDynamicGameObjects(std::vector<DynamicGameObject>& dynamicGameObjects, Shader& shader) {
    for (int i = 0; i < dynamicGameObjects.size(); i++) {
        dynamicGameObjects[i].render(shader);
    }
}

void Renderer::renderStaticGameObjectShadows(std::vector<StaticGameObject>& staticGameObjects) {

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < staticGameObjects.size(); i++) {
        staticGameObjects[i].render(shaders["shadowCubeMapShader"]);
    }

    /*
    Shader& mainShader = Renderer::getShaderByName("mainShader");
    mainShader.activate();
    mainShader.setMat4("lightProjection", lightProjection);
    mainShader.setFloat("farPlane", farPlane);
    */

    /*
    glActiveTexture(GL_TEXTURE0 + 2); // Bind to third uint cause the first two are taken by diffuse and specular maps  
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    Renderer::getMainShader().setInt("shadowMap", 2);
    */

    /*
    glActiveTexture(GL_TEXTURE0 + 2); // Bind to third uint cause the first two are taken by diffuse and specular maps  
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    mainShader.setInt("shadowCubeMap", 2);
    */
}

void Renderer::renderDynamicGameObjectShadows(std::vector<DynamicGameObject>& dynamicGameObjects) {
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < dynamicGameObjects.size(); i++) {
        dynamicGameObjects[i].render(shaders["shadowCubeMapShader"]);
    }

    /*
    Shader& mainShader = Renderer::getShaderByName("mainShader");
    mainShader.activate();
    mainShader.setMat4("lightProjection", lightProjection);
    mainShader.setFloat("farPlane", farPlane);
    */

    /*
    glActiveTexture(GL_TEXTURE0 + 2); // Bind to third uint cause the first two are taken by diffuse and specular maps  
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    Renderer::getMainShader().setInt("shadowMap", 2);
    */

    /*
    glActiveTexture(GL_TEXTURE0 + 2); // Bind to third uint cause the first two are taken by diffuse and specular maps  
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    mainShader.setInt("shadowCubeMap", 2);
    */
}

Shader& Renderer::getShaderByName(const char* name) {
    
    auto iter = shaders.begin();
    for (; iter != shaders.end(); iter++) {
        if ((*iter).first == name) {
            return (*iter).second;
        }
    }
    std::cout << "Renderer::getShaderByName() Coud not find shader: " << name << "\n";
    
}

unsigned int Renderer::getCubeMap() {
    return depthCubemap;
}

unsigned int Renderer::getFBO() {
    return pointShadowMapFBO;
}
