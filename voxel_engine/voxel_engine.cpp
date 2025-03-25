#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <memory>
#include <exception>
#include <iostream>
int WIDTH = 1300;
int HEIGHT = 1000;


void cleanUp(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
GLFWwindow* initializeWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "voxel_engine", nullptr, nullptr);
    if (window==nullptr)
    {
        cleanUp(window);
        throw std::runtime_error("Window creation failed");
    }
    glfwMakeContextCurrent(window);
    return window;
}
void mainLoop(GLFWwindow* window)
{
    glViewport(0, 0, WIDTH, HEIGHT);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    cleanUp(window);
}
int main() 
{
    try
    {
        GLFWwindow* window = initializeWindow();
        mainLoop(window);

    }
    catch(std::runtime_error e)
    {
        std::cerr << e.what();
    }
    return 0;
}