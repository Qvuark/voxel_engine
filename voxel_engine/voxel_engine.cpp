#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <memory>
#include <exception>
#include <iostream>
#include "window/window.h"

int WIDTH = 1200;
int HEIGHT = 800;

//void cleanUp(GLFWwindow* window)
//{
//    glfwDestroyWindow(window);
//    glfwTerminate();
//}
//void mainLoop(GLFWwindow* window)
//{
//    
//    cleanUp(window);
//}
int main() 
{
    Window::initialize(WIDTH, HEIGHT, "window");
    glViewport(0, 0, WIDTH, HEIGHT);
    while (!Window::isClosed())
    {
        glfwPollEvents();
        Window::swapBuffers();
    }
    return 0;
}