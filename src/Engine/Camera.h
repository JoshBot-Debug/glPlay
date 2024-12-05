#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
  virtual ~Camera() = default;

  virtual void setViewportSize() = 0;

  virtual const glm::mat4 getViewProjectionMatrix() = 0;
};