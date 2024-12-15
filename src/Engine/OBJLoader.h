#pragma once

#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Types.h"
#include "Debug.h"

struct MeshData
{
  std::string name;
  unsigned int facesIndex;
  std::vector<unsigned int> indices;
};

class OBJLoader
{
private:
  std::ifstream file;
  std::string filepath;

  // Name, faces start index, faces batch size (3/4)
  std::vector<MeshData> meshes;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> texCoords;
  std::vector<glm::vec3> normals;
  std::vector<glm::ivec3> faces;

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

      // if (command == "mtllib")
      //   while (iss >> word)
      //     const std::string path = (filepath.substr(0, filepath.find_last_of('/')) + "/" + text);

      if (command == "o")
      {
        iss >> word;
        meshes.emplace_back(word, faces.size());
      }

      if (command == "v")
      {
        unsigned int index = 0;
        glm::vec3 &position = positions.emplace_back();
        while (iss >> word)
        {
          position[index] = std::stof(word);
          ++index;
        }
      }

      if (command == "vt")
      {
        unsigned int index = 0;
        glm::vec2 &texCoord = texCoords.emplace_back();
        while (iss >> word)
        {
          texCoord[index] = std::stof(word);
          ++index;
        }
      }

      if (command == "vn")
      {
        unsigned int index = 0;
        glm::vec3 &normal = normals.emplace_back();
        while (iss >> word)
        {
          normal[index] = std::stof(word);
          ++index;
        }
      }

      // if (command == "g")
      //   g(word);

      // if (command == "usemtl")
      //   usemtl(word);

      // if (command == "s")
      //   s(word);

      if (command == "f")
      {
        std::vector<unsigned int> vertices;
        while (iss >> word)
        {
          glm::ivec3 &face = faces.emplace_back();
          getFace(face, word);
          vertices.push_back(face[0]);
        }
        processIndices(vertices);
      }
    }
  }

  void processIndices(const std::vector<unsigned int> &vertices)
  {
    MeshData &mesh = meshes.back();

    if (vertices.size() == 3)
      mesh.indices = vertices;
    else if (vertices.size() == 4)
    {
      mesh.indices.push_back(vertices[0]);
      mesh.indices.push_back(vertices[1]);
      mesh.indices.push_back(vertices[2]);

      mesh.indices.push_back(vertices[0]);
      mesh.indices.push_back(vertices[2]);
      mesh.indices.push_back(vertices[3]);
    }
  }

  void getFace(glm::ivec3 &face, const std::string &text)
  {
    std::stringstream ss(text);
    std::string token;
    std::vector<std::string> tokens;

    unsigned int index = 0;

    while (std::getline(ss, token, '/'))
    {
      face[index] = std::stoi(token) - 1;
      ++index;
    }
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

  unsigned int getMeshCount()
  {
    return meshes.size();
  };

  void getName(unsigned int meshIndex, std::string &name)
  {
    name = meshes[meshIndex].name;
  }

  void getVertices(unsigned int meshIndex, std::vector<Vertex> &vertices)
  {
    const MeshData &mesh = meshes[meshIndex];
    const unsigned int faceSize = meshes.size() > meshIndex + 1 ? meshes[meshIndex + 1].facesIndex : faces.size();

    std::cout << mesh.name << " " << mesh.facesIndex << " " << faceSize << std::endl;

    for (size_t i = mesh.facesIndex; i < faceSize; i++)
    {
      Vertex &vertex = vertices.emplace_back();

      vertex.position = positions[faces[i][0]];
      vertex.texCoord = texCoords[faces[i][1]];
      vertex.normal = normals[faces[i][2]];
    }
  }

  void getIndices(unsigned int meshIndex, std::vector<unsigned int> &indices)
  {
    indices = meshes[meshIndex].indices;
  }
};