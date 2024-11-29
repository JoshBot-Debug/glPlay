#pragma once

#include <string>
#include <vector>

#include "Material.h"
#include "Mesh.h"

class Model
{
private:
  const std::string name;
  const std::string path;
  std::vector<Mesh> meshes;

private:
  void loadFBX(const std::string &filename, std::vector<Mesh> &meshes);

public:
  Model(const std::string &name, const std::string &path);

  ~Model() = default;

  void setMaterial(const Material *material);
  
  const std::vector<Vertex> getVertices() const;

  const std::vector<unsigned int> getIndices() const;

  const std::string &getName() const;
};