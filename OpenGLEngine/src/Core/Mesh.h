#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>
#include "Graphics/Shader.h"
#include "Types/Texture2D.h"
#include "Types/EBO.h"
#include "Types/VAO.h"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indicies;
    std::vector<Texture2D*> textures;

    VAO vao;
    
    aiColor4D diffuse;
    aiColor4D specular;


    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Texture2D*> textures = {}, bool useTextures = true);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, aiColor4D diffuse, aiColor4D spec);

    void render(Shader shader);
    void setDrawMode(GLenum newDrawMode);

    void cleanup();


private:
    GLenum drawMode = GL_TRIANGLES;

    bool _useTextures;
    VBO vbo; //Hold actual Vertex data
    EBO ebo; //Contains indicies, each of which references to a group of vertices, eg index 1 might reference to the top left position on the screen (-0.5, 0.5, 0.0)

    void setup();

};
#endif // !MESH_H



