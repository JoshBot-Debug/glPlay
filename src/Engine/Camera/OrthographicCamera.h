#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Camera.h"

class OrthographicCamera : Camera
{
private:
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(0.0f);

public:
  float width = 1.0f;
  float height = 1.0f;

  float offsetX = 1.0f;
  float offsetY = 1.0f;

  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 position = glm::vec3(0.0f);

  float fov = 90.0f;
  float farPlane = 100.0f;
  float nearPlane = 0.1f;

  void update();

  void setViewportSize(float width, float height) override;

  void setOffset(float offsetX, float offsetY);

  void setPosition(float x, float y, float z);

  void setRotation(float pitch, float yaw, float roll);

  void setProjection(float fov, float nearPlane, float farPlane);

  void translate(float deltaX, float deltaY, float deltaZ);

  void rotate(float deltaPitch, float deltaYaw, float deltaRoll);

  const glm::mat4 getViewProjectionMatrix() const override;
};