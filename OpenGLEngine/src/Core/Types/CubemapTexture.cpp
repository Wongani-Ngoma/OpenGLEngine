#include "CubemapTexture.h"


CubemapTexture::CubemapTexture(unsigned int slot) {
    CubemapTexture::slot = slot;
    CubemapTexture::id = 0;
}

// find a smarter way to do this
// this uses filesystem to look into "folder" for textures, it needs filesystem cause it doesent yet know what color
// channels the tetxures in folder have. filesystem finds and returns files in sorted order, so in order for "i" to heve
// correct values all the time, the textures in folder need to be named such that filesystem retrieves them in the
// correct order, for example, the left texture should be named "2_NightSky_Left" since it is ment to be 
// loaded when index i == 1
void CubemapTexture::load(std::string folder) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int width, height, nChannels;
    int i = -1; // this index determines where a single 2d texture is in in the cubemap
    std::string entryStr;
    for (const auto& entry : fs::directory_iterator(std::string("assets\\textures\\skybox\\" + folder))) {
        i++;
        fs::path entryPath = entry.path();
        entryStr = entryPath.string();

        unsigned char* data = stbi_load(std::string(entryStr).c_str(), &width, &height, &nChannels, 0);
        if (data) {
            stbi_set_flip_vertically_on_load(false); // opengl reads cubemaps such that the top pixel is top right, no flip needed
            GLenum format = GL_RGB;
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
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                internalFormat,
                width,
                height,
                0,
                pixelFormat,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load texture: " << entryStr << "\n";
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

std::string& CubemapTexture::getPath() {
    return path;
}

GLuint CubemapTexture::getID() {
    return id;
}

// first activate a slot, now any operations like drawing or sending textures to shaders will use the textre in the "slot"th slot
// then bind the texture to the active slot
void CubemapTexture::bind() {
    glActiveTexture(GL_TEXTURE0 + slot); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubemapTexture::cleanUp() {
    glDeleteTextures(1, &id);
}
