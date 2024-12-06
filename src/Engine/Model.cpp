#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void loadModel(const char *path, std::vector<Mesh> &meshes)
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

Model::Model(unsigned int id, const char *filepath) : id(id)
{
  loadModel(filepath, meshes);
}

void Model::setMaterial(Material *material)
{
  this->material = material;
}

void Model::addTexture(Texture2D *texture)
{
  textures.push_back(texture);
}

void Model::bindTextures() const
{
  for (size_t i = 0; i < textures.size(); i++)
    textures[i]->bind(i);
}

template <>
unsigned int Model::createInstance<Instance>(Instance &&instance)
{
  instances.emplace_back(std::forward<Instance>(instance));
  return instances.size() - 1;
}

Instance *Model::getInstance(unsigned int id)
{
  return nullptr;
}

std::vector<Instance *> Model::getInstances()
{
  return std::vector<Instance *>();
}

const void Model::setIndiceOffset(unsigned int offset)
{
  this->offset.indice = offset;
}

const void Model::setVertexOffset(unsigned int offset)
{
  this->offset.vertex = offset;
}

const void Model::setInstanceOffset(unsigned int offset)
{
  this->offset.instance = offset;
}

const unsigned int Model::getIndiceOffset() const
{
  return this->offset.indice;
}

const unsigned int Model::getVertexOffset() const
{
  return this->offset.vertex;
}

const unsigned int Model::getInstanceOffset() const
{
  return this->offset.instance;
}

const unsigned int Model::getInstanceCount() const
{
  return instances.size();
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