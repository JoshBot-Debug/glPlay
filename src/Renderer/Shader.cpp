#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#include "Debug.h"

inline const char *readFile(const std::string &filepath)
{
  std::ifstream stream(filepath);

  if (!stream.is_open())
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to open file:", filepath);
    LOG_BREAK_AFTER;
    return nullptr;
  }

  std::stringstream sStream;
  sStream << stream.rdbuf();
  stream.close();

  std::string fileContents = sStream.str();

  char *result = new char[fileContents.size() + 1];

  std::strcpy(result, fileContents.c_str());

  return result;
}

bool Shader::compile(const std::string &filepath, const ShaderType &type)
{
  const char *source = readFile(filepath);

  if (!source)
    return false;

  shader = glCreateShader((unsigned int)type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  delete[] source;

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    char *log = new char[length];
    glGetShaderInfoLog(shader, length, &length, log);

    LOG_BREAK_BEFORE;
    LOG("Compilation failed", filepath);
    LOG("ERROR:", log);
    LOG_BREAK_AFTER;

    delete[] log;
    glDeleteShader(shader);
    return false;
  }

  LOG_BREAK_BEFORE;
  LOG("Shader file:", filepath);
  LOG("Shader created:", shader);
  LOG_BREAK_AFTER;

  return true;
}

Shader::Shader(const std::string &path, const ShaderType &type) : path(path), type(type)
{
  compile(path, type);
}

Shader::~Shader()
{
  destroy();
}

bool Shader::recompile()
{
  LOG_BREAK_BEFORE;
  LOG("Shader recompile");
  LOG_BREAK_AFTER;
  glDeleteShader(shader);
  return compile(path, type);
}

const unsigned int Shader::getShader()
{
  return shader;
}

void Shader::destroy()
{
  if (shader == 0)
  {
    LOG_BREAK_BEFORE;
    LOG("Shader not set (delete skipped):", shader);
    LOG_BREAK_AFTER;
    return;
  }
  
  glDeleteShader(shader);

  LOG_BREAK_BEFORE;
  LOG("Shader deleted:", shader);
  LOG_BREAK_AFTER;

  shader = 0;
}
