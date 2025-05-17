#ifndef WINDOW_H
#define WINDOW_H

class GLFWwindow;


class Window
{
private:
	static GLFWwindow* window;
	Window() = delete;
	static int width;
	static int height;
public:
	static int initialize(int width, int height, const char* title);
	static void terminate();
	static void destroyWindow();
	static bool isShouldBeClosed();
	static void setShouldClose(bool flag);
	static void swapBuffers();
	static void setCursorMode(int mode);
	static int getHeight();
	static int getWidth();
	static void setHeight(int height);
	static void setWidth(int width);
	static GLFWwindow* getWindow();
};



#endif