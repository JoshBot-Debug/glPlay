#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Texture2D.h"
#include "Material.h"

class Renderer;

struct Instance
{
  glm::vec3 translate = glm::vec3(0.0f);

  glm::vec3 rotation = glm::vec3(0.0f);

  glm::vec3 scale = glm::vec3(1.0f);

  glm::vec4 color = glm::vec4(1.0f);
};

struct DrawElementsIndirectCommand
{
  unsigned int count;
  unsigned int instanceCount;
  unsigned int firstIndex;
  unsigned int baseVertex;
  unsigned int baseInstance;
};

class Model
{
private:
  unsigned int id;

  std::vector<Mesh> meshes;

  Material *material;

  std::vector<Instance> instances;

  size_t vertexOffset;
  size_t instanceOffset;
  size_t indiceSizeOffset;
  unsigned int indicesCount;
  unsigned int instancesCount;

public:
  Model(unsigned int id, const char *filepath);
  ~Model();

  const unsigned int getID();

  void setMaterial(Material *material);

  const unsigned int createInstance();
  Instance &getInstance(unsigned int id);
  std::vector<Instance> &getInstances();

  const size_t getVertexOffset() const;
  const void setVertexOffset(size_t offset);

  const size_t getInstanceOffset() const;
  const void setInstanceOffset(size_t offset);

  const size_t getIndiceSizeOffset() const;
  const void setIndiceSizeOffset(size_t offset);

  const unsigned int getIndicesCount() const;
  const unsigned int getInstancesCount() const;

  const std::vector<Vertex> getVertices() const;
  const std::vector<unsigned int> getIndices() const;
};