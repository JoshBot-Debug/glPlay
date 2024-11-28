#include "ShaderProgram.h"

Program *ShaderProgram::createProgram(const std::string &name)
{
  return &programs[name];
}