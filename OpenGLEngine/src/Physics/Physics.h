#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <PxPhysicsAPI.h>
#include <iostream>

#include "../Util.hpp"

using namespace physx;

struct DynamicBodyAndShape {
    PxRigidDynamic* body;
    PxShape* shape;
};

struct StaticBodyAndShape {
    PxRigidStatic* body;
    PxShape* shape;
};


namespace Physics {
    void init();
    void shutDown();
    PxRigidDynamic* createRigidDynamic(Transformation transformation, PxMaterial* material);
    PxRigidStatic* createRigidStatic(Transformation transformation, PxMaterial* material);

    PxRigidDynamic* createRigidDynamicFromModel(Model* model, PxMaterial* material);
    PxRigidStatic* createRigidStaticFromModel(Model* model, PxMaterial* material);

    PxBoxGeometry getBoundingBoxFromModel(Model* model);

    PxPhysics* getPhysics();
    PxScene* getScene();

    void setRigidDyanamicPosition(PxRigidDynamic* body, Transformation newTrans);
    void applyForceAtPos(PxRigidBody& body, glm::vec3 force, glm::vec3 pos);

    PxRaycastBuffer rayCast(glm::vec3 origin, glm::vec3 dir);

    void stepPhysics(float deltaTime);

   
}

#endif // !PHYSICS_H

