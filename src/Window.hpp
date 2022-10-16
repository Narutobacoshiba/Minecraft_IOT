#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <time.h>
#include <iostream>
#include <thread>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <World.hpp>
#include <Light.hpp>
#include <Camera.hpp>
#include <SerialPort.hpp>
#include <Utils.hpp>

constexpr auto NS_PER_SECOND = (1000000000);
constexpr auto NS_PER_MS = (1000000);

uint64_t NOW();
uint64_t MAX(uint64_t a, uint64_t b);

void handle_serial(Camera* camera);

class Window {
public:
	Window(int _height, int _width, const char* _title) : height(_height), width(_width), title(_title) {}

	void window_loop();
	void mouse_set_grabbed(bool grabbed);

	struct Mouse mouse;
	struct Keyboard keyboard;

	int height;
	int width;
	const char* title;

	uint64_t last_second;
	uint64_t frames, fps, last_frame, frame_delta;
	uint64_t ticks, tps, tick_remainder;

private:
	void window_create();

	void init();
	void destroy();
	void tick();
	void update();
	void render();

	GLFWwindow *handle;
	World* world;
	Shader shader;
	Texture texture;
	Camera camera;
	Light light;

	static void _size_callback(GLFWwindow *t_handle, int width, int height);
	static void _cursor_callback(GLFWwindow *t_handle, double xp, double yp);
	static void _key_callback(GLFWwindow *t_handle, int key, int scancode, int action, int mods);
	static void _mouse_callback(GLFWwindow *t_handle, int button, int action, int mods);
	static void button_array_tick(size_t n, struct Button *buttons);
	static void button_array_update(size_t n, struct Button *buttons);
};

#endif