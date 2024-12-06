#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

void PerspectiveCamera::update()
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

void PerspectiveCamera::setViewportSize(const glm::vec2 &size)
{
  this->width = width;
  this->height = height;
}

void PerspectiveCamera::setViewportSize(float width, float height)
{
  this->width = width;
  this->height = height;
}

void PerspectiveCamera::setOffset(float offsetX, float offsetY)
{
  this->offsetX = offsetX;
  this->offsetY = offsetY;
}

void PerspectiveCamera::setPosition(float x, float y, float z)
{
  position.x = x;
  position.y = y;
  position.z = z;
}

void PerspectiveCamera::setRotation(float pitch, float yaw, float roll)
{
  rotation.x = pitch;
  rotation.y = yaw;
  rotation.z = roll;
}

void PerspectiveCamera::translate(float deltaX, float deltaY, float deltaZ)
{
  position.x += deltaX;
  position.y += deltaY;
  position.z += deltaZ;
}

void PerspectiveCamera::rotate(float deltaPitch, float deltaYaw, float deltaRoll)
{
  rotation.x += deltaPitch;
  rotation.y += deltaYaw;
  rotation.z += deltaRoll;
}

void PerspectiveCamera::setProjection(float fov, float nearPlane, float farPlane)
{
  this->fov = fov;
  this->nearPlane = nearPlane;
  this->farPlane = farPlane;
}

const glm::mat4 PerspectiveCamera::getViewProjectionMatrix() const
{
  return projection * view;
}