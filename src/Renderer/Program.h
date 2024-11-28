#pragma once

#include "Shader.h"

class Program
{
private:
  
public:
  Program() = default;
  ~Program() = default;

  void link(const Shader *shader);
};