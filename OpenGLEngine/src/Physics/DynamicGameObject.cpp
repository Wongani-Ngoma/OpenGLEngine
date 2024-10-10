#include "DynamicGameObject.h"

unsigned int DynamicGameObject::_staticID = 0;

DynamicGameObject::DynamicGameObject() : GameObject() {
}

DynamicGameObject::~DynamicGameObject() {
}

DynamicGameObject::DynamicGameObject(std::string path, Transformation transformation) : GameObject() {
    _id = ++_staticID;
    _model = AssetManager::loadModel(path, transformation);
    //_body = Physics::createRigidDynamic( transformation, _material);
    _body = Physics::createRigidDynamicFromModel(_model, _material);

}

PxRigidDynamic* DynamicGameObject::getPxRigidDynamic() {
    return _body;
}

Model* DynamicGameObject::getModel() {
    return _model;
}

void DynamicGameObject::render(Shader& shader) {
    _model->render(shader);
}

void DynamicGameObject::update() {
    _model->_transformation.position = Util::GlmVec3FromPxVec3(_body->getGlobalPose().p);
    _model->_transformation.localRotation = Util::GlmQuatFromPxQuat(_body->getGlobalPose().q);
}


void DynamicGameObject::addForce(glm::vec3 dir, float strength) {
        _body->addForce(Util::PxVec3FromGlmVec3(dir) * strength);
}
