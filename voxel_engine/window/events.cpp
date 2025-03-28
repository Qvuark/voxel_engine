#include "events.h"
#include <GLFW/glfw3.h>
#include "string.h"
#include <algorithm>
#define MOUSE_BUTTONS 1024

bool* Events::_keys;
unsigned* Events::_frames;
unsigned Events::_currentFrame = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::x = 0.0f;
float Events::y = 0.0f;
bool Events::_cursor_started = false;
bool Events::_cursor_locked = false;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (Events::_cursor_locked)
	{
		Events::deltaX += xpos - Events::x;
		Events::deltaY += ypos - Events::y;
	}
	else
	{
		Events::_cursor_started = true;
	}
	Events::x = xpos;
	Events::y = ypos;
}
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
	bool Events::clicked(int keycode)
	{
		int index = MOUSE_BUTTONS + keycode;
		return _keys[index];
	}
	bool Events::jtClicked(int keycode)
	{

		int index = MOUSE_BUTTONS + keycode;
		return _keys[index] && _frames[index] == _currentFrame;
	}
	bool Events::pressed(int keycode)
	{
		if (keycode < 0 || keycode >= MOUSE_BUTTONS)
		{
			return false;
		}
		else
		{
			return _keys[keycode];
		}
	}
	bool Events::jtPressed(int keycode)
	{
		if (keycode < 0 || keycode >= MOUSE_BUTTONS)
		{
			return false;
		}
		else
		{
			return _keys[keycode] && _frames[keycode] == _currentFrame;
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
	glfwSetCursorPosCallback(window, cursor_position_callback);
	return 0;
}
void Events::pullEvents()
{
	_currentFrame++;
	deltaX = 0.0f;
	deltaY = 0.0f;
	glfwPollEvents();
}

