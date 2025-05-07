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
#include "voxels/chunks.h"
#include "voxels/voxels.h"
#include "graphics/voxel_renderer.h"

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

int main()
{
    int currentWidth = Window::width;
    int currentHeight = Window::height;

    Window::initialize(currentWidth, currentHeight, "window");
    Events::initialize();

    Shader* shader = loadShader("D:/DEV/c++/voxel_engine/res/main.vert", "D:/DEV/c++/voxel_engine/res/main.frag");
    if (shader == nullptr)
    {
        std::cerr << "couldnt load the shader" << std::endl;
        Window::terminate();
        return 1;
    }

    Texture* texture = loadTexture("D:/DEV/c++/voxel_engine/res/block1.png");
    if (texture == nullptr)
    {
        std::cerr << "couldnt load the texture" << std::endl;
        Window::terminate();
        return 1;
    }
    Chunks* chunks = new Chunks(8, 1, 8);
    Mesh** meshes = new Mesh * [chunks->volume];
    
    VoxelRenderer renderer(1024 * 1024);
    Chunk* closeChunks[27];

    for (size_t i = 0; i < chunks->volume; i++) 
    {
        Chunk* chunk = chunks->chunks[i];
        for (int k = 0; k < 27; k++) 
        {
            closeChunks[k] = nullptr;
        }
        for (size_t j = 0; j < chunks->volume; j++) 
        {
            Chunk* other = chunks->chunks[j];

            int ox = other->x - chunk->x;
            int oy = other->y - chunk->y;
            int oz = other->z - chunk->z;

            if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1) 
            {
                continue;
            }

            ox++;
            oy++;
            oz++;

            if (ox < 0 || ox > 2 || oy < 0 || oy > 2 || oz < 0 || oz > 2) 
            {
                continue;
            }

            int index = oz * 9 + oy * 3 + ox;
            closeChunks[index] = other;
        }

        Mesh* mesh = renderer.render(chunk, (const Chunk**)closeChunks);
        meshes[i] = mesh;
    }


    glClearColor(0.6f, 0.62f, 0.65f, 1);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

    Camera* camera = new Camera(vec3(5, 5, 20), radians(70.0f));

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float sensitivityX = 0.004f;
    float sensitivityY = 0.0025f;

    float yaw = 0.0f;
    float pitch = 0.0f;
    float speed = 25;
    while (!Window::isShouldBeClosed())
    {
        


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
            camera->pos += camera->front * delta * speed;
        }
        if (Events::pressed(GLFW_KEY_A))
        {
            camera->pos -= camera->right * delta * speed;
        }
        if (Events::pressed(GLFW_KEY_S))
        {
            camera->pos -= camera->front * delta * speed;
        }
        if (Events::pressed(GLFW_KEY_D))
        {
            camera->pos += camera->right * delta * speed;
        }

        float pitchDelta = -Events::deltaY * sensitivityY;
        float yawDelta = -Events::deltaX * sensitivityX;
        static float pitch = 0.0f;
        pitch += pitchDelta;
        pitch = clamp(pitch, -89.0f, 89.0f);

        camera->rotate(pitchDelta, yawDelta);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw vao
        shader->use();
        shader->uniformMatrix("projview", camera->getPerspective() * camera->getView());
        texture->bind();

        mat4 model;
        for (size_t i = 0; i < chunks->volume; i++)
        {
            Chunk* chunk = chunks->chunks[i];
            Mesh* mesh = meshes[i];
            mat4 model = glm::translate(mat4(1.0f), vec3(chunk->x * CHUNK_WIDTH, chunk->y * CHUNK_HEIGHT, chunk->z * CHUNK_DEPTH));
            shader->uniformMatrix("model", model);
            mesh->drawPrimitive(GL_TRIANGLES);
        }

        Window::swapBuffers();
        Events::pullEvents();
    }
    delete shader;
    delete texture;


    Window::terminate();
    return 0;
}