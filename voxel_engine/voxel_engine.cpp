#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <memory>
#include <exception>
#include <iostream>
#include "window/window.h"

int WIDTH = 1200;
int HEIGHT = 800;

int main() 
{
    Window::initialize(WIDTH, HEIGHT, "window");
    
    while (!Window::isClosed())
    {
        glfwPollEvents();
        Window::swapBuffers();
    }
    Window::terminate();
    return 0;
}