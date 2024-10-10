/*
#ifndef MODELARRAY_HPP
#define MODELARRAY_HPP

#include "../../Physics/Rigidbody.h"
#include "../Model.h"

template <class T>
class ModelArray {
public:
    std::vector<Rigidbody> instances;

    void init() {
        model.init();
    }

    void render(Shader shader, float dt) {
        for (Rigidbody& rb : instances) {
            rb.update(dt);
            model.rigidBody.position = rb.position;
            model.render(shader, dt); 
        }


    }

    void setSize(glm::vec3 size) {
        model.size = size;
    }

    void cleanup() {
        model.cleanup();
    }

protected:
    T model;
};

#endif // MODELARRAY_HPP
*/