#pragma once

#include "Shader.h"

class Program
{
private:
  unsigned int program;
  
public:
  Program();
  ~Program();

  bool link(Shader *shader);

  const unsigned int getProgram();
};