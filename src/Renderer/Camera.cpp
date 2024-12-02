#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::update()
{
  if (type == CameraType::Perspective)
  {
    glm::vec3 front;
    front.x = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
    front.y = sin(glm::radians(rotation.x));
    front.z = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x)) * -1.0f;

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, front));

    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), front);

    right = glm::normalize(glm::vec3(rollMatrix * glm::vec4(right, 0.0f)));
    up = glm::normalize(glm::vec3(rollMatrix * glm::vec4(up, 0.0f)));

    glm::vec3 cameraPosition = position + glm::vec3(offsetX, offsetY, 0.0f);

    view = glm::lookAt(cameraPosition, cameraPosition + front, up);

    projection = glm::perspective(glm::radians(fov), width / height, nearPlane, farPlane);
  }

  if (type == CameraType::Orthographic)
  {
    float w = width * offsetX;
    float h = height * offsetY;

    projection = glm::ortho(-w, +w, -h, +h, -1.0f, 1.0f);

    view = glm::mat4(1.0f);
    view = glm::translate(view, -position);

    view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  }

  projection *= view;
}

void Camera::setType(CameraType type)
{
  this->type = type;
}

void Camera::setSize(float width, float height)
{
  this->width = width;
  this->height = height;
}

void Camera::setOffset(float offsetX, float offsetY)
{
  this->offsetX = offsetX;
  this->offsetY = offsetY;
}

void Camera::setPosition(float x, float y, float z)
{
  if (type == CameraType::Orthographic)
  {
    position.x = x - (width * offsetX);
    position.y = y - (height * offsetY);
    position.z = z;
    return;
  }

  position.x = x;
  position.y = y;
  position.z = z;
}

void Camera::setRotation(float pitch, float yaw, float roll)
{
  rotation.x = pitch;
  rotation.y = yaw;
  rotation.z = roll;
}

void Camera::translate(float deltaX, float deltaY, float deltaZ)
{
  position.x += deltaX;
  position.y += deltaY;
  position.z += deltaZ;
}

void Camera::rotate(float deltaPitch, float deltaYaw, float deltaRoll)
{
  rotation.x += deltaPitch;
  rotation.y += deltaYaw;
  rotation.z += deltaRoll;
}

void Camera::setProjection(float fov, float nearPlane, float farPlane)
{
  this->fov = fov;
  this->nearPlane = nearPlane;
  this->farPlane = farPlane;
}

const glm::mat4 &Camera::getViewProjectionMatrix() const
{
  return projection;
}