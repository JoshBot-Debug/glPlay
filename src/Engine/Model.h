#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Texture2D.h"
#include "Material.h"

struct Instance
{
  glm::vec3 translate = glm::vec3(0.0f);

  glm::vec3 rotation = glm::vec3(0.0f);

  glm::vec3 scale = glm::vec3(1.0f);

  glm::vec4 color = glm::vec4(1.0f);
};

struct BufferOffset
{
  unsigned int vertex = 0;   // The offset in the vertex array buffer where the data begins
  unsigned int indice = 0;   // The offset in the element array buffer where the data begins
  unsigned int instance = 0; // The offset in the instance vertex array buffer where the data begins
};

class Model
{
private:
  unsigned int id;

  std::vector<Mesh> meshes;

  std::vector<Texture2D *> textures;

  Material *material;

  BufferOffset offset;

  std::vector<Instance *> instances;

public:
  Model(unsigned int id, const char *filepath);
  ~Model();

  void setMaterial(Material *material);

  void addTexture(Texture2D *texture);

  void bindTextures() const;

  unsigned int createInstance();
  Instance *getInstance(unsigned int id);
  std::vector<Instance *> &getInstances();

  const void setIndiceOffset(unsigned int offset);
  const void setVertexOffset(unsigned int offset);
  const void setInstanceOffset(unsigned int offset);

  const unsigned int getIndiceOffset() const;
  const unsigned int getVertexOffset() const;
  const unsigned int getInstanceOffset() const;
  const unsigned int getInstanceCount() const;

  const std::vector<Vertex> getVertices() const;
  const std::vector<unsigned int> getIndices() const;
};