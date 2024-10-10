#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <iostream>
#include<queue>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../AssimpGLMHelpers.hpp"

#include "../Core/Types/Texture2D.h"
#include "../Core/Types/CubemapTexture.h"
#include "../Physics/DynamicGameObject.h"
#include "../Core/Model.h"

namespace AssetManager {

    // model loading
    Model* loadModel(std::string modelPath,  Transformation transformation);
    void processNode(aiNode* node, const aiScene* scene, Model* model);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, Model* model);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices , aiMesh* mesh, const aiScene* scene, Model* model);

    std::vector<Texture2D*> getTextures(aiMaterial* mat,  aiTextureType type);
    Texture2D* getTexturePtr(const char* texturePath);
    Texture2D* loadTexture(const char* path, aiTextureType type);

    CubemapTexture* getCubemapTexturePtr(const char* path);
    CubemapTexture* loadCubemapTexture(const char* path);

    void cleanup();

}

#endif // !ASSETMANAGER_H