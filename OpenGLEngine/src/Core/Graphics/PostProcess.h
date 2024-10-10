#pragma once

#include "../../BackEnd/BackEnd.h"
#include "../../Engine/Renderer.h"
#include "FrameBuffer.hpp"

namespace PostProcess {
    void init();
    void cleanup();
    void updateFramebuffer();
    void setActiveFrameBuffer();
    void renderPostProcessedScene();
};

