#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
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

inline unsigned int compileShader(const char *filepath, const ShaderType &type)
{
  const char *source = readFile(filepath);

  if (!source)
    return 0;

  unsigned int shader = glCreateShader((unsigned int)type);

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
    return 0;
  }

  LOG_BREAK_BEFORE;
  LOG("Shader created:", filepath, " : ", shader);
  LOG_BREAK_AFTER;

  return shader;
}

inline unsigned int createShaderProgram(const std::vector<unsigned int> &link)
{
  unsigned int id = glCreateProgram();

  for (size_t i = 0; i < link.size(); i++)
    glAttachShader(id, link[i]);

  glLinkProgram(id);

  int success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);

  if (!success)
  {
    int length = 0;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

    std::vector<char> log(length);
    glGetProgramInfoLog(id, length, &length, log.data());

    LOG_BREAK_BEFORE;
    LOG("Program linking failed");
    LOG("ERROR:", log.data());
    LOG_BREAK_AFTER;

    glDeleteProgram(id);
    return 0;
  }

  for (size_t i = 0; i < link.size(); i++)
    glDeleteShader(link[i]);

  return id;
}

Shader::~Shader()
{
  glUseProgram(0);
  program = 0;
  for (const auto &program : programs)
    glDeleteProgram(program.second.id);
}

unsigned int Shader::compile(const char *filepath, const ShaderType &type)
{
  unsigned int id = compileShader(filepath, type);

  if (id)
    shaders.emplace_back(id, filepath, type);

  return id;
}

void Shader::recompile()
{
  for (auto &program : programs)
  {
    std::vector<unsigned int> ids;

    for (auto &file : program.second.shaders)
    {
      file->id = compileShader(file->path, file->type);
      if (file->id)
        ids.push_back(file->id);
    }

    program.second.id = createShaderProgram(ids);
  }
}

unsigned int Shader::createProgram(const std::string &program, const std::vector<unsigned int> &link)
{
  unsigned int id = createShaderProgram(link);

  std::vector<ShaderFile *> shaderFiles;

  for (size_t i = 0; i < link.size(); i++)
    for (size_t j = 0; j < shaders.size(); j++)
      if (shaders[j].id == link[i])
        shaderFiles.push_back(&shaders[j]);

  programs.emplace(program, Program(id, shaderFiles));

  return id;
}

void Shader::bind(const std::string &program)
{
  glUseProgram(programs[program].id);
  this->program = programs[program].id;
}

void Shader::unbind()
{
  glUseProgram(0);
  program = 0;
}

void Shader::uniform1i(const std::string &name, int location)
{
  try
  {
    if (!program)
    {
      LOG_BREAK_BEFORE;
      LOG("Error! No shader program bound");
      LOG("Cannot set uniform", name);
      LOG_BREAK_AFTER;
      return;
    }

    if (!uniforms[name])
      uniforms[name] = glGetUniformLocation(program, name.c_str());

    glUniform1i(uniforms[name], location);
  }
  catch (const std::exception &e)
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to set uniform:", name);
    LOG("Bound program:", program);
    LOG_BREAK_AFTER;
  }
}

void Shader::uniformMatrix4fv(const std::string &name, const glm::mat4 &uniform)
{
  try
  {
    if (!program)
    {
      LOG_BREAK_BEFORE;
      LOG("Error! No shader program bound");
      LOG("Cannot set uniform", name);
      LOG_BREAK_AFTER;
      return;
    }

    if (!uniforms[name])
      uniforms[name] = glGetUniformLocation(program, name.c_str());

    glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, &uniform[0][0]);
  }
  catch (const std::exception &e)
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to set uniform:", name);
    LOG("Bound program:", program);
    LOG_BREAK_AFTER;
  }
}