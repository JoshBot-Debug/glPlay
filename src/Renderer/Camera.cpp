#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::update()
{
  if (type == CameraType::Perspective)
  {
    glm::vec3 front;
    front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
    front.y = sin(glm::radians(rotation.x));
    front.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));

    glm::vec3 up = glm::normalize(glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), front));

    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(glm::radians(fov), (float)width / (float)height, nearPlane, farPlane);
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
  update();
}

void Camera::setOffset(float offsetX, float offsetY)
{
  this->offsetX = offsetX;
  this->offsetY = offsetY;
  update();
}

void Camera::setPosition(float x, float y, float z)
{
  position.x = x - (width * offsetX);
  position.y = y - (height * offsetY);
  position.z = z;
  update();
}

void Camera::setRotation(float pitch, float yaw, float roll)
{
  rotation.x = pitch;
  rotation.y = yaw;
  rotation.z = roll;
  update();
}

void Camera::translate(float deltaX, float deltaY, float deltaZ)
{
  position.x += deltaX;
  position.y += deltaY;
  position.z += deltaZ;
  update();
}

void Camera::rotate(float deltaPitch, float deltaYaw, float deltaRoll)
{
  rotation.x += deltaPitch;
  rotation.y += deltaYaw;
  rotation.z += deltaRoll;
  update();
}

void Camera::setProjectionParams(float fov, float nearPlane, float farPlane)
{
  this->fov = fov;
  this->nearPlane = nearPlane;
  this->farPlane = farPlane;
  update();
}

const glm::mat4 &Camera::getViewProjectionMatrix() const
{
  return projection;
}