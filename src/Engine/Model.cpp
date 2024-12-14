#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Debug.h"

void processMesh(Mesh &m, aiMesh *mesh, const aiScene *scene)
{
  m.vertices.resize(mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex &v = m.vertices[i];

    std::cout << mesh->mVertices[i].y << std::endl;

    if (mesh->HasPositions())
    {
      v.position.x = mesh->mVertices[i].x;
      v.position.y = mesh->mVertices[i].y;
      v.position.z = mesh->mVertices[i].z;
    }

    if (mesh->HasNormals())
    {
      v.normal.x = mesh->mNormals[i].x;
      v.normal.y = mesh->mNormals[i].y;
      v.normal.z = mesh->mNormals[i].z;
    }

    if (mesh->HasTextureCoords(0))
    {
      v.texCoord.x = mesh->mTextureCoords[0][i].x;
      v.texCoord.y = mesh->mTextureCoords[0][i].y;
    }

    if (mesh->HasTangentsAndBitangents())
    {
      v.tangent.x = mesh->mTangents[i].x;
      v.tangent.y = mesh->mTangents[i].y;
      v.tangent.z = mesh->mTangents[i].z;

      v.bitangent.x = mesh->mBitangents[i].x;
      v.bitangent.y = mesh->mBitangents[i].y;
      v.bitangent.z = mesh->mBitangents[i].z;
    }

    if (mesh->HasVertexColors(0))
    {
      v.color.x = mesh->mColors[0][i].r;
      v.color.y = mesh->mColors[0][i].g;
      v.color.z = mesh->mColors[0][i].b;
    }
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace &face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      m.indices.push_back(face.mIndices[j]);
  }
};

void processNode(std::vector<Mesh> &meshes, aiNode *node, const aiScene *scene)
{
  unsigned int start = meshes.size();
  meshes.resize(start + scene->mNumMeshes);

  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(meshes[start + i], mesh, scene);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++)
    processNode(meshes, node->mChildren[i], scene);
}

void loadModel(const char *path, std::vector<Mesh> &meshes)
{
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    LOG_BREAK_BEFORE;
    LOG("Failed to load model:", importer.GetErrorString());
    LOG_BREAK_AFTER;
    return;
  }

  processNode(meshes, scene->mRootNode, scene);
}

Model::Model(unsigned int id, const char *filepath) : id(id)
{
  loadModel(filepath, meshes);
}

Model::~Model() {}

const unsigned int Model::getID()
{
  return id;
}

const unsigned int Model::createInstance()
{
  instances.emplace_back();

  const unsigned int id = instances.size() - 1;

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

const std::vector<Vertex> Model::getVertices() const
{
  std::vector<Vertex> vertices;

  size_t count = 0;
  for (const auto &mesh : meshes)
    count += mesh.vertices.size();

  vertices.reserve(count);

  for (const auto &mesh : meshes)
    vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());

  // for (size_t i = 0; i < meshes[0].vertices.size(); i++)
  // {
  //   std::cout << meshes[0].vertices[i].position.x << " " << meshes[0].vertices[i].position.y << " " << meshes[0].vertices[i].position.z << std::endl;
  // }

  return vertices;
}

const std::vector<unsigned int> Model::getIndices() const
{
  std::vector<unsigned int> indices;

  size_t count = 0;

  for (const auto &mesh : meshes)
    count += mesh.indices.size();

  indices.reserve(count);

  for (const auto &mesh : meshes)
    indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());

  return indices;
}