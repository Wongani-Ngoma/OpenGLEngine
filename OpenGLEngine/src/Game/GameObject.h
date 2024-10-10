#pragma once

#include "../Core/Model.h"

class GameObject {
public:
    GameObject();
    virtual ~GameObject() = default;
    virtual void render(Shader& shader);
    void update();

protected:
    Model* _model = NULL;
};

