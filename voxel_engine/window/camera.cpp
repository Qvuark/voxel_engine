#include "camera.h"
#include "window.h"
#include <glm/ext.hpp>

Camera::Camera(vec3 position, float fov) : pos(position), fov(fov), angle(1.0f)
{
	updateVectors();
}

void Camera::updateVectors()
{
	right = vec3(angle * vec4(1.0f, 0.0f, 0.0f, 1.0f));
	up    = vec3(angle * vec4(0.0f, 1.0f, 0.0f, 1.0f));
	front = vec3(angle * vec4(0.0f, 0.0f, -1.0f,1.0f));
}

void Camera::rotate(float x, float y, float z)
{
	angle = glm::rotate(angle, z, vec3(0.0f, 0.0f, 1.0f));
	angle = glm::rotate(angle, z, vec3(0.0f, 1.0f, 0.0f));
	angle = glm::rotate(angle, z, vec3(1.0f, 0.0f, 0.0f));
	
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
