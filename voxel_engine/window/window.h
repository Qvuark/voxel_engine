#ifndef WINDOW_H
#define WINDOW_H

class GLFWwindow;

class Window
{
private:
	static GLFWwindow* window;
	Window() = delete;
public:
	static int initialize(int width, int height, const char* title);
	static void terminate();
	static bool isClosed();
	static void swapBuffers();

};



#endif