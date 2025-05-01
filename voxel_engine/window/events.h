#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "window.h"
#include <GLFW/glfw3.h>

class Events
{
public:
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
	
	static int initialize();
	static void pullEvents();
	static bool clicked(int keycode);
	static bool jtClicked(int keycode);
	static bool pressed(int keycode);
	static bool jtPressed(int keycode);

	static void toggleCursor();

};

#endif