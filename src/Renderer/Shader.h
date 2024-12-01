#pragma once

#include <GL/glew.h>
#include <string>

enum class ShaderType : unsigned int
{
  VERTEX_SHADER = GL_VERTEX_SHADER,
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

class Shader
{
private:
  unsigned int shader = 0;
  const ShaderType type;
  const std::string path;

  bool compile(const std::string &filepath, const ShaderType &type);

public:
  Shader(const std::string &filepath, const ShaderType &type);

  ~Shader();

  bool recompile();

  const unsigned int getShader();

  void destroy();
};