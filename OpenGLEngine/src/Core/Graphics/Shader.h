#pragma once
#ifndef  SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
    Shader();
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath);
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    void generate(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath);
    void generate(const char* vertexShaderPath, const char* fragmentShaderPath);
    void activate();
    std::string loadShaderSrc(const char* filename);
    GLuint compileShader(const char* filepath, GLenum type);

    void setMat4(std::string name, glm::mat4 val);
    void setInt(std::string name, int val);
    void setFloat(std::string name, float val);
    void set3Float(std::string name, glm::vec3 v);
    void set3Float(std::string name, float v1, float v2, float v3);
    void set4Float(std::string name, aiColor4D color);
    void set4Float(std::string name, glm::vec4 v);
    void set4Float(std::string name, float v1, float v2, float v3, float v4);

private:
    unsigned int id;
    const char* _name = "";
};

#endif