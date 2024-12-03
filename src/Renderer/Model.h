#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Instance.h"

class Model
{
private:
  const std::string name;
  const std::string path;

  std::vector<Mesh> meshes;
  std::vector<Texture *> textures;

  Material *material;

  size_t indicesOffset = 0;
  std::unordered_map<std::string, InstanceManager> instanceManagers;

private:
  void loadFBX(const std::string &filename, std::vector<Mesh> &meshes);

public:
  Model(const std::string &name, const std::string &path);

  void setMaterial(Material *material);

  void addTexture(Texture *texture);

  void bindTextures() const;

  std::vector<InstanceManager *> getInstanceManagers();

  InstanceManager *getInstanceManager(const std::string &name);

  const void setIndicesOffset(size_t offset);

  const size_t getIndicesOffset() const;

  const size_t getInstancesCount();

  const std::vector<Vertex> getVertices() const;

  const std::vector<unsigned int> getIndices() const;

  const std::string &getName() const;
};