#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
  virtual ~Shader() = default;
  virtual void setUniformMatrix4fv(const std::string &name, const glm::mat4 &uniform) = 0;
};