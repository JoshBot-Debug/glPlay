#pragma once

#include <string>
#include <unordered_map>

#include "Program.h"

class ShaderProgram
{
private:
  std::unordered_map<std::string, Program> programs;

public:
  ShaderProgram() = default;
  ~ShaderProgram() = default;

  Program *createProgram(const std::string &name);
};