#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Debug.h"

bool Shader::compile(const std::string &path, const ShaderType &type)
{
  std::ifstream stream(path);

  if (!stream.is_open())
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to open shader file:", path);
    LOG_BREAK_AFTER;
    return false;
  }

  std::stringstream sStream;
  sStream << stream.rdbuf();
  stream.close();

  std::string shaderStr = sStream.str();
  const char *shaderCStr = shaderStr.c_str();

  shader = glCreateShader((unsigned int)type);
  glShaderSource(shader, 1, &shaderCStr, nullptr);
  glCompileShader(shader);

  LOG_BREAK_BEFORE;
  LOG("Created shader", path);
  LOG("Shader:", shader);
  LOG_BREAK_AFTER;

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    char *log = new char[length];
    glGetShaderInfoLog(shader, length, &length, log);

    LOG_BREAK_BEFORE;
    LOG("Shader compilation failed", path);
    LOG("Shader file:", path);
    LOG("ERROR:", log);
    LOG_BREAK_AFTER;

    delete[] log;
    glDeleteShader(shader);
    return false;
  }

  return true;
}

Shader::Shader(const std::string &path, const ShaderType &type) : path(path), type(type)
{
  compile(path, type);
}

bool Shader::recompile()
{
  return compile(path, type);
}

const unsigned int Shader::getShader()
{
  return shader;
}

void Shader::destroy()
{
  glDeleteShader(shader);

  LOG_BREAK_BEFORE;
  LOG("Deleted shader:", shader);
  LOG_BREAK_AFTER;

  shader = 0;
}
