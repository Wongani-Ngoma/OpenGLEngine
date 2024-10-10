#include "Mesh.h"
#include "../BackEnd/BackEnd.h"

std::vector<struct Vertex> Vertex::genList(float* vertices, int novertices) {
    std::vector<Vertex> ret(novertices);

    int stride = sizeof(Vertex) / sizeof(float);

    for (int i = 0; i < novertices; i++) {
        ret[i].pos = glm::vec3(
            vertices[i * stride + 0],
            vertices[i * stride + 1],
            vertices[i * stride + 2]
        );
        ret[i].normal = glm::vec3(
            vertices[i * stride + 3],
            vertices[i * stride + 4],
            vertices[i * stride + 5]
        );
        ret[i].texCoord = glm::vec2(
            vertices[i * stride + 6],
            vertices[i * stride + 7]
        );
    }

    return ret;
}

Mesh::Mesh() {
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Texture2D*> textures, bool useTextures)
    : vertices(vertices), indicies(indicies), textures(textures), _useTextures(useTextures) {
      setup();

}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, aiColor4D diffuse, aiColor4D specular) 
    : vertices(vertices), indicies(indicies), textures(textures), diffuse(diffuse), specular(specular), _useTextures(false) {
    setup();

}

void Mesh::render(Shader shader) {
    
    if (_useTextures) {
        //textures
        int diffuseIdx = 0;
        int specularIdx = 0;
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);

            // retrieve texture info
            std::string name;
            switch (textures[i]->getType()) {
            case aiTextureType_DIFFUSE:
                name = "diffuse" + std::to_string(diffuseIdx++);
                break;
            case aiTextureType_SPECULAR:
                name = "specular" + std::to_string(specularIdx++);
                break;
            }
            //set shader value
            shader.setInt(name, i);
            //bind
            textures[i]->bind();
        }
        shader.setInt("noTex", 0);
    }
    else {
        //materials
        shader.set4Float("material.diffuse", diffuse);
        shader.set4Float("material.specular", specular);
        shader.setInt("noTex", 1);
    }

    vao.Bind();
    glDrawElements(drawMode, indicies.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //glActiveTexture(0);
}

void Mesh::setDrawMode(GLenum newDrawMode) {
    drawMode = newDrawMode;
}

void Mesh::cleanup() {
    vao.Delete();
    vbo.Delete();
    ebo.Delete();

}

void Mesh::setup() {

    vao.generate();
    vao.Bind();

    vbo = VBO(vertices);
    ebo = EBO(indicies);

    //Set the attribute pointers
    //Vertex.pos
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    //Vertex.Normal
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //Vertex.texCoord
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    // ids
    vao.LinkAttribI(vbo, 3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    // weights
    vao.LinkAttrib(vbo, 4, 4,GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

}

