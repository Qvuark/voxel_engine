#include "camera.h"
#include "window.h"
#include <glm/ext.hpp>

Camera::Camera(vec3 position, float fov) : pos(position), fov(fov), angle(1.0f)
{
	updateVectors();
}

void Camera::updateVectors()
{
	right = vec3(angle * vec4(1, 0, 0, 1));
	up    = vec3(angle * vec4(0, 1, 0, 1));
	front = vec3(angle * vec4(0, 0,-1, 1));
}

void Camera::rotate(float x, float y, float z)
{
	angle = glm::rotate(angle, x, vec3(1, 0, 0));
	angle = glm::rotate(angle, y, vec3(0, 1, 0));
	angle = glm::rotate(angle, z, vec3(0, 0, 1));
	updateVectors();
}

mat4 Camera::getPerspective()
{
	float aspect = static_cast<float>(Window::width) / static_cast<float> (Window::height);
	return perspective(fov, aspect, 0.1f, 100.0f);
}
mat4 Camera::getView()
{
	return lookAt(pos, pos + front, up);
}
