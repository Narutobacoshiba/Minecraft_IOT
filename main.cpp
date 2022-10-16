#include <Window.hpp>

int main() {
	std::cout << "creat window\n";
	Window window = Window(700, 1200, "Minecraft");
	std::cout << "start loopping\n";
	window.window_loop();
}