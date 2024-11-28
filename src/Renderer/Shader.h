#pragma once

#include <GL/gl.h>
#include <string>

enum class ShaderType: unsigned int {
  VERTEX_SHADER = GL_VERTEX_SHADER,
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

class Shader
{
private:
  const ShaderType type;
  const std::string path;

public:
  Shader(const std::string &path, const ShaderType &type);
  ~Shader() = default;
};