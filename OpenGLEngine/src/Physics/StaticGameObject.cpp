#include "StaticGameObject.h"

unsigned int StaticGameObject::_staticID = 0;


StaticGameObject::StaticGameObject() : GameObject() {
}

StaticGameObject::~StaticGameObject() {
}

StaticGameObject::StaticGameObject(std::string path, Transformation t) {
    _id = ++_staticID;

    _model = AssetManager::loadModel(path, t);
    _model->_transformation = t;
    //_body = Physics::createRigidStatic(Transformation(pos, scale, rotation), _material);
    _body = Physics::createRigidStaticFromModel(_model, _material);
}

StaticGameObject::StaticGameObject(std::string path, glm::vec3 pos, glm::vec3 rotation) : GameObject() {
    _id = ++_staticID;

    _model = AssetManager::loadModel(path, Transformation(pos, rotation));
    _model->_transformation = Transformation(pos, rotation);
    //_body = Physics::createRigidStatic(Transformation(pos, rotation), _material);
    _body = Physics::createRigidStaticFromModel(_model, _material);
}

void StaticGameObject::update() {
    _model->_transformation.position = Util::GlmVec3FromPxVec3(_body->getGlobalPose().p);
    _model->_transformation.localRotation = Util::GlmQuatFromPxQuat(_body->getGlobalPose().q);
}

void StaticGameObject::render(Shader& shader) {
    _model->render(shader);
}

Model* StaticGameObject::getModel() {
    return _model;
}
