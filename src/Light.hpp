#ifndef LIGHT_CLASS_HPP
#define LIGHT_CLASS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.hpp>

class Light {
private:
	glm::vec3 position;
	glm::vec3 colour;


public:

	Light(glm::vec3 position, glm::vec3 colour);
	Light();

	void uniform(Shader& shader, const char* uniform_pos, const char* uniform_co);

	glm::vec3 getPosition();

	glm::vec3  getColour();

	void setPosition(glm::vec3 position);

	void setColour(glm::vec3 colour);
};

#endif