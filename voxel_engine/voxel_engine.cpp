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
    0.0f, -0.01f,
    0.0f,  0.01f,

    -0.01f, 0.0f,
     0.01f, 0.0f
};
std::vector<int> attributes = {2,0};
int main()
{
    srand(time(0));
    int currentWidth = Window::getWidth();
    int currentHeight = Window::getHeight();

    Window::initialize(currentWidth, currentHeight, "window");
    Events::initialize();

    Shader* voxelShader = loadShader("D:/DEV/c++/voxel_engine/res/voxel.vert", "D:/DEV/c++/voxel_engine/res/voxel.frag");
    if (voxelShader == nullptr)
    {
        std::cerr << "couldnt load the voxel shader" << std::endl;
        Window::terminate();
        return 1;
    }
    Shader* crosshairShader = loadShader("D:/DEV/c++/voxel_engine/res/crosshair.vert", "D:/DEV/c++/voxel_engine/res/crosshair.frag");
    if (crosshairShader == nullptr)
    {
        std::cerr << "couldnt load the crosshair shader" << std::endl;
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
    Chunks* chunks = new Chunks(20, 2, 20);
    Mesh** meshes = new Mesh*[chunks->getVolume()];
    for (size_t i = 0; i < chunks->getVolume(); i++)
        meshes[i] = nullptr;

    VoxelRenderer renderer(1024 * 1024);
    

    glClearColor(0.6f, 0.62f, 0.65f, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Mesh* crosshair = new Mesh(vertices, 4, attributes);

    Camera* camera = new Camera(vec3(155, 55, 55), radians(70.0f));

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
            camera->move(camera->getFront() * delta * speed);
        }
        if (Events::pressed(GLFW_KEY_A)) 
        {
            camera->move(-camera->getRight() * delta * speed);
        }
        if (Events::pressed(GLFW_KEY_S)) 
        {
            camera->move(-camera->getFront() * delta * speed);
        }
        if (Events::pressed(GLFW_KEY_D)) 
        {
            camera->move(camera->getRight() * delta * speed);
        }
        float pitchDelta = -Events::getDeltaY() * sensitivityY;
        float yawDelta = -Events::getDeltaX() * sensitivityX;
        static float pitch = 0.0f;
        pitch += pitchDelta;
        pitch = clamp(pitch, -89.0f, 89.0f);
        
        camera->rotate(pitchDelta, yawDelta);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            vec3 end;
            vec3 norm;
            vec3 voxCoords;
            AirBlock* air = new AirBlock();
            PlanksBlock* wood = new PlanksBlock();
            IBlock* vox = chunks->pointerRay(camera->getPos(), camera->getFront(), 15.0f, end, norm, voxCoords);
            if (vox != nullptr) 
            {
                if (Events::jtClicked(GLFW_MOUSE_BUTTON_1)) 
                {
                    chunks->setVoxel(static_cast<int>(voxCoords.x), static_cast<int>(voxCoords.y), static_cast<int>(voxCoords.z), air);
                }
                if (Events::jtClicked(GLFW_MOUSE_BUTTON_2)) 
                {
                    chunks->setVoxel(static_cast<int>(voxCoords.x) + static_cast<int>(norm.x), static_cast<int>(voxCoords.y) + static_cast<int>(norm.y), static_cast<int>(voxCoords.z) + static_cast<int>(norm.z), wood);
                }
            }
        }
        Chunk* closeChunks[27];

        for (size_t i = 0; i < chunks->getVolume(); i++)
        {
            Chunk* chunk = chunks->getChunkById(i);
            if (!chunk->getModifiedState())
                continue;
            chunk->modify(false);

            if (meshes[i] != nullptr)
                delete meshes[i];

            for (int k = 0; k < 27; k++)
            {
                closeChunks[k] = nullptr;
            }
            for (size_t j = 0; j < chunks->getVolume(); j++)
            {
                Chunk* other = chunks->getChunkById(j);

                int ox = other->getX() - chunk->getX();
                int oy = other->getY() - chunk->getY();
                int oz = other->getZ() - chunk->getZ();

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

                int index = (oy * 3 + oz) * 3 + ox;
                closeChunks[index] = other;
            }

            Mesh* mesh = renderer.render(chunk, (const Chunk**)closeChunks, true);
            meshes[i] = mesh;
        }

        voxelShader->use();
        voxelShader->uniformMatrix("projview", camera->getPerspective() * camera->getView());
        texture->bind();

        mat4 model;
        for (size_t i = 0; i < chunks->getVolume(); i++)
        {
            Chunk* chunk = chunks->getChunkById(i);
            Mesh* mesh = meshes[i];
            mat4 model = glm::translate(mat4(1.0f), vec3(chunk->getX() * CHUNK_WIDTH+0.5f, chunk->getY() * CHUNK_HEIGHT + 0.5f, chunk->getZ() * CHUNK_DEPTH + 0.5f));
            voxelShader->uniformMatrix("model", model);
            mesh->drawPrimitive(GL_TRIANGLES);
        }
        crosshairShader->use();
        crosshair->drawPrimitive(GL_LINES);

        Window::swapBuffers();
        Events::pullEvents();
    }
    delete voxelShader;
    delete crosshairShader;
    delete crosshair;
    delete texture;
    delete chunks;

    Window::terminate();
    return 0;
}