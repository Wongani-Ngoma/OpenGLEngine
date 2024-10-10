#ifndef EBO_H
#define EBO_H

#include<glad/glad.h>
#include<vector>

class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint id;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO();
	EBO(std::vector<GLuint>& indices);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};

#endif