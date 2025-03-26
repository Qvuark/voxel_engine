#include "events.h"
#include <GLFW/glfw3.h>
#include "string.h"
#include <algorithm>


bool* Events::_keys;
unsigned* Events::_frames;
unsigned Events::_currentFrame = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Events::_keys[key] = true;
		Events::_frames[key] = Events::_currentFrame;
	}
}
int Events::initialize()
{
	GLFWwindow* window = Window::window;
	_keys = new bool[1032];
	_frames = new unsigned int[1032];

	std::fill_n(_keys, 1032, false);
	std::fill_n(_frames, 1032, 0);
	return 0;
}

