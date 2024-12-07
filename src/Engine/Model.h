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

class Model
{
private:
  Renderer *renderer;

  unsigned int id;

  std::vector<Mesh> meshes;

  std::vector<Texture2D *> textures;

  Material *material;

  std::vector<Instance> instances;

  unsigned int indicesCount;

public:
  Model(Renderer *renderer, unsigned int id, const char *filepath);
  ~Model();

  const unsigned int getID();

  void setMaterial(Material *material);

  void addTexture(Texture2D *texture);

  void bindTextures() const;

  const unsigned int createInstance();
  Instance &getInstance(unsigned int id);
  std::vector<Instance> &getInstances();

  const unsigned int getIndicesCount() const;
  const unsigned int getInstancesCount() const;

  const std::vector<Vertex> getVertices() const;
  const std::vector<unsigned int> getIndices() const;
};