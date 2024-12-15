#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "Types.h"
#include "Debug.h"

enum class Indexer : unsigned int
{
  COUNT_MESH,
  COUNT_V,
  COUNT_VN,
  V,
  VT,
  VN,
  F,
  VERTEX_POSITION,
  VERTEX_TEXCOORD,
  VERTEX_NORMAL,
};

class OBJLoader
{
private:
  std::ifstream file;
  std::string filepath;
  std::vector<Mesh> meshes;

  std::vector<glm::vec3> normals;

  std::vector<unsigned int> index = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  void read()
  {
    std::string line;
    while (std::getline(file, line))
    {
      if (line.starts_with("#"))
        continue;

      std::istringstream iss(line);
      std::string command, word;
      iss >> command;

      if (command == "mtllib")
        while (iss >> word)
          mtllib(word);

      if (command == "o")
        o(word);

      if (command == "v")
      {
        index[(int)Indexer::VERTEX_POSITION] = 0;
        while (iss >> word)
          v(word);
        ++index[(int)Indexer::V];
      }

      if (command == "vt")
      {
        index[(int)Indexer::VERTEX_TEXCOORD] = 0;
        while (iss >> word)
          vt(word);
        ++index[(int)Indexer::VT];
      }

      if (command == "vn")
      {
        index[(int)Indexer::VERTEX_NORMAL] = 0;
        while (iss >> word)
          vn(word);
        ++index[(int)Indexer::VN];
      }

      if (command == "g")
        g(word);

      if (command == "usemtl")
        usemtl(word);

      if (command == "s")
        s(word);

      if (command == "f")
      {
        std::vector<unsigned int> vertexIndices;
        while (iss >> word)
          vertexIndices.push_back(f(word));
        generateIndices(vertexIndices);
        ++index[(int)Indexer::F];
      }
    }
  }

  void mtllib(const std::string &text)
  {
    const std::string path = (filepath.substr(0, filepath.find_last_of('/')) + "/" + text);
  }

  void o(const std::string &text)
  {
    ++index[(int)Indexer::COUNT_MESH];
    Mesh &mesh = meshes.emplace_back();
    mesh.name = text;

    normals.clear();
    index[(int)Indexer::COUNT_V] += index[(int)Indexer::V];
    index[(int)Indexer::COUNT_VN] += index[(int)Indexer::VN];
    index[(int)Indexer::V] = 0;
    index[(int)Indexer::VT] = 0;
    index[(int)Indexer::VN] = 0;
    index[(int)Indexer::F] = 0;
  }

  void v(const std::string &text)
  {
    Vertex &vertex = getCurrentVertex(Indexer::V);
    vertex.position[index[(int)Indexer::VERTEX_POSITION]] = std::stof(text);
    ++index[(int)Indexer::VERTEX_POSITION];
  }

  void vt(const std::string &text)
  {
    Vertex &vertex = getCurrentVertex(Indexer::VT);
    vertex.texCoord[index[(int)Indexer::VERTEX_TEXCOORD]] = std::stof(text);
    ++index[(int)Indexer::VERTEX_TEXCOORD];
  }

  void vn(const std::string &text)
  {
    glm::vec3 &normal = getCurrentLocalNormal();
    normal[index[(int)Indexer::VERTEX_NORMAL]] = std::stof(text);
    ++index[(int)Indexer::VERTEX_NORMAL];
  }

  void g(const std::string &text) {}
  void usemtl(const std::string &text) {}
  void s(const std::string &text) {}

  unsigned int f(const std::string &text)
  {
    Mesh &mesh = meshes.back();

    std::stringstream ss(text);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, '/'))
      tokens.push_back(token);

    unsigned int vertexIndex = std::stoi(tokens[0]) - 1 - index[(int)Indexer::COUNT_V];
    unsigned int normalIndex = std::stoi(tokens[2]) - 1 - index[(int)Indexer::COUNT_VN];

    Vertex &vertexNormal = mesh.vertices[vertexIndex];
    vertexNormal.normal = normals[normalIndex];

    return vertexIndex + index[(int)Indexer::COUNT_V];
  }

  void generateIndices(std::vector<unsigned int> vertexIndices)
  {
    Mesh &mesh = meshes.back();

    if (vertexIndices.size() == 3)
      mesh.indices = vertexIndices;
    else if (vertexIndices.size() == 4)
    {
      mesh.indices.push_back(vertexIndices[0]);
      mesh.indices.push_back(vertexIndices[1]);
      mesh.indices.push_back(vertexIndices[2]);

      mesh.indices.push_back(vertexIndices[0]);
      mesh.indices.push_back(vertexIndices[2]);
      mesh.indices.push_back(vertexIndices[3]);
    }
  }

  Vertex &getCurrentVertex(Indexer vType)
  {
    Mesh &mesh = meshes.back();
    if (mesh.vertices.size() <= index[(int)vType])
      return mesh.vertices.emplace_back();
    return mesh.vertices[index[(int)vType]];
  }

  glm::vec3 &getCurrentLocalNormal()
  {
    if (normals.size() <= index[(int)Indexer::VN])
      return normals.emplace_back();
    return normals[index[(int)Indexer::VN]];
  }

public:
  OBJLoader(const std::string &filepath) : file(std::ifstream(filepath, std::ios::binary)), filepath(filepath)
  {
    if (!file)
    {
      LOG_BREAK_BEFORE;
      LOG("Failed to read obj file:", filepath);
      LOG_BREAK_AFTER;
    }

    read();
  }

  ~OBJLoader()
  {
    file.close();
  }

  std::vector<Mesh> &getMeshes()
  {
    return meshes;
  };
};