#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

enum class VertexDraw
{
  STATIC = GL_STATIC_DRAW,
  DYNAMIC = GL_DYNAMIC_DRAW,
  STREAM = GL_STREAM_DRAW
};

enum class VertexType
{
  FLOAT = GL_FLOAT,
  INT = GL_INT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
  BYTE = GL_BYTE,
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  SHORT = GL_SHORT,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  BOOL = GL_BOOL,
};

enum class Primitive
{
  POINTS = GL_POINTS,
  LINES = GL_LINES,
  LINE_STRIP = GL_LINE_STRIP,
  LINE_LOOP = GL_LINE_LOOP,
  TRIANGLES = GL_TRIANGLES,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  TRIANGLE_FAN = GL_TRIANGLE_FAN
};

struct Vertex
{
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 normal = glm::vec3(0.0f);
  glm::vec2 texCoord = glm::vec2(0.0f);
  glm::vec3 tangent = glm::vec3(0.0f);
  glm::vec3 bitangent = glm::vec3(0.0f);
  glm::vec3 color = glm::vec3(1.0f);
};

struct Instance
{
  glm::vec3 translate = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
  glm::vec4 color = glm::vec4(1.0f);
};

struct Mesh
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
};

struct DrawElementsIndirectCommand
{
  unsigned int count;
  unsigned int primCount;
  unsigned int firstIndex;
  unsigned int baseVertex;
  unsigned int baseInstance;
};