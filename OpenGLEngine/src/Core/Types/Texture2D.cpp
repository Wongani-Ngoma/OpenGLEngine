#include "Texture2D.h"
#include <iostream>

Texture2D::Texture2D() {

    // default values
    Texture2D::slot = 0;
    Texture2D::id = 0;
    Texture2D::type = aiTextureType_DIFFUSE;
}

void Texture2D::load(std::string path, aiTextureType type) {

    Texture2D::path = path;
    Texture2D::type = type; 

    glGenTextures(1, &id);

    int width, height, nChannels, err;
    //std::string p = (dir + "/" + path); // use this when the .obj contains relative texture directories
    std::string p = (path); // use path only cause blender stores absolute texture directories in exported .obj files,
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(p.c_str(), &width, &height, &nChannels, 0);

    if (nChannels == 4) {
        pixelFormat = GL_RGBA;
        internalFormat = GL_SRGB_ALPHA; // use this when using gamma correction 
        //internalFormat = GL_RGBA; // use this when not using gamma correction
    }

    if (nChannels == 3) {
        pixelFormat = GL_RGB; // most common
        internalFormat = GL_SRGB; // use this when using gamma correction 
        //internalFormat = GL_RGB; // use this when not using gamma correction 
    }

    if (nChannels == 1) {
        pixelFormat = GL_RED;
        internalFormat = GL_SRGB; // use this when using gamma correction 
        //internalFormat = GL_RGB; // use this when not using gamma correction 
    }
    if (data) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    }
    else {
        std::cout << "ERROR: IMAGE " << p << " NOT LOADED" << "\n";
    }
    stbi_image_free(data);
}

aiTextureType Texture2D::getType() {
    return type;
}

// first activate a slot, now any operations like drawing or sending textures to shaders will use the textre in the "slot"th slot
// then bind the texture to the active slot
void Texture2D::bind() {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}
