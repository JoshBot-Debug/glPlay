#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string &name, const std::string &path) : name(name), path(path)
{
  loadFBX(path, meshes);
}

void Model::setMaterial(Material *material)
{
  this->material = material;
}

void Model::addTexture(Texture *texture)
{
  textures.push_back(texture);
}

void Model::bindTextures() const
{
  for (size_t i = 0; i < textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    textures[i]->bind();
  }
}

std::vector<InstanceManager *> Model::getInstanceManagers()
{
  std::vector<InstanceManager *> ims;

  for (auto &im : instanceManagers)
    ims.push_back(&im.second);

  return ims;
}

InstanceManager *Model::getInstanceManager(const std::string &name)
{
  return &instanceManagers[name];
}

const void Model::setIndicesOffset(size_t size)
{
  indicesOffset = size;
}

const size_t Model::getIndicesOffset() const
{
  return indicesOffset;
}

const size_t Model::getInstancesCount()
{
  return instanceManagers.size();
}

const std::vector<Vertex> Model::getVertices() const
{
  std::vector<Vertex> vertices;

  size_t count = 0;
  for (const auto &mesh : meshes)
    count += mesh.getVertices().size();

  vertices.reserve(count);

  for (const auto &mesh : meshes)
  {
    const auto &v = mesh.getVertices();
    vertices.insert(vertices.begin(), v.begin(), v.end());
  }

  return vertices;
}

const std::vector<unsigned int> Model::getIndices() const
{
  std::vector<unsigned int> indices;

  size_t count = 0;
  
  for (const auto &mesh : meshes)
    count += mesh.getIndices().size();

  indices.reserve(count);

  for (const auto &mesh : meshes)
  {
    const auto &v = mesh.getIndices();
    indices.insert(indices.begin(), v.begin(), v.end());
  }

  return indices;
}

const std::string &Model::getName() const
{
  return name;
}

void Model::loadFBX(const std::string &path, std::vector<Mesh> &meshes)
{
  // Initialize Assimp importer
  Assimp::Importer importer;

  // Load the model (FBX file)
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals);

  if (!scene)
  {
    std::cerr << "Failed to load FBX model: " << importer.GetErrorString() << std::endl;
    return;
  }

  // Reserve space in the vector
  meshes.resize(scene->mNumMeshes);

  // Iterate over each mesh in the scene
  for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
  {
    aiMesh *mesh = scene->mMeshes[i];
    Mesh &m = meshes[i];

    m.resize(mesh->mNumVertices);

    // Process vertices
    for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
    {
      Vertex &v = m.getVertex(j);

      const aiVector3D &vertex = mesh->mVertices[j];
      v.position.x = vertex.x;
      v.position.y = vertex.y;
      v.position.z = vertex.z;

      const aiVector3D &normal = mesh->mNormals[j];
      v.normal.x = normal.x;
      v.normal.y = normal.y;
      v.normal.z = normal.z;

      if (mesh->mTextureCoords[0])
      {
        const aiVector3D &texCoord = mesh->mTextureCoords[0][j];
        v.texCoord.x = texCoord.x;
        v.texCoord.y = texCoord.y;
      }
      else
      {
        v.texCoord.x = 0.0f;
        v.texCoord.y = 0.0f;
      }
    }

    for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
    {
      const aiFace &face = mesh->mFaces[j];
      for (unsigned int k = 0; k < face.mNumIndices; ++k)
        m.addIndex(face.mIndices[k]);
    }
  }
}