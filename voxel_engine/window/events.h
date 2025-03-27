#ifndef WINDOW_EVENTS_H
#define WINDOW_EVENTS_H

#include "window.h"

class Events
{
public:
	static bool* _keys;
	static unsigned* _frames;
	static unsigned _currentFrame;
	static float deltaX;
	static float deltaY;
	static float x;
	static float y;
	static bool _cursor_started;
	static bool _cursor_locked;
	
	static int initialize();
	static void pullEvents();
	static bool clicked(int keycode);
	static bool jtClicked(int keycode);
	static bool pressed(int keycode);
	static bool jtPressed(int keycode);
};

#endif