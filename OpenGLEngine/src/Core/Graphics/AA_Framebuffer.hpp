#pragma once
#include "../io/Screen.h"
#include<glm/glm.hpp>
#include<glad/glad.h>
#include <iostream>
#include <vector>

int samples = 2;

struct AA_ColorAttachment {
    const char* name;
    GLuint handle;
    GLenum internalFormat;
};

struct AA_DepthAttachment {
    GLuint handle;
    GLenum internalFormat = GL_DEPTH_COMPONENT;
};

struct AA_DepthStencilAttachment {
    GLuint handle;
    GLenum internalFormat = GL_DEPTH24_STENCIL8;
};

struct AAFrameBuffer {
private:
    const char* name = " ";
    GLuint handle = 0;
    std::vector<AA_ColorAttachment> colorAttachments;
    AA_ColorAttachment currentColorAttachment;
    AA_DepthAttachment depthAttachment;
    AA_DepthStencilAttachment depthStencilAttachment;
    unsigned int _width, _height;

public:

    void create(unsigned int width, unsigned int height) {
        glGenFramebuffers(1, &handle);
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        _width = width;
        _height = height;

    }

    void createColorAttachment(const char* name, GLenum format) {
        GLenum slot = GL_COLOR_ATTACHMENT0 + colorAttachments.size();
        int err = glGetError();
        AA_ColorAttachment& colorAttachment = colorAttachments.emplace_back();
        err = glGetError();
        colorAttachment.name = name;
        err = glGetError();
        colorAttachment.internalFormat = format;
        err = glGetError();
        glGenTextures(1, &colorAttachment.handle);
        err = glGetError();
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorAttachment.handle);
        err = glGetError();
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, _width, _height, GL_TRUE);
        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_2D_MULTISAMPLE, colorAttachment.handle, 0);
        err = glGetError();

    }

    void createDepthAttachment() {
        depthAttachment.internalFormat = GL_DEPTH_COMPONENT;
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        glGenTextures(1, &depthAttachment.handle);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthAttachment.handle);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, depthAttachment.internalFormat, _width, _height, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthAttachment.handle, 0);
    }

    void createDepthStencilAttachment() {
        depthStencilAttachment.internalFormat = GL_DEPTH24_STENCIL8;
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        glGenTextures(1, &depthStencilAttachment.handle);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthAttachment.handle);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_DEPTH24_STENCIL8, _width, _height, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthStencilAttachment.handle, 0);
    }

    void checkErrors() {
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer error: " << fboStatus << "\n";
        }
    }

    void Bind(int flags) {
        glBindFramebuffer(flags, handle);
    }

    void bindColorAttachment(const char* name) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, GetColorAttachmentHandleByName(name));
    }

    void setViewPort() {
        glViewport(0, 0, _width, _height);
    }

    GLuint GetDepthAttachmentHandle() {
        return depthAttachment.handle;
    }

    GLuint GetColorAttachmentHandleByName(const char* name) {
        for (int i = 0; i < colorAttachments.size(); i++) {
            if (std::strcmp(name, colorAttachments[i].name) == 0) {
                return colorAttachments[i].handle;
            }
        }
        std::cout << "GetColorAttachmentHandleByName() with name '" << name << "' failed. Name does not exist in FrameBuffer '" << this->name << "'\n";
        return GL_NONE;
    }

    GLenum GetColorAttachmentSlotByName(const char* name) {
        for (int i = 0; i < colorAttachments.size(); i++) {
            if (std::strcmp(name, colorAttachments[i].name)) {
                return GL_COLOR_ATTACHMENT0 + i;
            }
        }
        std::cout << "GetColorAttachmentHandleByName() with name '" << name << "' failed. Name does not exist in FrameBuffer '" << this->name << "'\n";
        return GL_INVALID_VALUE;
    }

    const char* getName() {
        return name;
    }

    GLuint getHandle() {
        return handle;
    }

    void cleanUp() {
        colorAttachments.clear();
        glDeleteFramebuffers(1, &handle);
    }
};