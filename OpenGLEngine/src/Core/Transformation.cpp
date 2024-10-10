#include "Transformation.h"

Transformation::Transformation() {
}

Transformation::Transformation(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : position(pos), localRotation(rot) {

}

int Transformation::addAdditionalTransform(glm::mat4& t) {
    additionalTransformations.push_back(t);
    return additionalTransformations.size() - 1;
}

void Transformation::setAdditionalTransformation(int index, glm::mat4& t) {
    additionalTransformations[index] = t;
}

glm::mat4 Transformation::getMatrix() {
    glm::mat4 mat(1.0);
    mat = glm::translate(mat, position);

    for (glm::mat4& m : additionalTransformations) {
        mat *= m;
    }

    mat *= glm::mat4_cast(localRotation);
    mat = glm::scale(mat,_scale);

    return mat;
}
