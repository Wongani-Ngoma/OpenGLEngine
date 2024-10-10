#include "Gun.h"

Gun::Gun() {
}

Gun::Gun(glm::vec3 pos, glm::vec3 rot, std::string path) {
    hasAimedDownSight = false;
    // pos doesent really matter in this case
    model = AssetManager::loadModel(path, Transformation(pos, rot));

}

void Gun::render(Shader shader, Camera camera) {
    glm::mat4 transformation = glm::mat4(1.0);
    updatePositions(camera); // we update here cause if we dont there is noticible lag in gun movement

    // TRANSLATION
    transformation = glm::translate(transformation, currentPos);

    // ROTATION
    // rotation due to pitch - rotate around cameraRight using dot product
    float theta = acos(glm::dot(camera._worldUp, camera._cameraFront) /
        glm::length(camera._cameraUp) / glm::length(camera._cameraFront));
    transformation = glm::rotate(transformation, atanf(1) * 2 - theta, camera._cameraRight); // offset by pi/2 radians because angle between cameraFront and gunFront

    // rotation due to yaw - rotate around cameraUp using dot product
    glm::vec2 front2D = glm::vec2(camera._cameraFront.x, camera._cameraFront.z);
    theta = acos(glm::dot(glm::vec2(1.0, 0.0), front2D) / glm::length(front2D));
    transformation = glm::rotate(transformation, (camera._cameraFront.z < 0) ? theta : -theta, camera._worldUp);

    // you can do better ...
    if (index == -1) { // index is -1 so this transformation asnt been created in the model's additionalTransormatins yet
        index = model->_transformation.addAdditionalTransform(transformation); 
    }
    else { // index is not -1, the transformation was created earier, let's just update it
        model->_transformation.setAdditionalTransformation(index, transformation);
    }

    model->render(shader);

}

void Gun::updatePositions(Camera camera) {

    sholderPos = camera._cameraPos +
        glm::vec3(camera._cameraFront * 3.0f) +
        glm::vec3(camera._cameraUp * yShoulderMultiplier) +
        glm::vec3(camera._cameraRight * xShoulderMultiplier);

    aimDownSightPos = camera._cameraPos +
        glm::vec3(camera._cameraFront * 3.0f) +
        glm::vec3(camera._cameraUp * yADSMultiplier) +
        glm::vec3(camera._cameraRight * xADSMultiplier);
        
    currentPos = hasAimedDownSight ? aimDownSightPos : sholderPos;
}

void Gun::updatePositions(glm::vec3 position, glm::vec3 direction) {
    glm::vec3 front, right, up;

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, up));
    up = glm::normalize(glm::cross(right, front));

    sholderPos = position +
        glm::vec3(front * zShoulderMultiplier) +
        glm::vec3(up * yShoulderMultiplier) +
        glm::vec3(right * xShoulderMultiplier);

    aimDownSightPos = position +
        glm::vec3(front * 3.0f) +
        glm::vec3(up * yADSMultiplier) +
        glm::vec3(right * xADSMultiplier);

    currentPos = hasAimedDownSight ? aimDownSightPos : sholderPos;

}

void Gun::processInput() {
    if (Keyboard::keyWentDown(GLFW_KEY_TAB)) {
        if (hasAimedDownSight) {
            currentPos = sholderPos;
            hasAimedDownSight = false;
        }
        else {
            currentPos = aimDownSightPos;
            hasAimedDownSight = true;
        }
    }

    // ADJUSTING GUN POSITION, FOR DEBUGGING

    
    if (hasAimedDownSight) {
        if (Keyboard::key(GLFW_KEY_D)) {
            xADSMultiplier += 0.01;
        }
        if (Keyboard::key(GLFW_KEY_A)) {
            xADSMultiplier -= 0.01;
        }
        if (Keyboard::key(GLFW_KEY_W)) {
            yADSMultiplier += 0.01;
        }
        if (Keyboard::key(GLFW_KEY_S)) {
            yADSMultiplier -= 0.01;
        }
    } else {
        if (Keyboard::key(GLFW_KEY_D)) {
            xShoulderMultiplier += 0.01;
        }
        if (Keyboard::key(GLFW_KEY_A)) {
            xShoulderMultiplier -= 0.01;
        }
        if (Keyboard::key(GLFW_KEY_W)) {
            yShoulderMultiplier += 0.01;
        }
        if (Keyboard::key(GLFW_KEY_S)) {
            yShoulderMultiplier -= 0.01;
        }
        
    }
    
}
