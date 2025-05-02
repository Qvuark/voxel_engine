#include "camera.h"
#include "window.h"
#include <glm/ext.hpp>

Camera::Camera(vec3 position, float fov) : pos(position), fov(fov), orientation(quat(1, 0, 0, 0))
{
	updateVectors();
}
void Camera::updateVectors()
{
    front = normalize(orientation * vec3(0, 0, -1));
    right = normalize(cross(front, vec3(0, 1, 0)));
    up    = normalize(cross(right, front));
}

void Camera::rotate(float pitch, float yaw, float roll)
{
    quat qYaw = angleAxis(yaw, vec3(0, 1, 0));
    quat qPitch = angleAxis(pitch, vec3(1, 0, 0));

    orientation = qYaw * orientation * qPitch;
    orientation = normalize(orientation);

    updateVectors();
}

mat4 Camera::getPerspective() const
{
	float aspect = static_cast<float>(Window::width) / static_cast<float> (Window::height);
	return perspective(fov, aspect, 0.1f, 100.0f);
}
mat4 Camera::getView() const
{
	return lookAt(pos, pos + front, up);
}
