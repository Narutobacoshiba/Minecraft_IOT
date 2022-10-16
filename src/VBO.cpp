#include "VBO.hpp"


GLuint VBO::vbo_create()
{
	GLuint ID;
	glGenBuffers(1, &ID);
	return ID;
}

void VBO::vbo_buffer(GLuint vboID, const void* vertices, GLsizeiptr size)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::vbo_bind(GLuint vboID)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

// Unbinds the VBO
void VBO::vbo_unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::vbo_delete(GLuint vboID)
{
	glDeleteBuffers(1, &vboID);
}