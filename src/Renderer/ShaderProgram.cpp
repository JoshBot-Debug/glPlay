#include "ShaderProgram.h"

#include "Debug.h"

Program *ShaderProgram::createProgram(const std::string &name)
{
  return &programs[name];
}

void ShaderProgram::bind(const std::string &name)
{
  if (program == name)
    return;

  try
  {
    program = name;
    glUseProgram(programs.at(name).getProgram());
  }
  catch (const std::exception &e)
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to bind program:", name);
    LOG_BREAK_AFTER;
  }
}

void ShaderProgram::unbind()
{
  glUseProgram(0);
  program.reset();
}

void ShaderProgram::uniformMatrix4fv(const std::string &name, const glm::mat4 &uniform)
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
      uniforms[name] = glGetUniformLocation(programs.at(program.value()).getProgram(), name.c_str());

    glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, &uniform[0][0]);
  }
  catch (const std::exception &e)
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to set uniform:", name);
    LOG("Bound program:", program.value());
    LOG_BREAK_AFTER;
  }
}
