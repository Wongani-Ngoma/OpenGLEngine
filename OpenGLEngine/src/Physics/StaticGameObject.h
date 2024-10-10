#pragma once
#ifndef StaticGameObject_H
#define StaticGameObject_H
#include "../Engine/AssetManager.h"
#include "../Game/GameObject.h"
#include "../physics/Physics.h"

class StaticGameObject : public GameObject {
public:
    StaticGameObject();
    ~StaticGameObject();
    StaticGameObject(std::string path, Transformation t);
    StaticGameObject(std::string path, glm::vec3 pos, glm::vec3 rotation);
    void update();
    void render(Shader& shader) override;
    Model* getModel();
    unsigned int getID() const { return _id; }
    std::string getName() const { return std::string("static") + std::to_string(_id); }


private:
    PxRigidStatic* _body = NULL;
    PxShape* _shape = NULL;
    PxMaterial* _material = NULL;

    static unsigned int _staticID;
    unsigned int _id;
};

#endif // !StaticGameObject_H


