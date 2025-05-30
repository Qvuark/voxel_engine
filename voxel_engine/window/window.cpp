#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* Window::window;
int Window::width = 1600;
int Window::height = 1000;

int Window::initialize(int width, int height, const char* title)
{
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) 
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) 
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, width, height);

    Window::width = width;
    Window::height = height;
    return 0;
}
bool Window::isShouldBeClosed()
{   
    return glfwWindowShouldClose(window);
}
void Window::setShouldClose(bool flag)
{
    glfwSetWindowShouldClose(window, flag);
}
void Window::swapBuffers()
{
    glfwSwapBuffers(window);
}
void Window::setCursorMode(int mode)
{
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}
void Window::destroyWindow()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}
void Window::terminate()
{
    destroyWindow();
    glfwTerminate();
}
int Window::getHeight() { return height; }
int Window::getWidth() { return width; }
void Window::setHeight(int h) { height = h; }
void Window::setWidth(int w) { width = w; }
GLFWwindow* Window::getWindow() { return window; }