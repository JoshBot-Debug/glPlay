#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
  virtual void update() = 0;

  virtual void setViewportSize(float width, float height) = 0;

  virtual const glm::mat4 getViewProjectionMatrix() const = 0;
};