#include "InstanceBufferManager.h"
#include "Model.h"

InstanceBufferManager::InstanceBufferManager() : vbo(BufferTarget::ARRAY_BUFFER), ebo(BufferTarget::ELEMENT_ARRAY_BUFFER), ibo(BufferTarget::ARRAY_BUFFER, VertexDraw::DYNAMIC)
{
  vao.generate();
  vbo.generate();
  ebo.generate();
  ibo.generate();

  vao.bind();

  vbo.bind();
  vao.set(0, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
  vao.set(1, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  vao.set(2, 2, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

  ibo.bind();
  vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
  vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
  vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
  vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);
}

const unsigned int InstanceBufferManager::addBufferData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int &firstIndex, int &baseVertex)
{
  const unsigned int epID = ebo.addPartition(0);
  const unsigned int vpID = vbo.addPartition(0);

  assert(epID == vpID);

  ebo.upsert(0, indices, epID);
  firstIndex = ebo.getBufferPartitionOffsetSize(epID) / sizeof(unsigned int);

  vbo.upsert(0, vertices, vpID);
  baseVertex = vbo.getBufferPartitionOffsetSize(vpID) / sizeof(Vertex);

  vao.set(0, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
  vao.set(1, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  vao.set(2, 2, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

  return epID;
}

unsigned int InstanceBufferManager::add(const unsigned int partition, Instance &instance, unsigned int &baseInstance)
{
  if (!ibo.partitionExists(partition))
    ibo.addPartition(0);

  const unsigned int offset = ibo.getBufferPartitionSize(partition) / sizeof(Instance);

  ibo.upsert(sizeof(Instance), offset, sizeof(instance), (const void *)&instance, partition);
  baseInstance = ibo.getBufferPartitionOffsetSize(partition) / sizeof(Instance);

  vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
  vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
  vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
  vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);

  return offset;
}

std::vector<unsigned int> InstanceBufferManager::add(const unsigned int partition, std::vector<Instance> &instances, unsigned int &baseInstance)
{
  if (!ibo.partitionExists(partition))
    ibo.addPartition(0);

  const unsigned int offset = ibo.getBufferPartitionSize(partition) / sizeof(Instance);

  ibo.upsert(offset, instances, partition);
  baseInstance = ibo.getBufferPartitionOffsetSize(partition) / sizeof(Instance); // Send to the outside setInstanceOffset

  vao.bind();
  vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
  vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
  vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
  vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);

  std::vector<unsigned int> offsets(instances.size());

  for (size_t i = 0; i < instances.size(); i++)
    offsets.push_back(offset + i);

  return offsets;
}

void InstanceBufferManager::update(const unsigned int partition, const unsigned int offset, Instance &instance)
{
  assert(ibo.partitionExists(partition));
  assert(offset < ibo.getBufferPartitionSize(partition));

  ibo.upsert(sizeof(Instance), offset, sizeof(instance), (const void *)&instance, partition);
}

void InstanceBufferManager::update(const unsigned int partition, const unsigned int offset, std::vector<Instance> &instances)
{
  assert(ibo.partitionExists(partition));
  assert(offset < ibo.getBufferPartitionSize(partition));

  ibo.upsert(offset, instances, partition);
}
