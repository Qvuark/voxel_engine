#include <GL/glew.h>
#include "events.h"


#include "string.h"
#include <algorithm>
#include <iostream>

const int Events::MOUSE_BUTTONS = GLFW_KEY_LAST + 1;
const int Events::TOTAL_KEYS = Events::MOUSE_BUTTONS + GLFW_MOUSE_BUTTON_LAST + 1;
bool* Events::_keys = nullptr;
unsigned* Events::_frames = nullptr;
unsigned Events::_currentFrame = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::x = 0.0f;
float Events::y = 0.0f;
bool Events::_cursorStarted = false;
bool Events::_cursorLocked = false;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (Events::_cursorStarted)
	{
		Events::deltaX += xpos - Events::x;
		Events::deltaY += ypos - Events::y;
	}
	else
	{
		Events::_cursorStarted = true;	
	}
	Events::x = xpos;
	Events::y = ypos;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Events::_keys[Events::MOUSE_BUTTONS+button] = true;
		Events::_frames[Events::MOUSE_BUTTONS+button] = Events::_currentFrame;
	}
	else if (action == GLFW_RELEASE)
	{
		Events::_keys[Events::MOUSE_BUTTONS + button] = false;
		Events::_frames[Events::MOUSE_BUTTONS + button] = Events::_currentFrame;
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
bool Events::clicked(int keycode)
{
	int index = Events::MOUSE_BUTTONS + keycode;
	return _keys[index];
}
bool Events::jtClicked(int keycode)
{
	int index = Events::MOUSE_BUTTONS + keycode;
	return _keys[index] && _frames[index] == _currentFrame;
}
bool Events::pressed(int keycode)
{
	if (keycode < 0 || keycode >= Events::MOUSE_BUTTONS)
	{
		return false;
	}
	return _keys[keycode];
}
bool Events::jtPressed(int keycode)
{
	if (keycode < 0 || keycode >= Events::MOUSE_BUTTONS)
	{
		return false;
	}
	return _keys[keycode] && _frames[keycode] == _currentFrame;
}
void window_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window::width = width;
	Window::height = height;
}
int Events::initialize()
{
	GLFWwindow* window = Window::window;
	_keys = new bool[TOTAL_KEYS];
	_frames = new unsigned int[TOTAL_KEYS];

	std::fill_n(_keys, TOTAL_KEYS, false);
	std::fill_n(_frames, TOTAL_KEYS, 0);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);

	
	return 0;
}
void Events::pullEvents()
{
	_currentFrame++;
	deltaX = 0.0f;
	deltaY = 0.0f;
	glfwPollEvents();

}
void Events::toggleCursor() 
{
	_cursorLocked = !_cursorLocked;
	Window::setCursorMode(_cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

