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
    glClearColor(0, 0, 0, 1);
    while(!Window::isShouldClosed())
    {
        Events::pullEvents();
        if (Events::jtPressed(GLFW_KEY_ESCAPE))
        {
            Window::setShouldClose(true);
        }
        {
        if (Events::jtClicked(GLFW_MOUSE_BUTTON_1))
            glClearColor(1, 0, 0, 1);
        }
        if (Events::jtClicked(GLFW_MOUSE_BUTTON_2))
        {
            glClearColor(0, 1, 0, 1);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        Window::swapBuffers();
    }
    Window::terminate();
    return 0;
}