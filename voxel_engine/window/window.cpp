#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* Window::window;

int Window::initialize(int width, int height, const char* title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "failed to initialize GLEW" << std::endl;
        return -1;
    }
    glViewport(0, 0, width, height);
    return 0;
}
bool Window::shouldClosed()
{   
    return glfwWindowShouldClose(window);
}
void Window::swapBuffers()
{
    return glfwSwapBuffers(window);
}
void Window::terminate()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}