#ifndef CAMERA_MOVEMENT_H
#define CAMERA_MOVEMENT_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

class Camera 
{
private:
    vec3 front, right, up;
    vec3 pos;
    float fov;
    quat orientation;
    void updateVectors();
public:
    Camera(vec3 position, float fov);
    void rotate(float pitch, float yaw, float roll = 0.0f);
    mat4 getPerspective() const;
    mat4 getView() const;
    vec3 getPos() const;
    quat getOrientation() const;
    vec3 getFront() const;
    vec3 getRight() const;
    vec3 getUp() const;
    void move(const vec3& delta);

};
#endif