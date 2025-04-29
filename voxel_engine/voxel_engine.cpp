//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <exception>
#include <iostream>

#include "graphics/shaders.h"
#include "window/window.h"
#include "window/events.h"

int WIDTH = 1200;
int HEIGHT = 800;

float vertices[] =
{
    0.1f,0.1f,0.0f,
    -0.3f,0.0f,0.0f,
    0.0f,-0.3f,0.0f,
};

int main() 
{
    Window::initialize(WIDTH, HEIGHT, "window");
    Events::initialize();
    
    Shader* shader = loadShader("D:/DEV/c++/voxel_engine/res/main.vert", "D:/DEV/c++/voxel_engine/res/main.frag");
    if (shader == nullptr) 
    { 
        std::cerr << "couldnt load the shader";
        Window::terminate();
    }
    //create VAO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    glClearColor(0.6f, 0.62f, 0.65f, 1);
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

        //draw vao
        shader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(0);

        Window::swapBuffers();
    }
    Window::terminate();
    return 0;
}