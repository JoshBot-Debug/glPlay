#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::setType(const CameraType &type)
{
  this->type = type;
}

void Camera::update()
{
  /**
   * Set the origin to the center of the screen
   */
  float wSize = width / 2;
  float hSize = height / 2;
  projection = glm::ortho(-wSize, +wSize, -hSize, +hSize, -1.0f, 1.0f);

  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 translation = glm::translate(model, glm::vec3(-position.x, -position.y, 0.0f));

  glm::mat4 rotationX = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 rotationY = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotationZ = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 rotation = rotationZ * rotationY * rotationX;

  viewProjection = projection * (translation * rotation);
}

Camera::Camera()
{
  projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  viewProjection = projection * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

void Camera::setDimensions(int width, int height)
{
  this->width = width;
  this->height = height;
  update();
}

void Camera::setPosition(float x, float y, float z)
{
  /**
   * Set the origin to the center of the screen
   * x0 & y0 will be the center of the screen
   */
  position.x = x - (width / 2);
  position.y = y - (height / 2);
  position.z = z;
  update();
}

void Camera::move(float deltaX, float deltaY, float deltaZ)
{
  position.x += deltaX;
  position.y += deltaY;
  position.z += deltaZ;
  update();
}

void Camera::setRotation(float x, float y, float z)
{
  rotation.x = x;
  rotation.y = y;
  rotation.z = z;
  update();
}

const glm::mat4 &Camera::getUniformMatrix4fv() const
{
  return viewProjection;
}

const glm::vec3 &Camera::getPosition() const
{
  return position;
}

const glm::vec3 &Camera::getRotation() const
{
  return rotation;
}
