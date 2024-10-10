#include "AssetManager.h"
#include "../BackEnd/BackEnd.h"

namespace AssetManager {
    std::vector<std::string>    _loadedModelPaths;
    std::vector<std::string>    _loadedTexture2DPaths;
    std::vector<std::string>    _loadedCubemapTexturePaths;

    std::vector<DynamicGameObject>   _DynamicGameObjectsLoaded;

    // i must be retarded
    // using queue cause i need access to these models using their address but std::vector moves stuff around as more 
    // stuff is added so the new addresses have to be obtained everytime new models are added, which i dont wantto do now 
    // maybe later, queue doesent do this. FIND ANOTHER DATA STRUCTURE THAT DOES THIS BUT IS NOT QUEQUE
    std::queue<Model>          _modelsLoaded;
    std::vector<Texture2D>      _texture2DsLoaded;
    std::vector<CubemapTexture> _cubemapTexturesLoaded;
}

Model* AssetManager::loadModel(std::string modelPath, Transformation transformation) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Could not load model at " << modelPath << std::endl << import.GetErrorString() << std::endl;
        return nullptr;
    }

    Model m(transformation);

    _modelsLoaded.push(m);
    Model* model = &_modelsLoaded.back();
    model->_directory = modelPath;
    processNode(scene->mRootNode, scene, model);
    _loadedModelPaths.push_back(modelPath);

    // return the addesss of the model just loaded
    return model;

}

void AssetManager::processNode(aiNode* node, const aiScene* scene, Model* model) { 
    // process all meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->_meshes.push_back(processMesh(mesh, scene, model));
    }

    // process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, model);
    }
}

Mesh AssetManager::processMesh(aiMesh* mesh, const aiScene* scene, Model* model) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2D*> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // seSetVertexBoneDataToDefault values
        for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
        {
            vertex.m_BoneIDs[i] = -1;
            vertex.m_Weights[i] = 0.0f;
        }

        // Position
        vertex.pos = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // Normals
        vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        // TexCoord
        // assimp mesh can have multiple textures coords

        if (mesh->mTextureCoords[0]) { // if the 0th texture coord exists, we use it
            vertex.texCoord = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } 
        else {
            vertex.texCoord = glm::vec2(0.0f); // or else we use no texture
        }
        vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    ExtractBoneWeightForVertices(vertices, mesh, scene, model);

    // Process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // The model might be have been created with noTex set to false because it's a collection of multiple textured and 
        // non-textured meshes, so, if all meshes' noTex variable were to be set to model's noTex(false), the meshes with no
        // textures will be rendered with a texture. To prevent this, it checks if the mesh has textures(by checking the 
        // count of diffuse and spec textures) and if all are zero, it means that that specific mesh should be rendered with 
        // no texture.

        if (((material->GetTextureCount(aiTextureType_DIFFUSE) == 0) &&
            (material->GetTextureCount(aiTextureType_SPECULAR)) == 0)) { // if noTex, use the materials

            model->_useTextures = false;
            aiColor4D diff(1.0);
            aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

            aiColor4D spec(1.0);
            aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

            return Mesh(vertices, indices, diff, spec); // this constructor sets _useTextures to false by default
        }
        else {
            model->_useTextures = true;

            // diffuse maps
            std::vector<Texture2D*> diffuseMaps = getTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<Texture2D*> specularMaps = getTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            return Mesh(vertices, indices, textures, true);
        }

    }
}

void AssetManager::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
    for (int i = 0; i < MAX_BONE_WEIGHTS; ++i) {
        if (vertex.m_BoneIDs[i] < 0) {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
            break;
        }
    }

}

void AssetManager::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene, Model* model) {
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (model->m_BoneInfoMap.find(boneName) == model->m_BoneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = model->m_BoneCounter;
            newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
            model->m_BoneInfoMap[boneName] = newBoneInfo;
            boneID = model->m_BoneCounter;
            model->m_BoneCounter++;
        }
        else
        {
            boneID = model->m_BoneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

std::vector<Texture2D*> AssetManager::getTextures(aiMaterial* mat, aiTextureType type) {
    std::vector<Texture2D*> textures;

    for (int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        Texture2D* texPtr;
        mat->GetTexture(type, i, &str);

        texPtr = getTexturePtr(str.C_Str());
        if (texPtr == nullptr) {
            texPtr = loadTexture(str.C_Str(), type);
            if (texPtr == nullptr) {
                std::cout << "Could not load texture: " << str.C_Str() << "\n";
                continue;
            }
        }

        textures.push_back(texPtr);
    }

    return textures;
}

Texture2D* AssetManager::getTexturePtr(const char* texturePath) {
    for (int i = 0; i < _texture2DsLoaded.size(); i++) {
        if (_texture2DsLoaded[i].path == texturePath) {
            // texture found in _loadedTexturePaths
            return &_texture2DsLoaded[i];
        }
    }
    // the texture is not loaded yet, lets try to load it
    return nullptr;

}

// might want to check if path exists before laoding
Texture2D* AssetManager::loadTexture(const char* path, aiTextureType type) {
    Texture2D& tex = _texture2DsLoaded.emplace_back();
    std::cout << "Loading texture " << path << std::endl;
    tex.load(path, type);
    if (tex.id != NULL) {
        _loadedTexture2DPaths.push_back(std::string(path));
        return &tex;
    } else {
        _texture2DsLoaded.pop_back();
        return nullptr;
    }
}

CubemapTexture* AssetManager::getCubemapTexturePtr(const char* path) {
    for (int i = 0; i < _cubemapTexturesLoaded.size(); i++) {
        if (_cubemapTexturesLoaded[i].getPath() == path) {
            // texture found in _loadedCubemapTexturePaths
            return &_cubemapTexturesLoaded[i];
        }
    }
    // the texture is not loaded yet
    return nullptr;
}

CubemapTexture* AssetManager::loadCubemapTexture(const char* path) {
    CubemapTexture& tex = _cubemapTexturesLoaded.emplace_back();
    tex.load(path);
    if (tex.getID() != NULL) {
        _loadedCubemapTexturePaths.push_back(path);
        return &tex;
    } else {
        _cubemapTexturesLoaded.pop_back();
        return nullptr;
    }
}

void AssetManager::cleanup() {
    _loadedModelPaths.clear();
    _loadedTexture2DPaths.clear();
    _loadedCubemapTexturePaths.clear();
    _DynamicGameObjectsLoaded.clear();
    bool success = _modelsLoaded.empty();
    _texture2DsLoaded.clear();
    _cubemapTexturesLoaded.clear();
}


