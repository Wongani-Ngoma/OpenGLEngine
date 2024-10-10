#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Transformation.h"
#include "Mesh.h"

struct BoneInfo {
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};

class Model {
public:
    ~Model();
    Model(Transformation transformation);
    Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0));

    void render(Shader& shader);
    void renderBoundingBox(Shader& shader);
    void cleanup();
    std::vector<Vertex> getBoundingBox();
    void setRenderBoundingBox(bool value);


    // make some of these private 
    Transformation _transformation;

    std::vector<Mesh> _meshes;

    std::string _directory;
    std::string _name = "NONAME";
    static unsigned int _id;

    // This noTex variable overloads the individual meshes' noTex, for example if the meshes have textures and their noTex is 
    // true but this noTex is false, the mesh will be rendered with no texture
    bool _useTextures = true;

    std::map<std::string, BoneInfo> m_BoneInfoMap; //
    int m_BoneCounter = 0;
    
    void setVertexBoneDataToDefault(Vertex& vertex);
    std::map<std::string, BoneInfo>& GetBoneInfoMap();
    int& GetBoneCount();
};

#endif // !MODEL_H



