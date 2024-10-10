#include "Physics.h"

PxDefaultAllocator _defaultAllocatorCallback;
PxDefaultErrorCallback _defaulterrorCallback;
PxDefaultCpuDispatcher* _dispatcher = NULL;
PxTolerancesScale mToleranceScale;
PxFoundation* _foundation = NULL;
PxPhysics* _physics = NULL;

PxScene* _scene = NULL;
PxMaterial* mDefaultMaterial = NULL;

PxPvd* _pvd = NULL;

PxRigidStatic* groundPlane = NULL;
  
void Physics::init() {

    _foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _defaultAllocatorCallback, _defaulterrorCallback);
    if (!_foundation) {
        std::cout << "PxCreateFoundation failed\n";
        return;
    }

    _pvd = PxCreatePvd(*_foundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    _pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    _physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, PxTolerancesScale(), true, _pvd);
    if (!_physics) {
        std::cout << "PxCreatePhysics Failed\n";
        return;
    }

    PxSceneDesc sceneDesc(_physics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0, -9.81f, 0.0);
    _dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = _dispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    _scene = _physics->createScene(sceneDesc);

    physx::PxPvdSceneClient* pvdClient = _scene->getScenePvdClient();
    if (pvdClient) {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    mDefaultMaterial = _physics->createMaterial(0.5, 0.5, 0.6);

}

void Physics::shutDown() {

    PX_RELEASE(_scene);
    PX_RELEASE(_dispatcher);
    PX_RELEASE(_physics);
    if (_pvd)
    {
        PxPvdTransport* transport = _pvd->getTransport();
        PX_RELEASE(_pvd);
        PX_RELEASE(transport);
    }
    PX_RELEASE(_foundation);

}

PxRigidDynamic* Physics::createRigidDynamic(Transformation transformation, PxMaterial* material) {

    if (material == NULL) material = mDefaultMaterial;

    PxShape* shape = _physics->createShape(physx::PxBoxGeometry(Util::PxVec3FromGlmVec3(transformation._scale)), *material);
    PxRigidDynamic* body = _physics->createRigidDynamic(Util::PxTransFromModelTrans(transformation));
    PxRigidBodyExt::updateMassAndInertia(*body, 1.0);
    body->attachShape(*shape);
    _scene->addActor(*body);

    shape->release();

    return body;

}

PxRigidStatic* Physics::createRigidStatic(Transformation transformation, PxMaterial* material) {

    if (material == NULL) material = mDefaultMaterial;

    PxShape* shape = _physics->createShape(physx::PxBoxGeometry(Util::PxVec3FromGlmVec3(transformation._scale)), *material);
    PxRigidStatic* body = _physics->createRigidStatic(Util::PxTransFromModelTrans(transformation));
    body->attachShape(*shape);
    _scene->addActor(*body);

    shape->release();

    return body;
}

PxRigidDynamic* Physics::createRigidDynamicFromModel(Model* model, PxMaterial* material) {
    if (material == NULL) material = mDefaultMaterial;

    PxShape* shape = _physics->createShape(getBoundingBoxFromModel(model), *material);
    PxRigidDynamic* body = _physics->createRigidDynamic(Util::PxTransFromModelTrans(model->_transformation));
    PxRigidBodyExt::updateMassAndInertia(*body, 1.0);
    body->attachShape(*shape);
    _scene->addActor(*body);

    shape->release();

    return body;
}

PxRigidStatic* Physics::createRigidStaticFromModel(Model* model, PxMaterial* material) {
    if (material == NULL) material = mDefaultMaterial;

    //PxShape* shape = _physics->createShape(physx::PxBoxGeometry(Util::PxVec3FromGlmVec3(transformation.scale)), *material);
    PxShape* shape = _physics->createShape(getBoundingBoxFromModel((model)), *material);
    PxRigidStatic* body = _physics->createRigidStatic(Util::PxTransFromModelTrans(model->_transformation));
    body->attachShape(*shape);
    _scene->addActor(*body);

    shape->release();

    return body;
}

PxBoxGeometry Physics::getBoundingBoxFromModel(Model* model) {
    // change these to something more robust
    glm::vec3 min(100000.0);
    glm::vec3 max(-100000.0);

    for (int i = 0; i < model->_meshes.size(); i++) {
        for (int e = 0; e < model->_meshes[i].vertices.size(); e++) {
            max.x = std::max(max.x, model->_meshes[i].vertices[e].pos.x);
            max.y = std::max(max.y, model->_meshes[i].vertices[e].pos.y);
            max.z = std::max(max.z, model->_meshes[i].vertices[e].pos.z);

         min.x = std::min(min.x, model->_meshes[i].vertices[e].pos.x);
            min.y = std::min(min.y, model->_meshes[i].vertices[e].pos.y);
            min.z = std::min(min.z, model->_meshes[i].vertices[e].pos.z);
        }
    }
    // if scales look fucked in the render, dont fuck with these, re-export the models in blender and try again :p
    PxBoxGeometry box(PxVec3(
        (max.x - min.x) * 0.5f,
        (max.y - min.y) * 0.5f,
        (max.z - min.z) * 0.5f)
    );

    return box;
}

PxPhysics* Physics::getPhysics() {
    return _physics;
}

PxScene* Physics::getScene() {
    return _scene;
}

void Physics::setRigidDyanamicPosition(PxRigidDynamic* body, Transformation newTrans) {
    //body->se(Util::PxTransFromModelTrans(newTrans));
}

void Physics::applyForceAtPos(PxRigidBody& body, glm::vec3 force, glm::vec3 pos) {
    body.addForce(Util::PxVec3FromGlmVec3(force));
}

PxRaycastBuffer Physics::rayCast(glm::vec3 origin, glm::vec3 dir) {
    PxRaycastBuffer hit;

    _scene->raycast(Util::PxVec3FromGlmVec3(origin), Util::PxVec3FromGlmVec3(dir), 1000, hit);

    return hit;
}

void Physics::stepPhysics(float deltaTime) {
    _scene->simulate(deltaTime);
    _scene->fetchResults(true);
}

