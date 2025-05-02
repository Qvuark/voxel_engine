#ifndef CAMERA_MOVEMENT_H
#define CAMERA_MOVEMENT_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

class Camera 
{
public:
    vec3 pos;
    float fov;
    quat orientation;
    vec3 front, right, up;

    Camera(vec3 position, float fov);
    void updateVectors();
    void rotate(float pitch, float yaw, float roll = 0.0f);
    mat4 getPerspective() const;
    mat4 getView() const;
};
#endif