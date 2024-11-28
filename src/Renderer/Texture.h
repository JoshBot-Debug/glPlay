#pragma once

#include <string>

class Texture
{
private:
  const std::string name;
  const std::string path;

public:
  Texture(const std::string &name, const std::string &path);
  ~Texture() = default;
};