#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer.h"
#include "Debug.h"

void loadModel(const char *path, std::vector<Mesh> &meshes)
{
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals);

  if (!scene)
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to load model:", importer.GetErrorString());
    LOG_BREAK_AFTER;
    return;
  }

  meshes.resize(scene->mNumMeshes);

  for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
  {
    aiMesh *mesh = scene->mMeshes[i];
    Mesh &m = meshes[i];

    m.resize(mesh->mNumVertices);

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

Model::Model(Renderer *renderer, unsigned int id, const char *filepath) : renderer(renderer), id(id)
{
  loadModel(filepath, meshes);

  indicesCount = getIndices().size();

  renderer->upsertModel(this);
}

Model::~Model()
{
  material = nullptr;
}

const unsigned int Model::getID()
{
  return id;
}

void Model::setMaterial(Material *material)
{
  this->material = material;
}

const unsigned int Model::createInstance()
{
  instances.emplace_back();

  instancesCount = instances.size();

  const unsigned int id = instancesCount - 1;

  renderer->upsertInstance(this, instances[id], id);

  return id;
}

Instance &Model::getInstance(unsigned int id)
{
  return instances.at(id);
}

std::vector<Instance> &Model::getInstances()
{
  return instances;
}

const unsigned int Model::getIndicesCount() const
{
  return indicesCount;
}

const unsigned int Model::getInstancesCount() const
{
  return instancesCount;
}

const size_t Model::getIndiceSizeOffset() const
{
  return indiceSizeOffset;
}

const void Model::setIndiceSizeOffset(size_t offset)
{
  indiceSizeOffset = offset;
}

const size_t Model::getVertexOffset() const
{
  return vertexOffset;
}

const void Model::setVertexOffset(size_t offset)
{
  vertexOffset = offset;
}

const size_t Model::getInstanceOffset() const
{
  return instanceOffset;
}

const void Model::setInstanceOffset(size_t offset)
{
  instanceOffset = offset;
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