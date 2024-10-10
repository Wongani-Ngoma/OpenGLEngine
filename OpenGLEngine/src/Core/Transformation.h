#ifndef  TRANSFORMATION_H
#define TRANSFORMATION_H
#pragma once

#include <vector> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Transformation {
public:
    Transformation();
    Transformation(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale = glm::vec3(1.0));

    int addAdditionalTransform(glm::mat4& t);
    void setAdditionalTransformation(int index, glm::mat4& t);

    glm::mat4 getMatrix();

    // local transformations
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 _scale = glm::vec3(1.0); // a cube with, say, a unit lenght of 1, and a scale of 20, will have sides of lenght 40, so scale is like half extents
    glm::quat localRotation = glm::vec3(0.0); // rotation on the model's center / origin

    // for example, a gun rotates around it's origin, but it also needs rotating around the player as he looks around
    std::vector<glm::mat4> additionalTransformations;

};

#endif // ! TRANSFORMATION_H