#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>

enum class ShaderType : unsigned int
{
  VERTEX_SHADER = GL_VERTEX_SHADER,
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

struct ShaderFile
{
  unsigned int id;
  const char *path;
  const ShaderType type;
};

struct Program
{
  unsigned int id;
  std::vector<ShaderFile *> shaders;
};

class Shader
{
private:
  unsigned int program = 0;

  std::vector<ShaderFile> shaders;
  std::unordered_map<std::string, unsigned int> uniforms;
  std::unordered_map<std::string, Program> programs;

public:
  ~Shader();

  unsigned int compile(const char *filepath, const ShaderType &type);

  void recompile();

  unsigned int createProgram(const std::string &program, const std::vector<unsigned int> &link);

  void bind(const std::string &program);

  void unbind();

  void uniform1i(const std::string &name, int location);

  void uniformMatrix4fv(const std::string &name, const glm::mat4 &uniform);
};