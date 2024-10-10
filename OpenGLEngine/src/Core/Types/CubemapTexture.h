#pragma once
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <stb_image.h>
#include <filesystem>

namespace fs = std::filesystem;

class CubemapTexture {
public:
    CubemapTexture(unsigned int slot = 0);
    void load(std::string name);
    void bind();
    void cleanUp();

    std::string& getPath();
    GLuint getID();


private:
    std::string path;
    GLuint slot; // there are 16 texture slots, this variable tell which slot this texture is in
    GLuint id;

    GLenum internalFormat, pixelFormat;
};

