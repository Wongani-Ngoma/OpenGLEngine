#include "Model.h"
#include "../Engine/Renderer.h"

Model::Model(Transformation transformation) : _transformation(transformation) {
}

Model::Model(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation) : _transformation(Transformation(pos, scale, rotation)) {
    
}

Model::~Model() {
    cleanup();
}

void Model::render(Shader& shader) {

    shader.activate();

    glm::mat4 model = _transformation.getMatrix();

    shader.setMat4("model", model);

    shader.setFloat("material.shininess", 0.5f);

    for (Mesh mesh : _meshes) {
        mesh.render(shader);
    }
}

void Model::renderBoundingBox(Shader& shader) {

}

void Model::cleanup() {
    for (Mesh mesh : _meshes) {
        mesh.cleanup();
    }
    _meshes.clear();
}

void Model::setVertexBoneDataToDefault(Vertex& vertex) {
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++) {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}

std::map<std::string, BoneInfo>& Model::GetBoneInfoMap() {
    return m_BoneInfoMap; 
}

int& Model::GetBoneCount() {
    return m_BoneCounter;
}
