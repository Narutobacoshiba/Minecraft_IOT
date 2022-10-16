#include "Window.hpp"


uint64_t NOW() {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return ((ts.tv_sec * NS_PER_SECOND) + ts.tv_nsec);
}

uint64_t MAX(uint64_t a, uint64_t b) {
	return a > b ? a : b;
}


void Window::button_array_tick(size_t n, struct Button *buttons) {
	for (size_t i = 0; i < n; i++) {
		buttons[i].pressed_tick = buttons[i].down && !buttons[i].last_tick;
		buttons[i].last_tick = buttons[i].down;
	}
}

void Window::button_array_update(size_t n, struct Button *buttons) {
	for (size_t i = 0; i < n; i++) {
		buttons[i].pressed = buttons[i].down && !buttons[i].last;
		buttons[i].last = buttons[i].down;
	}
}

void Window::_size_callback(GLFWwindow *t_handle, int width, int height) {
	Window *handle = static_cast<Window *>(glfwGetWindowUserPointer(t_handle));
	glViewport(0, 0, width, height);

	handle->width = width;
	handle->height = height;
}

void Window::_cursor_callback(GLFWwindow *t_handle, double xp, double yp) {
	Window *handle = static_cast<Window *>(glfwGetWindowUserPointer(t_handle));
		glm::vec2 p(xp, yp);
	handle->mouse.position = p;
}

void Window::_key_callback(GLFWwindow *t_handle, int key, int scancode, int action, int mods) {
	if (key < 0) {
		return;
	}

	Window *handle = static_cast<Window *>(glfwGetWindowUserPointer(t_handle));
	switch (action)
	{
	case GLFW_PRESS:
		handle->keyboard.keys[key].down = true;
		break;
	case GLFW_RELEASE:
		handle->keyboard.keys[key].down = false;
		break;
	default:
		break;
	}

}

void Window::_mouse_callback(GLFWwindow *t_handle, int button, int action, int mods) {
	if (button < 0) {
		return;
	}

	Window *handle = static_cast<Window *>(glfwGetWindowUserPointer(t_handle));
	switch (action)
	{
	case GLFW_PRESS:
		handle->mouse.buttons[button].down = true;
		break;
	case GLFW_RELEASE:
		handle->mouse.buttons[button].down = false;
		break;
	default:
		break;
	}

}

void Window::window_create() {

	this->last_frame = NOW();
	this->last_second = NOW();

	if (!glfwInit()) {
		std::cout << "error initializing GLFW\n";
		exit(1);
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	this->handle = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);

	if (this->handle == NULL) {
		std::cout << "error creating window\n";
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(this->handle);

	// configure callbacks
	glfwSetWindowUserPointer(this->handle, this);
	glfwSetFramebufferSizeCallback(this->handle, this->_size_callback);
	glfwSetCursorPosCallback(this->handle, this->_cursor_callback);
	glfwSetKeyCallback(this->handle, this->_key_callback);
	glfwSetMouseButtonCallback(this->handle, this->_mouse_callback);

	// Introduce the window into the current context
	glfwMakeContextCurrent(this->handle);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
}


void Window::init() {

	Shader shaderProgram("./res/shaders/basic.vs", "./res/shaders/basic.fs");
	this->shader = shaderProgram;

	this->world = new World();
	this->world->world_init(&this->shader);

	std::string texPath = "C:\\Users\\hapha\\source\\repos\\Game3D\\Game3D\\res\\image\\";
	Texture cubeTex((texPath + "blocks.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	cubeTex.texUnit(shaderProgram, "tex0", 0);
	this->texture = cubeTex;

	Camera camera(this->width, this->height, glm::vec3(floorf(WORLD_SIZE.x/2), 20.0f, floorf(WORLD_SIZE.z/2)));
	this->camera = camera;

	Light light(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	this->light = light;

	glEnable(GL_DEPTH_TEST);
}

void Window::destroy() {

	this->world->world_destroy();
	delete this->world;

	this->texture.Delete();
	this->shader.Delete();

	glfwDestroyWindow(this->handle);
	glfwTerminate();
}

void Window::tick() {
	this->ticks++;
	button_array_tick(GLFW_MOUSE_BUTTON_LAST, this->mouse.buttons);
	button_array_tick(GLFW_KEY_LAST, this->keyboard.keys);
	// camera tick
	this->camera.cameraTick(this->handle, this->keyboard, this->mouse, this->world);
}

void Window::update() {
	button_array_update(GLFW_MOUSE_BUTTON_LAST, this->mouse.buttons);
	button_array_update(GLFW_KEY_LAST, this->keyboard.keys);
	this->world->world_update(this->camera.Position);
}

void Window::render() {
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->shader.Activate();

	// Handles camera inputs
	this->camera.Inputs(this->handle, this->keyboard, this->mouse, this->world);

	this->camera.free_move = true;

	// Updates and exports the camera matrix to the Vertex Shader
	this->camera.Matrix(0.1f, 400.0f, this->shader, "camMatrix");

	this->texture.Bind();


	this->light.uniform(this->shader, "lightPosition", "lightColour");

	this->world->world_render();

	this->frames++;
}

bool is_threading = true;

void Window::window_loop() {

	this->window_create();
	this->init();

	std::thread serial_handler(handle_serial, &this->camera);
	//serial_handler.join();

	while (!glfwWindowShouldClose(this->handle)) {
		uint64_t now = NOW();
		
		this->frame_delta = now - this->last_frame;
		this->last_frame = now;

		if (now - this->last_second > NS_PER_SECOND) {
			this->fps = this->frames;
			this->tps = this->ticks;
			this->frames = 0;
			this->ticks = 0;
			this->last_second = now;

			std::cout << "FPS: " << this->fps << " | TPS: " << this->tps << "\n";
		}

		

		const uint64_t NS_PER_TICK = (NS_PER_SECOND / 60);
		uint64_t tick_time = this->frame_delta + this->tick_remainder;
		while (tick_time > NS_PER_TICK) {
			this->tick();
			tick_time -= NS_PER_TICK;
		}
		this->tick_remainder = MAX(tick_time, 0);

		this->update();
		this->render();

		glfwSwapBuffers(this->handle);
		glfwPollEvents();
	}

	is_threading = false;
	Sleep(100);
	this->destroy();
	exit(0);
}

void Window::mouse_set_grabbed(bool grabbed) {
	glfwSetInputMode(this->handle, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void handle_serial(Camera* camera) {
	std::cout << "start thread\n";
	char port[] = "\\.\\com3";

	char output[MAX_DATA_LENGTH];
	char temp[MAX_DATA_LENGTH];

	SerialPort arduino(port);
	if (arduino.isConnected()) {
		std::cout << "Connection made" << std::endl << std::endl;
	}
	else {
		std::cout << "Error in port name" << std::endl << std::endl;
	}

	char pr_count = '-';
	glm::vec3 n(0.0, 1.0, 0.0);
	uint64_t pre_time = NOW();
	char pre_key = '-';

	while (arduino.isConnected() && is_threading) {
		int bytes = arduino.readSerialPort(output, MAX_DATA_LENGTH);
		
		if (output[0] == 'r' && output[1] != pr_count) {
			pr_count = output[1];
			glm::vec3 cur_orientation = camera->Orientation;

			for (int i = 2; i < bytes; i++) {
				temp[i-2] = output[i];
			}
			
			std::vector<std::string> els = split(temp, " ");

			if (els.size() > 1) {
				float ne_y = std::atof(els[0].c_str());
				if (ne_y > 0)
					cur_orientation.y += 0.05;
				else if (ne_y < 0)
					cur_orientation.y -= 0.05;
				if (cur_orientation.y < -0.99)
					cur_orientation.y = -0.99;
				else if (cur_orientation.y > 0.99)
					cur_orientation.y = 0.99;


				float ne_x = std::atof(els[1].c_str());
				if (ne_x < 0)
					cur_orientation.x += 0.1;
				else if (ne_x > 0)
					cur_orientation.x -= 0.1;

				if (cur_orientation.x > 0.99) {
					cur_orientation.x = 0.99;
				}
				else if (cur_orientation.x < -0.99) {
					cur_orientation.x = -0.99;
				}

				std::cout << "y: " << ne_y << "x: " << ne_x << "\n";
				camera->Orientation = glm::vec3(cur_orientation.x, cur_orientation.y, cur_orientation.z);
				camera->prj_Orientation = camera->Orientation - (glm::dot(camera->Orientation, n) * n);
			
			}
		}
		else if (output[0] == 'f') {
			for (int i = 1; i < bytes; i++)
				temp[i - 1] = output[i];
			int a = std::stoi(temp);
			if (a >= 10 && a <= 45)
				camera->fov = a;
		}
		else if (output[0] == 'k') {
			if (output[1] == '1') {

			}
			else if (output[1] == '2') {
				if (pre_key == '2') {
					uint64_t curr_time = NOW();
					if (curr_time - pre_time > NS_PER_SECOND / 100) {
						pre_time = curr_time;
						camera->Position += camera->speed * camera->prj_Orientation;
					}
				}
				else {
					pre_key = '2';
				}
			}
			else if (output[1] == '3') {

			}
			else if (output[1] == '4') {
				if (pre_key == '4') {
					uint64_t curr_time = NOW();
					if (curr_time - pre_time > NS_PER_SECOND / 100) {
						pre_time = curr_time;
						camera->Position += camera->speed * -glm::normalize(glm::cross(camera->prj_Orientation, camera->Up));
					}
				}
				else {
					pre_key = '4';
				}
			}
			else if (output[1] == '5') {
				if (pre_key == '5') {
					uint64_t curr_time = NOW();
					if (curr_time - pre_time > NS_PER_SECOND / 100) {
						pre_time = curr_time;
						if (camera->dy == 0)
							camera->dy = 0.035;
						camera->Position += camera->speed * camera->Up;
					}
				}
				else {
					pre_key = '5';
				}
			}
			else if (output[1] == '6') {
				if (pre_key == '6') {
					uint64_t curr_time = NOW();
					if (curr_time - pre_time > NS_PER_SECOND / 100) {
						pre_time = curr_time;
						camera->Position += camera->speed * glm::normalize(glm::cross(camera->prj_Orientation, camera->Up));
					}
				}
				else {
					pre_key = '6';
				}
			}
			else if (output[1] == '7') {

			}
			else if (output[1] == '8') {
				if (pre_key == '8') {
					uint64_t curr_time = NOW();
					if (curr_time - pre_time > NS_PER_SECOND / 100) {
						pre_time = curr_time;
						camera->Position += camera->speed * -camera->prj_Orientation;
					}
				}
				else {
					pre_key = '8';
				}
			}
			else if (output[1] == '9') {

			}
		}

		/*
		std::vector<std::string> list_els = split(output, " ");
		if (list_els[0] == "m") {
			if (list_els.size() >= 2) {
				std::vector<std::string> acc_els = split(list_els[1], ":");
				if (acc_els.size() >= 3) {
					glm::vec3 cur_orientation = camera->Orientation;
					//std::cout << acc_els[1] << "\n";
					ne_y = std::atof(acc_els[1].c_str());
					if (ne_y - pre_y < 0)
						cur_orientation.y -= 0.05;
					else if(ne_y -pre_y > 0)
						cur_orientation.y += 0.05;

					camera->Orientation = glm::vec3(cur_orientation.x, cur_orientation.y, cur_orientation.z);
					
					if(pre_y != ne_y)
						pre_y = ne_y;
				}
			}
		}*/
		/*
		int out_int = std::stoi(output);

		if (out_int != camera->fov && out_int >= 10 && out_int <= 45) {
			camera->fov = (float)out_int;
		}*/
	}

	std::cout << "stop thread\n";
}