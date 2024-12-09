#pragma once

#include "Core/VertexArray.h"
#include "Core/ArrayBuffer.h"

struct Instance;

class InstanceBufferManager
{
private:
  VertexArray vao;
  ArrayBuffer vbo;
  ArrayBuffer ebo;
  ArrayBuffer ibo;

public:
  InstanceBufferManager();

  /**
   * @param vertices The virtex data
   * @param indices The indice data
   * 
   * @param firstIndex Fist index of the indice in the ebo.
   * @param baseVertex Fist index of the vertices in the vbo.
   *
   * @returns The partition ID which you can use to create instances of this mesh
   */
  const unsigned int addBufferData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int &firstIndex, int &baseVertex);

  /**
   * @param partition The partition we are creating or updating. Starts from 0, MUST GO IN SEQUENCE
   * @param instances The instance data
   *
   * @param baseInstance Fist index of the instance data in the instance vbo.
   * 
   * @returns The offset of the instance
   */
  unsigned int add(const unsigned int partition, Instance &instance, unsigned int &baseInstance);

  /**
   * @param partition The partition we are creating or updating. Starts from 0, MUST GO IN SEQUENCE
   * @param offset The offset in the partiton where you want to insert or update
   * @param instances The instance data
   * 
   * @param baseInstance Fist index of the instance data in the instance vbo.
   * 
   * @returns The offsets of the instances
   */
  std::vector<unsigned int> add(const unsigned int partition, std::vector<Instance> &instances, unsigned int &baseInstance);

  /**
   * @param partition The partition we are creating or updating. Starts from 0, MUST GO IN SEQUENCE
   * @param offset The offset in the partiton where you want to insert or update
   * @param instance The instance data
   */
  void update(const unsigned int partition, const unsigned int offset, Instance &instance);

  /**
   * @param partition The partition we are creating or updating. Starts from 0, MUST GO IN SEQUENCE
   * @param offset The offset in the partiton where you want to insert or update
   * @param instance The instance data
   */
  void update(const unsigned int partition, const unsigned int offset, std::vector<Instance> &instances);
};