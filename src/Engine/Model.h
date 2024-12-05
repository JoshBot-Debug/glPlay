#pragma once

#include <string>

class Model
{
public:
  virtual ~Model() = default;
  virtual void load(const std::string &filepath) = 0;
};