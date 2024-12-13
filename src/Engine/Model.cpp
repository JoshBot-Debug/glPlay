#include "Model.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Debug.h"

void loadModel(const char *path, std::vector<Mesh> &meshes)
{
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes);

  // const aiScene *scene = importer.ReadFile(path,
  //                                          aiProcess_Triangulate |
  //                                              aiProcess_FlipUVs |
  //                                              aiProcess_GenSmoothNormals |
  //                                              aiProcess_OptimizeMeshes |
  //                                              aiProcess_JoinIdenticalVertices);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
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

    m.vertices.resize(mesh->mNumVertices);

    for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
    {
      Vertex &v = m.vertices[j];

      const aiVector3D &vertex = mesh->mVertices[j];
      v.position.x = vertex.x;
      v.position.y = vertex.y;
      v.position.z = vertex.z;

    std::cout <<  v.position.x << " " << v.position.y << " " << v.position.z << std::endl;


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
        m.indices.push_back(face.mIndices[k]);
    }
  }


  for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
  {
    aiMaterial *material = scene->mMaterials[i];
    aiString texturePath;

    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
    {
      if (texturePath.C_Str()[0] == '*')
      {
        int textureIndex = std::atoi(texturePath.C_Str() + 1);
        if (textureIndex < scene->mNumTextures)
        {
          const aiTexture *embeddedTexture = scene->mTextures[textureIndex];
          LOG_BREAK_BEFORE;
          LOG("Embedded texture found, size:", embeddedTexture->mWidth);
          LOG_BREAK_AFTER;
        }
      }
    }
  }
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