#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Instance.h"


struct BufferOffset
{
  unsigned int vertex = 0;   // The offset in the vertex array buffer where the data begins
  unsigned int indice = 0;   // The offset in the element array buffer where the data begins
  unsigned int instance = 0; // The offset in the instance vertex array buffer where the data begins
};

class Model
{
private:
  const std::string path;

  std::vector<Mesh> meshes;
  std::vector<Texture *> textures;

  Material *material;

  BufferOffset offset; // The offsets of the data in all buffers

  std::unordered_map<std::string, InstanceManager> InstanceManagers;

private:
  void loadFBX(const std::string &filename, std::vector<Mesh> &meshes);

public:
  Model(const std::string &path);

  void setMaterial(Material *material);

  void addTexture(Texture *texture);

  void bindTextures() const;

  std::vector<InstanceManager *> getInstanceManagers();

  InstanceManager *getInstanceManager(const std::string &name);

  const void setIndiceOffset(unsigned int offset);
  const void setVertexOffset(unsigned int offset);
  const void setInstanceOffset(unsigned int offset);

  const unsigned int getIndiceOffset() const;
  const unsigned int getVertexOffset() const;
  const unsigned int getInstanceOffset() const;

  const size_t getInstanceCount() const;

  const std::vector<Vertex> getVertices() const;

  const std::vector<unsigned int> getIndices() const;
};