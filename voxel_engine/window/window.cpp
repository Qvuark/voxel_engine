#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* Window::window;

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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
    glfwSwapInterval(1);

    return 0;
}
bool Window::isShouldClosed()
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