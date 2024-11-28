#include "Shader.h"

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>

// Shader::~Shader()
// {
//   if (program != 0)
//     glDeleteProgram(program);
// }

// bool Shader::compile(const char *filepath, ShaderType type, bool recompilable)
// {
//   std::ifstream stream(filepath);

//   if (recompilable)
//     files.emplace_back<ShaderFile>({filepath, type});

//   if (!stream.is_open())
//   {
//     std::cerr << "ERROR: Failed to open shader file: " << filepath << std::endl;
//     return false;
//   }

//   std::stringstream sStream;
//   sStream << stream.rdbuf();
//   stream.close();

//   std::string shaderStr = sStream.str();
//   const char *shaderCStr = shaderStr.c_str();

//   unsigned int shader = glCreateShader((unsigned int)type);
//   glShaderSource(shader, 1, &shaderCStr, nullptr);
//   glCompileShader(shader);

//   int success;
//   glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//   if (!success)
//   {
//     int length;
//     glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
//     char *log = new char[length];
//     glGetShaderInfoLog(shader, length, &length, log);
//     std::cerr << "ERROR: Shader compilation failed (" << filepath << "):\n"
//               << log << std::endl;
//     delete[] log;
//     glDeleteShader(shader);
//     return false;
//   }

//   shaders.push_back(shader);
//   return true;
// }

// bool Shader::recompile()
// {
//   for (auto file : files)
//     if (!compile(file.filepath, file.type, false))
//       return false;
//   return true;
// }

// bool Shader::link()
// {
//   program = glCreateProgram();

//   if (program == 0)
//     std::cerr << "Failed to create shader program." << std::endl;

//   for (const unsigned int &shader : shaders)
//     glAttachShader(program, shader);

//   glLinkProgram(program);

//   int success;
//   glGetProgramiv(program, GL_LINK_STATUS, &success);
//   if (!success)
//   {
//     int length;
//     glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
//     char *log = new char[length];
//     glGetProgramInfoLog(program, length, &length, log);
//     std::cerr << "ERROR: Shader program linking failed:\n"
//               << log << std::endl;
//     delete[] log;
//     glDeleteProgram(program);
//     return false;
//   }

//   for (const unsigned int &shader : shaders)
//     glDeleteShader(shader);

//   shaders.clear();

//   return true;
// }

// void Shader::bind() const
// {
//   if (!glIsProgram(program))
//     std::cerr << "[Shader Program] Invalid program ID: " << program << std::endl;

//   glUseProgram(program);
// }

// void Shader::unbind() const
// {
//   glUseProgram(0);
// }

// void Shader::addUniformMatrix4fv(const std::string &name, glm::mat4 &uniform) const
// {
//   if (!uniformLocation[name])
//     uniformLocation[name] = glGetUniformLocation(program, name.c_str());

//   glUniformMatrix4fv(uniformLocation[name], 1, GL_FALSE, &uniform[0][0]);
// }

Shader::Shader(const std::string &path, const ShaderType &type) : path(path), type(type) {}
