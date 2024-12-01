#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <assimp/Importer.hpp>

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

class Mesh
{

private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

public:
  void resize(size_t count) { vertices.resize(count); }

  Vertex &getVertex(unsigned int index) { return vertices[index]; }

  const std::vector<Vertex> &getVertices() const { return vertices; }

  const std::vector<unsigned int> &getIndices() const { return indices; }

  void addVertex(const Vertex &vertex) { vertices.push_back(vertex); }

  void addIndex(unsigned int index) { indices.push_back(index); }

  void clear()
  {
    vertices.clear();
    indices.clear();
  }
};