#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "Shader.h"

struct PointLight {
    
    glm::vec3 position;
 
    //attenuation
    float k0;
    float k1;
    float k2;

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    void render(Shader shader, int idx);  
};

struct SpotLight {

    glm::vec3 position;
    glm::vec3 direction;

    //attenuation
    float k0;
    float k1;
    float k2;

    float cutOff;
    float outerCutOff;

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    void render(Shader shader, int idx);
};

struct DirLight {

    glm::vec3 direction;

    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;

    void render(Shader shader);
};

#endif // !LIGHT_H
