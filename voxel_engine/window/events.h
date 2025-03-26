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

	static int initialize();
	static void pullEvents();
};

#endif