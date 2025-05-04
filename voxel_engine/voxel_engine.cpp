
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
        std::cerr << "couldnt load the shader"<<std::endl;
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
    //create VAO

    VoxelRenderer renderer(1024 * 1024);
    std::vector<Chunk*>   chunks;
    std::vector<Mesh*>    meshes;
    chunks.reserve(4 * 4);
    meshes.reserve(4 * 4);

    // 3) Сгенерируем чанки и их меши
    for (int cz = 0; cz < 4; ++cz) {
        for (int cx = 0; cx < 4; ++cx) {
            Chunk* chunk = new Chunk();
            chunks.push_back(chunk);

            Mesh* mesh = renderer.render(chunk);
            meshes.push_back(mesh);
        }
    }
    glClearColor(0.6f, 0.62f, 0.65f, 1);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    Camera* camera = new Camera(vec3(5,5,20),radians(70.0f));

    mat4 model(1.0f);
    model = translate(model, vec3(0.5f, 0, 0));

    float lastTime = glfwGetTime();
    float delta = 0.0f;

    float sensitivityX = 0.004f;
    float sensitivityY = 0.0025f;

    float yaw = 0.0f;
    float pitch = 0.0f;
    float speed = 5;
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

        // 4) Для каждого чанка — своя модельная матрица
        int idx = 0;
        for (int cz = 0; cz < 4; ++cz) {
            for (int cx = 0; cx < 4; ++cx) {
                // сместим каждый чанк на его координаты в «мире» (ширина и глубина CHUNK_WIDTH/DEPTH)
                glm::mat4 chunkModel = glm::translate(
                    glm::mat4(1.0f),
                    glm::vec3(
                        float(cx * CHUNK_WIDTH),
                        0.0f,
                        float(cz * CHUNK_DEPTH)
                    )
                );
                shader->uniformMatrix("model", chunkModel);

                // рисуем меш чанка
                meshes[idx++]->drawPrimitive(GL_TRIANGLES);
            }
        }
        Window::swapBuffers();
    }
    delete shader;
    delete texture;
    for (auto m : meshes)   delete m;
    for (auto c : chunks)   delete c;
    
    Window::terminate();
    return 0;
}