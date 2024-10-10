#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#define MAX_BONE_INFLUENCE 4
#define MAX_BONE_WEIGHTS 4

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

// Structure to standardize the vertices used in the meshes
struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];

	static std::vector<struct Vertex> genList(float* vertices, int novertices);
};

class VBO
{
public:
	// Reference id of the Vertex Buffer Object
	GLuint id;
	VBO();
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(std::vector<Vertex>& vertices);

	// Binds the VBO
	void Bind();
	// Deletes the VBO
	void Delete();
};

#endif