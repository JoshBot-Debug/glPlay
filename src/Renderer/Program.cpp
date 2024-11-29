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
  }
}

bool Program::link(Shader *shader)
{
  if (program == 0)
    std::cerr << "Failed to create shader program." << std::endl;

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
    return false;
  }

  shader->destroy();

  return true;
}

const unsigned int Program::getProgram()
{
  return program;
}
