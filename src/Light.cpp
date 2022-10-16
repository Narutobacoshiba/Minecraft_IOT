#ifndef LIGHT_H
#define LIGHT_H
#include "light.hpp"

Light::Light(){}
Light::Light(glm::vec3 position, glm::vec3 colour) {
	this->position = position;
	this->colour = colour;
}

void Light::uniform(Shader& shader, const char* uniform_pos, const char* uniform_co) {
	glUniform3fv(glGetUniformLocation(shader.ID, uniform_pos), 1, glm::value_ptr(this->position));
	glUniform3fv(glGetUniformLocation(shader.ID, uniform_co), 1, glm::value_ptr(this->colour));
	;
}

glm::vec3 Light::getPosition() {
	return this->position;
}

glm::vec3 Light::getColour() {
	return this->colour;
}
#endif