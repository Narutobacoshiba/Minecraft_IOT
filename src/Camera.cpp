#include "camera.hpp"

Camera::Camera(){}

Camera::Camera(int _width, int _height, glm::vec3 position)
{
	Camera::width = _width;
	Camera::height = _height;
	Position = position;
	focus = true;
	free_move = true;
}

void Camera::Matrix(float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	/*
	const float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;

	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));*/
	// Makes camera look in the right direction from the right position
	//std::cout << Orientation.x << ":" << Orientation.y << ":" << Orientation.z << "\n";
	view = glm::lookAt(Position, Position + Orientation, Up) ;

	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(this->fov), (float)width / height, nearPlane, farPlane);

	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
}



void Camera::Inputs(GLFWwindow* window, Keyboard keyboard, Mouse mouse, World *world)
{
	if (focus) {
		// Handles key inputs
		if (keyboard.keys[GLFW_KEY_ESCAPE].down) {
			focus = false;
		}
		if (keyboard.keys[GLFW_KEY_ENTER].down) {
			free_move = free_move ^ true;
		}
		if (keyboard.keys[GLFW_KEY_W].down)
		{
			this->current_key = GLFW_KEY_W;
			Position += speed * prj_Orientation;
		}
		if (keyboard.keys[GLFW_KEY_A].down)
		{
			this->current_key = GLFW_KEY_A;
			Position += speed * -glm::normalize(glm::cross(prj_Orientation, Up));
		}
		if (keyboard.keys[GLFW_KEY_S].down)
		{
			this->current_key = GLFW_KEY_S;
			Position += speed * -prj_Orientation;
		}
		if (keyboard.keys[GLFW_KEY_D].down)
		{
			this->current_key = GLFW_KEY_D;
			Position += speed * glm::normalize(glm::cross(prj_Orientation, Up));
		}
		if (keyboard.keys[GLFW_KEY_SPACE].down)
		{
			if (this->dy == 0)
				this->dy = 0.035;
			Position += speed * Up;
		}
		if (keyboard.keys[GLFW_KEY_LEFT_CONTROL].down)
		{
			Position += speed * -Up;
		}
		if (keyboard.keys[GLFW_KEY_LEFT_SHIFT].down)
		{
			speed = 0.04f;
		}
		else if (keyboard.keys[GLFW_KEY_LEFT_SHIFT].down)
		{
			speed = 0.01f;
		}
		
		if (keyboard.keys[GLFW_KEY_1].down) {
			this->block = 1;
		}
		else if (keyboard.keys[GLFW_KEY_2].down) {
			this->block = 2;
		}
		else if (keyboard.keys[GLFW_KEY_3].down) {
			this->block = 3;
		}
		else if (keyboard.keys[GLFW_KEY_4].down) {
			this->block = 4;
		}
		else if (keyboard.keys[GLFW_KEY_5].down) {
			this->block = 5;
		}
		else if (keyboard.keys[GLFW_KEY_6].down) {
			this->block = 6;
		}
		else if (keyboard.keys[GLFW_KEY_7].down) {
			this->block = 7;
		}

		if (keyboard.keys[GLFW_KEY_M].down) {
			this->fov -= 1.0f;
			if (this->fov < 10.0f)
				this->fov = 10.0f;
		}
		else if (keyboard.keys[GLFW_KEY_N].down) {
			this->fov += 1.0f;
			if (this->fov > 45.0f)
				this->fov = 45.0f;
		}
		
		this->dy -= (speed/25);
		this->dy = this->dy > -1 ? this->dy : -1;
		Position.y += this->dy;
		Position.y = Position.y > -1 ? Position.y : -1;

		collide(world);

		// Hides mouse cursor
		glfwSetInputMode(window , GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Stores the coordinates of the cursor
		double mouseX = mouse.position.x;
		double mouseY = mouse.position.y;

		
		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		if (!free_move) {
			glm::vec3 n(0.0, 1.0, 0.0);
			// Calculates projection orientation
			prj_Orientation = Orientation - (glm::dot(Orientation, n) * n);
		}
		else {
			prj_Orientation = Orientation;
		}

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else {
		if (keyboard.keys[GLFW_KEY_ESCAPE].down) {
			focus = true;
		}
	}
}

void Camera::cameraTick(GLFWwindow* window, Keyboard keyboard, Mouse mouse, World *world) {

	if (this->current_key == this->previous_key) {
		this->speed *= 1.4;
		this->speed = (this->speed > 0.04 ? 0.04 : this->speed);
	}
	else {
		this->previous_key = this->current_key;
		this->speed = 0.02;
	}
}

void Camera::collide(World* world) {
	float pad = 0.25;
	float position[3] = { this->Position.x, this->Position.y, this->Position.z };
	glm::vec3 nPosition = normalize(position[0], position[1], position[2]);
	float nposition[3] = { nPosition.x, nPosition.y, nPosition.z};

	for (size_t i = 0; i < 6; i++) {
		for (size_t t = 0; t < 3; t++) {
			if (!FACES_F[i][t])
				continue;

			float d = (position[t] - nposition[t]) *  FACES_F[i][t];
			if (d < pad)
				continue;

			for (int h = 0; h < this->player_height; h++) {
				float op[3] = { nPosition.x, nPosition.y, nPosition.z };
				op[1] -= h;
				op[t] += FACES_F[i][t];

				if (world->world_get_data(glm::vec3(op[0], op[1], op[2])) == 0)
					continue;

				position[t] -= (d - pad) * FACES_F[i][t];

				if (FACES[i] == glm::vec3(0, -1, 0) || FACES[i] == glm::vec3(0, 1, 0))
					this->dy = 0;
				break;
			}
		}
	}

	this->Position = glm::vec3(position[0], position[1], position[2]);
}

Hittest Camera::HitTest(World* world, glm::vec3 position, glm::vec3 vector, int max_distance) {
	int m = 8;
	float x = position.x;
	float y = position.y;
	float z = position.z;
	glm::vec3 cPosition = normalize(x, y, z);
	glm::vec3 cPosition2 = normalize(x, y+1, z);
	glm::vec3 previous;

	for (int i = 0; i < max_distance*m; i++) {
		glm::vec3 key = normalize(x, y, z);
		if (key != previous && previous != cPosition && previous != cPosition2 && world->world_get_data(key) != 0)
			return Hittest{ key, previous };
		
		previous = key;
		x += vector.x / m;
		y += vector.y / m;
		z += vector.z / m;
	}

	return Hittest{ glm::vec3(0,0,0), glm::vec3(0,0,0) };
}

glm::vec3 Camera::normalize(float x, float y, float z) {
	return glm::vec3(round(x), round(y), round(z));
}