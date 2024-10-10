#include"VBO.h"

VBO::VBO()
{
}

// Constructor that generates a Vertex2 Buffer Object and links it to vertices
VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

// Deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &id);
}
