#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "window.h"
#include <GLFW/glfw3.h>

class Events
{
private:
	static const int MOUSE_BUTTONS;
	static const int TOTAL_KEYS;
	static bool* _keys;
	static unsigned* _frames;
	static unsigned _currentFrame;
	static float deltaX;
	static float deltaY;
	static float x;
	static float y;
	static bool _cursorStarted;
	static bool _cursorLocked;

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void window_size_callback(GLFWwindow* window, int width, int height);

public:
	static int initialize();
	static void pullEvents();
	static bool clicked(int keycode);
	static bool jtClicked(int keycode);
	static bool pressed(int keycode);
	static bool jtPressed(int keycode);
	static void toggleCursor();

	static const int getMouseButtons();
	static const int getTotalKeys();
	static float getDeltaX();
	static float getDeltaY();
	static bool isCursorLocked();
};

#endif