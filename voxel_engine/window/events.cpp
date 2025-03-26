#include "events.h"
#include <GLFW/glfw3.h>
#include "string.h"
#include <algorithm>
#define MOUSE_BUTTONS 1024

bool* Events::_keys;
unsigned* Events::_frames;
unsigned Events::_currentFrame = 0;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Events::_keys[MOUSE_BUTTONS+button] = true;
		Events::_frames[MOUSE_BUTTONS+button] = Events::_currentFrame;
	}
	else if (action == GLFW_RELEASE)
	{
		Events::_keys[MOUSE_BUTTONS + button] = false;
		Events::_frames[MOUSE_BUTTONS + button] = Events::_currentFrame;
	}

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Events::_keys[key] = true;
		Events::_frames[key] = Events::_currentFrame;
	}
	else if(action == GLFW_RELEASE)
	{
		Events::_keys[key] = false;
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

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	return 0;
}

