#pragma once
#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include <stb_image.h>

class Texture2D {
public:
    Texture2D();
    void load(std::string path, aiTextureType type);
    aiTextureType getType();
    void bind();


    std::string path;
    unsigned int id;
    GLint slot; // there are 16 texture slots, this variable tell which slot this texture is in
    aiTextureType type;

    GLenum internalFormat;
    GLenum pixelFormat;
};

#endif // !TEXTURE2D_H


