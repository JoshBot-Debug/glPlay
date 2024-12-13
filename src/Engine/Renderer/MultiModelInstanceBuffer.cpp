#include "MultiModelInstanceBuffer.h"

MultiModelInstanceBuffer::MultiModelInstanceBuffer() : vbo(BufferTarget::ARRAY_BUFFER), ebo(BufferTarget::ELEMENT_ARRAY_BUFFER), ibo(BufferTarget::ARRAY_BUFFER, VertexDraw::DYNAMIC)
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

const unsigned int MultiModelInstanceBuffer::addBufferData(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, unsigned int &firstIndex, unsigned int &baseVertex)
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

  // for (size_t i = 0; i < 36; i++)
  // {
  //   std::cout << vertices[i].position.x << " " << vertices[i].position.y << " " << vertices[i].position.z << std::endl;
  // }

  // const auto &data = ebo.getBufferData<unsigned int>(0);

  // for (size_t i = 0; i < data.size(); i++)
  // {
  //   std::cout << data[i] << std::endl;
  // }

  // std::cout << data.size() - 36 << " " << data.size() << std::endl;

  // const auto &data = vbo.getBufferData<Vertex>(0);

  // for (size_t i = 0; i < data.size(); i++)
  // {
  //   std::cout << data[i].position.x << " " << data[i].position.y << " " << data[i].position.z << std::endl;
  // }

  return epID;
}

unsigned int MultiModelInstanceBuffer::add(const unsigned int partition, const Instance &instance, unsigned int &baseInstance)
{
  if (ibo.isNextPartition(partition))
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

std::vector<unsigned int> MultiModelInstanceBuffer::add(const unsigned int partition, const std::vector<Instance> &instances, unsigned int &baseInstance)
{
  if (ibo.isNextPartition(partition))
    ibo.addPartition(0);

  const unsigned int offset = ibo.getBufferPartitionSize(partition) / sizeof(Instance);

  ibo.upsert(offset, instances, partition);
  baseInstance = ibo.getBufferPartitionOffsetSize(partition) / sizeof(Instance);

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

void MultiModelInstanceBuffer::update(const unsigned int partition, const unsigned int offset, const Instance &instance)
{
  assert(ibo.partitionExists(partition));
  assert(offset < ibo.getBufferPartitionSize(partition));

  ibo.upsert(sizeof(Instance), offset, sizeof(instance), (const void *)&instance, partition);
}

void MultiModelInstanceBuffer::update(const unsigned int partition, const unsigned int offset, const std::vector<Instance> &instances)
{
  assert(ibo.partitionExists(partition));
  assert(offset < ibo.getBufferPartitionSize(partition));

  ibo.upsert(offset, instances, partition);
}

void MultiModelInstanceBuffer::bind() const
{
  vao.bind();
}

void MultiModelInstanceBuffer::unbind() const
{
  vbo.unbind();
  ebo.unbind();
  ibo.unbind();
  vao.unbind();
}
