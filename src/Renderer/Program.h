#pragma once

#include <set>

#include "Shader.h"

class Program
{
private:
  unsigned int program = 0;
  std::set<Shader *> shaders;
  
public:
  Program();
  ~Program();

  bool link(Shader *shader);

  void recompile();

  const unsigned int getProgram();
};