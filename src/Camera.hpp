#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

#include <shader.hpp>
#include <World.hpp>


struct Hittest {
	glm::vec3 key, previous;
};


struct Button {
	bool down, last, last_tick, pressed, pressed_tick;
};


struct Mouse {
	struct Button buttons[GLFW_MOUSE_BUTTON_LAST];
	glm::vec2 position, delta;
};

struct Keyboard {
	struct Button keys[GLFW_KEY_LAST];
};

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 prj_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool focus;
	bool free_move;
	// Stores the width and height of the window
	int width;
	int height;
	float player_height = 2;
	uint32_t block = 1;



	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 0.06f;
	float sensitivity = 80.0f;
	float dy = 0;
	float fov = 45.0f;

	uint32_t current_key;
	uint32_t previous_key=0;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);
	Camera();

	void collide(World* world);

	// Updates and exports the camera matrix to the Vertex Shader
	void Matrix(float nearPlane, float farPlane, Shader& shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window, Keyboard keyboard, Mouse mouse, World *world);

	void cameraTick(GLFWwindow* window, Keyboard keyboard, Mouse mouse, World *world);

	static Hittest HitTest(World* world, glm::vec3 position, glm::vec3 vector, int max_distance);
	static glm::vec3 normalize(float x, float y, float z);
};
#endif