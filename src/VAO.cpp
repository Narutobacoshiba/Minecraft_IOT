#include "VAO.hpp"

// Constructor that generates a VAO ID
GLuint VAO::vao_create(){
	GLuint ID;
	glGenVertexArrays(1, &ID);
	return ID;
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::LinkAttrib(GLuint vboID, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO::vbo_bind(vboID);
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO::vbo_unbind();
}

// Binds the VAO
void VAO::vao_bind(GLuint vaoID)
{
	glBindVertexArray(vaoID);
}

// Unbinds the VAO
void VAO::vao_unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::vao_delete(GLuint vaoID)
{
	glDeleteVertexArrays(1, &vaoID);
}