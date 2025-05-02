
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <memory>
#include <exception>
#include <iostream>

#include "graphics/mesh.h"
#include "window/camera.h"
#include "loaders/png_loading.h"
#include "graphics/textures.h"
#include "graphics/shaders.h"
#include "window/window.h"
#include "window/events.h"
#include "voxels/chunk.h"
#include "voxels/voxels.h"

std::vector<float> vertices =
{
    //x   y    z     u      v
   -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
    1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};
std::vector<int> attributes =
{
    3,2,0
};

int main() 
{
    int currentWidth = Window::width;  
    int currentHeight = Window::height;

    Window::initialize(currentWidth, currentHeight, "window");
    Events::initialize();
    
    Shader* shader = loadShader("D:/DEV/c++/voxel_engine/res/main.vert", "D:/DEV/c++/voxel_engine/res/main.frag");
    if (shader == nullptr) 
    { 
        std::cerr << "couldnt load the shader"<<std::endl;
        Window::terminate();
        return 1;
    }

    Texture* texture = loadTexture("D:/DEV/c++/voxel_engine/res/block.png");
    if (texture == nullptr)
    {
        std::cerr << "couldnt load the texture" << std::endl;
        Window::terminate();
        return 1;
    }
    //create VAO


    glClearColor(0.6f, 0.62f, 0.65f, 1);

    Chunk* chunk = new Chunk();
    Mesh* mesh = new Mesh(vertices,attributes, 6);
    Camera* camera = new Camera(vec3(0,0,1),radians(70.0f));
    mat4 model(1.0f);

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float sensitivityX = 0.004f;
    float sensitivityY = 0.0025f;

    float yaw = 0.0f;
    float pitch = 0.0f;

    while(!Window::isShouldBeClosed())
    {
        Events::pullEvents();


        float currentTime = glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        if (Events::jtPressed(GLFW_KEY_ESCAPE))
        {
            Window::setShouldClose(true);
        }
        if (Events::jtPressed(GLFW_KEY_TAB))
        {
            Events::toggleCursor();
        }
        if (Events::pressed(GLFW_KEY_W))
        {
            camera->pos += camera->front * delta;
        }
        if (Events::pressed(GLFW_KEY_A))
        {
            camera->pos -= camera->right * delta;
        }
        if (Events::pressed(GLFW_KEY_S))
        {
            camera->pos -= camera->front * delta;;
        }
        if (Events::pressed(GLFW_KEY_D))
        {
            camera->pos += camera->right * delta;;
        }

        float pitchDelta = -Events::deltaY * sensitivityY;
        float yawDelta = -Events::deltaX * sensitivityX;
        static float pitch = 0.0f;
        pitch += pitchDelta;
        pitch = clamp(pitch, -89.0f, 89.0f); 

        camera->rotate(pitchDelta, yawDelta);

        glClear(GL_COLOR_BUFFER_BIT);

        //draw vao
        shader->use();
        shader->uniformMatrix("model", model);
        shader->uniformMatrix("projview", camera->getPerspective() * camera->getView());

        texture->bind();
        mesh->drawPrimitive(GL_TRIANGLES);

        Window::swapBuffers();
    }
    delete shader;
    delete texture;
    delete chunk;
    delete mesh;
    
    Window::terminate();
    return 0;
}