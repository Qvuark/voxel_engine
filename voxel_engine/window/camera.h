#ifndef CAMERA_MOVEMENT_H
#define CAMERA_MOVEMENT_H

#include <glm/glm.hpp>
using namespace glm;

class Camera
{
	void updateVectors();
public:
	vec3 x,y,z;
	float fov;
	vec3 pos;
	mat4 angle;
	Camera(vec3 position, float fov);

	void rotate(float x, float y, float z);
	mat4 getProjection();
	mat4 getView();
};
#endif