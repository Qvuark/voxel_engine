#include <GL/glew.h>
#include "events.h"
#include "string.h"
#include <algorithm>
#include <iostream>

const int Events::MOUSE_BUTTONS = GLFW_KEY_LAST + 1;
const int Events::TOTAL_KEYS = MOUSE_BUTTONS + GLFW_MOUSE_BUTTON_LAST + 1;
bool* Events::_keys = nullptr;
unsigned* Events::_frames = nullptr;
unsigned Events::_currentFrame = 0;
float Events::deltaX = 0.0f;
float Events::deltaY = 0.0f;
float Events::x = 0.0f;
float Events::y = 0.0f;
bool Events::_cursorStarted = false;
bool Events::_cursorLocked = false;

const int Events::getMouseButtons() { return MOUSE_BUTTONS; }
float Events::getDeltaX() { return deltaX; }
float Events::getDeltaY() { return deltaY; }
const int Events::getTotalKeys() { return TOTAL_KEYS; }
bool Events::isCursorLocked() { return _cursorLocked; }

void Events::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (_cursorStarted)
	{
		deltaX += xpos - x;
		deltaY += ypos - y;
	}
	else
	{
		_cursorStarted = true;
	}
	x = xpos;
	y = ypos;
}
void Events::mouse_button_callback(GLFWwindow* window, int button, int action, int mode) 
{
	const int index = MOUSE_BUTTONS + button;
	if (action == GLFW_PRESS) 
	{
		_keys[index] = true;
		_frames[index] = _currentFrame;
	}
	else if (action == GLFW_RELEASE) 
	{
		_keys[index] = false;
		_frames[index] = _currentFrame;
	}
}

void Events::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if (action == GLFW_PRESS) 
	{
		_keys[key] = true;
		_frames[key] = _currentFrame;
	}
	else if (action == GLFW_RELEASE) 
	{
		_keys[key] = false;
		_frames[key] = _currentFrame;
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
	return _keys[keycode];
}
bool Events::jtPressed(int keycode)
{
	if (keycode < 0 || keycode >= MOUSE_BUTTONS)
	{
		return false;
	}
	return _keys[keycode] && _frames[keycode] == _currentFrame;
}
void Events::window_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	Window::setWidth(width);
	Window::setHeight(height);
}
int Events::initialize()
{
	GLFWwindow* window = Window::getWindow();
	_keys = new bool[TOTAL_KEYS];
	_frames = new unsigned int[TOTAL_KEYS];

	std::fill_n(_keys, TOTAL_KEYS, false);
	std::fill_n(_frames, TOTAL_KEYS, 0);

	glfwSetKeyCallback(window, [](GLFWwindow* w, int k, int s, int a, int m) {
		Events::key_callback(w, k, s, a, m);
	});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int b, int a, int m) {
		Events::mouse_button_callback(w, b, a, m);
	});
	glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
		Events::cursor_position_callback(w, x, y);
	});
	glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int wd, int h) {
		Events::window_size_callback(w, wd, h);
	});

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
