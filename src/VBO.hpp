#ifndef VBO_CLASS_HPP
#define VBO_CLASS_HPP

#include <glad/glad.h>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	static GLuint vbo_create();

	static void vbo_buffer(GLuint vboID, const void* vertices, GLsizeiptr size);
	// Binds the VBO
	static void vbo_bind(GLuint vboID);
	// Unbinds the VBO
	static void vbo_unbind();
	// Deletes the VBO
	static void vbo_delete(GLuint vboID);
};

#endif