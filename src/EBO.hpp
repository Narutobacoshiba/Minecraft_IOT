#ifndef EBO_CLASS_HPP
#define EBO_CLASS_HPP

#include <glad/glad.h>

class EBO
{
public:
	static GLuint ebo_create();

	static void ebo_buffer(GLuint eboID, const void* indices, GLsizeiptr size);
	// Binds the EBO
	static void ebo_bind(GLuint eboID);
	// Unbinds the EBO
	static void ebo_unbind();
	// Deletes the EBO
	static void ebo_delete(GLuint eboID);
};

#endif