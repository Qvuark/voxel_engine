#define GLEW_STATIC
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <memory>
#include <exception>
#include <iostream>

#include "graphics/shaders.h"
#include "window/window.h"
#include "window/events.h"

int WIDTH = 1200;
int HEIGHT = 800;

int main() 
{
    Window::initialize(WIDTH, HEIGHT, "window");
    Events::initialize();
    
    glClearColor(0.6f, 0.62f, 0.65f, 1);
    Shader* shader = loadShader("D:/DEV/c++/voxel_engine/res/main.vert", "D:/DEV/c++/voxel_engine/res/main.frag");
    if (shader == nullptr) { std::cerr << "couldnt load the shader"; }

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