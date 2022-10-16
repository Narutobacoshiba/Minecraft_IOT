#ifndef VAO_CLASS_HPP
#define VAO_CLASS_HPP

#include <glad/glad.h>
#include <VBO.hpp>

class VAO
{
public:
	
	static GLuint vao_create();

	// Links a VBO Attribute such as a position or color to the VAO
	static void LinkAttrib(GLuint vboID, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAO
	static void vao_bind(GLuint vaoID);
	// Unbinds the VAO
	static void vao_unbind();
	// Deletes the VAO
	static void vao_delete(GLuint vaoID);
};

#endif