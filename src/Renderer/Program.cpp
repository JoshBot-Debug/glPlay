#include "Program.h"

#include <iostream>
#include "Debug.h"

Program::Program()
{
  program = glCreateProgram();

  LOG_BREAK_BEFORE;
  LOG("Program created:", program);
  LOG_BREAK_AFTER;
}

Program::~Program()
{
  for (const auto &shader : shaders)
    shader->destroy();

  if (program == 0)
    return;

  glDeleteProgram(program);
  LOG_BREAK_BEFORE;
  LOG("Program deleted:", program);
  LOG_BREAK_AFTER;
  program = 0;
}

bool Program::link(Shader *shader)
{
  shaders.insert(shader);

  glAttachShader(program, shader->getShader());

  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char *log = new char[length];
    glGetProgramInfoLog(program, length, &length, log);

    LOG_BREAK_BEFORE;
    LOG("Program linking failed");
    LOG("ERROR:", log);
    LOG_BREAK_AFTER;

    delete[] log;

    glDeleteProgram(program);
    program = 0;

    return false;
  }

  LOG_BREAK_BEFORE;
  LOG("Program linked shaded");
  LOG_BREAK_AFTER;

  shader->destroy();

  return true;
}

void Program::recompile()
{
  glDeleteProgram(program);

  program = glCreateProgram();

  for (const auto &shader : shaders)
  {
    shader->recompile();
    link(shader);
  }
}

const unsigned int Program::getProgram()
{
  return program;
}
