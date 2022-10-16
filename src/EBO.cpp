#include "EBO.hpp"


GLuint EBO::ebo_create()
{
	GLuint ID;
	glGenBuffers(1, &ID);
	return ID;
}

void EBO::ebo_buffer(GLuint eboID, const void* indices, GLsizeiptr size) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
// Binds the EBO
void EBO::ebo_bind(GLuint eboID)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
}

// Unbinds the EBO
void EBO::ebo_unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::ebo_delete(GLuint eboID)
{
	glDeleteBuffers(1, &eboID);
}