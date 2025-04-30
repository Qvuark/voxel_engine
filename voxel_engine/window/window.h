#ifndef WINDOW_H
#define WINDOW_H

class GLFWwindow;

class Window
{
public:
	static GLFWwindow* window;
	Window() = delete;

	static int initialize(int width, int height, const char* title);
	static void terminate();
	static void destroyWindow();
	static bool isShouldBeClosed();
	static void setShouldClose(bool flag);
	static void swapBuffers();

};



#endif