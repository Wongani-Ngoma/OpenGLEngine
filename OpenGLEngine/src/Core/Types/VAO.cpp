#include"VAO.h"

// Constructor that generates a VAO id
VAO::VAO(){
}

void VAO::generate() {
	glGenVertexArrays(1, &id);
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();

	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VAO::LinkAttribI(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();

	glVertexAttribIPointer(layout, numComponents, type, stride, offset);
	glEnableVertexAttribArray(layout);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

// Binds the VAO
void VAO::Bind()
{
	glBindVertexArray(id);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &id);
}
