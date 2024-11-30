#include "Program.h"

#include <iostream>
#include "Debug.h"

Program::Program()
{
  program = glCreateProgram();

  LOG_BREAK_BEFORE;
  LOG("Created shader program");
  LOG("Program:", program);
  LOG_BREAK_AFTER;
}

Program::~Program()
{
  if (program != 0)
  {
    glDeleteProgram(program);
    LOG_BREAK_BEFORE;
    LOG("Deleted shader program");
    LOG("Program:", program);
    LOG_BREAK_AFTER;
    program = 0;
  }
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
    LOG("Shader program linking failed");
    LOG("ERROR:", log);
    LOG_BREAK_AFTER;

    delete[] log;

    glDeleteProgram(program);
    program = 0;
    
    return false;
  }

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
