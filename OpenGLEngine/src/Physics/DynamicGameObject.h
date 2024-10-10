#pragma once
#ifndef DynamicGameObject_H
#define DynamicGameObject_H

#include "../Engine/AssetManager.h"
#include "../Game/GameObject.h"
#include "../physics/Physics.h"

class DynamicGameObject : public GameObject {

public:
    DynamicGameObject();
    ~DynamicGameObject();
    DynamicGameObject(std::string path, Transformation transformation);

    PxRigidDynamic* getPxRigidDynamic();
    Model* getModel();
    unsigned int getID() const { return _id;}
    void render(Shader& shader) override;
    void update();
    void addForce(glm::vec3 dir, float strength);
    std::string getName() const { return std::string("dynamic") + std::to_string(_id); }

    PxMaterial* _material = NULL;

private:
    PxRigidDynamic* _body = NULL;
    PxShape* _shape = NULL;

    static unsigned int _staticID;
    unsigned int _id;
};

#endif

