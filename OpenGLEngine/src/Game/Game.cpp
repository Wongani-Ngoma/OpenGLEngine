#include "Game.h"
#include "../Engine/Editor.h"

std::vector<StaticGameObject> Game::_staticGameObjects;
std::vector<DynamicGameObject> Game::_dynamicGameObjects;

Room Game::room;
Player Game::player;
StaticGameObject Game::ground;
DynamicGameObject Game::box, Game::box2;

Joystick Game::joystick;
SkyBox Game::skyBox;

DebugCamera Game::_debugCamera;

DirLight Game::dirLight;
SpotLight Game::spotLight;
PointLight Game::pointLight;
PointLight Game::pointLight2;

Axes Game::axes;

bool Game::_debugMode;

Game::Game() {
}

void Game::init() {
    joystick = Joystick(0);
    if (joystick.isPresent()) {
        std::cout << joystick.getName() << " is present.\n";
    } else {
        std::cout << "No controller present.\n";
    }

    _debugCamera = DebugCamera(glm::vec3(0,0, -40));

    dirLight = DirLight(
        glm::vec3(0, -1, 0),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
    );
    spotLight = SpotLight(
        glm::vec3(1.0, 5.0, 0.0),
        glm::vec3(0.0, -1.0, 0.0),
        1.0, 0.08, 0.032,
        glm::cos(glm::radians(10.5)), glm::cos(glm::radians(20.0)),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
    );
    pointLight = PointLight(
        glm::vec3(5.0),
        1.0, 0.08, 0.032,
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
    );

    pointLight2 = PointLight(
        glm::vec3(-5.0),
        1.0, 0.08, 0.032,
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
    );

    axes.setup();
    player.init(glm::vec3(0.0, 20.0, 0.0));
    Scene::init(&_staticGameObjects, &_dynamicGameObjects);
    Scene::loadScene("assets/maps/createdMaps/test.json");
    //Scene::loadScene("assets/maps/Map1.json");
    skyBox = SkyBox("ThickCloudsWater");
    ground = StaticGameObject("assets/models/Ground.obj", Transformation(glm::vec3(0.0, -5.0, 0.0f), glm::vec3(0.0f)));
    //box  = DynamicGameObject("assets/models/Cube.obj", Transformation(glm::vec3(0.0f, 20.0, 0.0f), glm::vec3(0.0f)));
    //box2 = DynamicGameObject("assets/models/Cube.obj", Transformation(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f)));

    //_staticGameObjects.push_back(ground);
    //_dynamicGameObjects.push_back(box);
    //_dynamicGameObjects.push_back(box2);

    _debugMode = true;

}

void Game::setTitle(float deltaTime, float counter) {
    Camera camera;
    // slicing but i dont give a fuck
    if (_debugMode) {
        camera = _debugCamera;
    } else {
        camera = player._camera;
    }
    std::string title =
        // performance
        std::string("FPS: ") +
        std::to_string(1 / deltaTime * counter) +
        std::string(" | ") + std::to_string(deltaTime / counter * 1000) +
        std::string("ms") +
        //camera info
        std::string(" |||| Camera x: " + std::to_string(camera._cameraPos.x) + std::string(" y: ") +
            std::to_string(camera._cameraPos.y) + std::string(" z: ") +
            std::to_string(camera._cameraPos.z) + std::string(" | yaw: ") +
            std::to_string(camera.getYaw())) + std::string(" pitch: ") +
        std::to_string(camera.getPitch());

    BackEnd::setWindowTitle(title);
}

void Game::setTransformation(Shader& shader, Camera& camera) {
    //Create transformation for screen
    shader.activate();
    glm::mat4 view = glm::mat4(1.0f); // from world to camera, specific to each camera
    glm::mat4 projection = glm::mat4(1.0); // general, actually specific to each viewport
    view = camera.getViewMatrix();
    projection = glm::perspective(glm::radians(camera.getZoom()), (float)BackEnd::getScreenWidth() / (float)BackEnd::getScreenHeight(), camera._near, camera._far);
    shader.setMat4("view", view);
    shader.activate();
    shader.setMat4("projection", projection);
    shader.setFloat("near", camera._near);
    shader.setFloat("far", camera._far);
}

void Game::renderShadows(Shader& shader) {
    if (_debugMode) {
        //setTransformation(shader, _debugCamera); delete this,but am too scared
        renderGameObjects(shader, _debugCamera);
    } else {
        //setTransformation(shader, player._camera);
        renderGameObjects(shader, player._camera);
    }
}

void Game::renderPass() {
    glEnable(GL_DEPTH_TEST);
    // Back faces are those whose drawing order is CW, they'll be discarded, front faces(those drawn CCW) will be drawn
    // something is wrong here
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CW);
    // lazy gamma correction enable
    //glEnable(GL_FRAMEBUFFER_SRGB);

    if (_debugMode) {
        renderEverything(Renderer::getShaderByName("mainShader"), _debugCamera);
    } else {
        renderEverything(Renderer::getShaderByName("mainShader"), player._camera);
    }

}

void Game::updateGameObjects() {
    for (int i = 0; i < _staticGameObjects.size(); i++) {
        _staticGameObjects[i].update();
    }
    for (int i = 0; i < _dynamicGameObjects.size(); i++) {
        _dynamicGameObjects[i].update();
    }
    player.update();
}
void Game::renderEverything(Shader& shader, Camera& camera) {
    axes.render(Renderer::getShaderByName("axesShader"), camera);
    skyBox.render(Renderer::getShaderByName("skyboxShader"), camera);

    setTransformation(shader, camera);
    renderLights(shader, camera);
    renderGameObjects(shader, camera);

}
void Game::shadowRenderPass() {
    pointLight.position += glm::vec3(0.0, 0.0, glfwGetTime() * 0.001f);
    pointLight2.position += glm::vec3(0.0, 0.0, glfwGetTime() * 0.001f);
    Renderer::renderPointLightShadow(pointLight.position, _staticGameObjects, _dynamicGameObjects);
    //Renderer::renderPointLightShadow(pointLight2.position, _staticGameObjects, _dynamicGameObjects);
}

void Game::renderLights(Shader& shader, Camera camera) {
    shader.set3Float("viewPos", camera._cameraPos);
    //shader.setInt("noPointLights", 1);
    //shader.setInt("noSpotLights", 1);
    //pointLight.render(shader, 0);
    //pointLight2.render(shader, 1);
    dirLight.render(shader);
    //spotLight.render(shader, 0);
}

void Game::renderGameObjects(Shader& shader, Camera camera) {
    player.render(shader);
    Renderer::renderStaticGameObjects(_staticGameObjects, shader);
    Renderer::renderDynamicGameObjects(_dynamicGameObjects, shader);
}

void Game::processInput(float dt) {
    if (Keyboard::key(GLFW_KEY_ESCAPE) || joystick.buttonState(GLFW_JOYSTICK_START)) {
        BackEnd::setShouldWindowClose(true);
    }

    if (Keyboard::keyWentDown(GLFW_KEY_P)) {  
        _debugMode = !_debugMode; 
    }


    if (Keyboard::key(GLFW_KEY_Z)) {
        for (auto& obj : _dynamicGameObjects) {
            for (Mesh& mesh : obj.getModel()->_meshes) {
                mesh.setDrawMode(GL_LINES);
            }
        }
        for (auto& obj : _staticGameObjects) {
            for (Mesh& mesh : obj.getModel()->_meshes) {
                mesh.setDrawMode(GL_LINES);
            }
        }
    }

    if (Keyboard::key(GLFW_KEY_X)) {
        for (auto& obj : _dynamicGameObjects) {
            for (Mesh& mesh : obj.getModel()->_meshes) {
                mesh.setDrawMode(GL_TRIANGLES);
            }
        }
        for (auto& obj : _staticGameObjects) {
            for (Mesh& mesh : obj.getModel()->_meshes) {
                mesh.setDrawMode(GL_TRIANGLES);
            }
        }
    }

    if (Keyboard::keyWentDown(GLFW_KEY_PAGE_DOWN)) {
        _debugCamera.decreaseSpeed();
    }

    if (Keyboard::keyWentDown(GLFW_KEY_PAGE_UP)) {
        _debugCamera.increaseSpeed();
    }
    
    if (Keyboard::key(GLFW_KEY_T)) {
        player._gun.zShoulderMultiplier += 0.1;
    }
    if (Keyboard::key(GLFW_KEY_G)) {
        player._gun.zShoulderMultiplier -= 0.1;
    }

    if (_debugMode) {
        _debugCamera.processInput(joystick, dt);
    }
    else {
        player.processInput();
    }
}

Camera& Game::getActiveCamera() {
    if (_debugMode) {
        return _debugCamera;
    } else {
        return player._camera;
    }
}

void Game::shutdown() {

}
