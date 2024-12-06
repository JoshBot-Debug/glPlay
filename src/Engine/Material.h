#pragma once

#include <glm/glm.hpp>

#include "Texture2D.h"

class Material
{
private:
  unsigned int id;

public:
  Material(unsigned int id);
  ~Material() = default;

  void setDiffuseTexture(const Texture2D *texture);
  void setDiffuseColor(const glm::vec3 &color);
  void setRoughness(const float &value);
  void setShininess(const float &value);
};