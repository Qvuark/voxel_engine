#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <memory>
#include <exception>
#include <iostream>
#include "window/window.h"
#include "window/events.h"

int WIDTH = 1200;
int HEIGHT = 800;

int main() 
{
    Window::initialize(WIDTH, HEIGHT, "window");
    Events::initialize();
    while(!Window::isShouldClosed())
    {
        Events::pullEvents();
        if (Events::jtPressed(GLFW_KEY_ESCAPE))
        {
            Window::setShouldClose(true);
        }   
        Window::swapBuffers();
    }
    Window::terminate();
    return 0;
}