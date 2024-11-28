#pragma once

#include <string>

#include "Material.h"

class Model
{
private:
  const std::string name;
  const std::string path;

public:
  Model(const std::string &name, const std::string &path);

  ~Model() = default;

  void setMaterial(const Material *material);
};