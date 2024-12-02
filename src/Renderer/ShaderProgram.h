#pragma once

#include <string>
#include <unordered_map>
#include <optional>

#include "Program.h"

#include <glm/glm.hpp>

class ShaderProgram
{
private:
  /**
   * The program that is currently bound
   */
  std::optional<std::string> program;
  std::unordered_map<std::string, Program> programs;
  std::unordered_map<std::string, unsigned int> uniforms;

public:
  Program *createProgram(const std::string &name);

  void recompile();

  void bind(const std::string &name);

  void unbind();

  void uniform1i(const std::string &name, int location);

  void uniformMatrix4fv(const std::string &name, const glm::mat4 &uniform);
};