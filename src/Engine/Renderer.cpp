#include "Renderer.h"
#include "Model.h"

#include "Debug.h"

Renderer::Renderer() : vbo(BufferTarget::ARRAY_BUFFER), ebo(BufferTarget::ELEMENT_ARRAY_BUFFER), ibo(BufferTarget::ARRAY_BUFFER, VertexDraw::DYNAMIC)
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

void Renderer::upsertModel(Model *model)
{
  assert(ebo.addPartition(0) == model->getID());
  ebo.upsert(0, model->getIndices(), model->getID());
  model->setIndiceSizeOffset(ebo.getBufferPartitionOffsetSize(model->getID()));

  assert(vbo.addPartition(0) == model->getID());
  vbo.upsert(0, model->getVertices(), model->getID());
  model->setVertexOffset(vbo.getBufferPartitionOffsetSize(model->getID()) / sizeof(Vertex));

  vao.set(0, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
  vao.set(1, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  vao.set(2, 2, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
}

void Renderer::upsertInstance(Model *model, Instance &instance, const unsigned int id)
{
  ibo.addPartition(0);
  ibo.upsert(sizeof(Instance), id, sizeof(instance), (const void *)&instance, model->getID());
  model->setInstanceOffset(ibo.getBufferPartitionOffsetSize(model->getID()) / sizeof(Instance));

  vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
  vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
  vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
  vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);
}

void Renderer::update(const unsigned int partition, std::vector<Instance> instances)
{
  ibo.update(0, instances, partition);
}

void Renderer::draw(std::vector<Model *> &models, const Primitive &primitive)
{
  for (const auto &model : models)
  {
    model->bindTextures();
    // glMultiDrawElementsIndirect MAGIC!!!
    // Welcome to batching elements into one draw call!!
    // typedef struct
    // {
    //   uint count;
    //   uint instanceCount;
    //   uint firstIndex;
    //   int baseVertex;
    //   uint baseInstance;
    // } DrawElementsIndirectCommand;
    // glMultiDrawElementsIndirect accepts an array of structs like this!!!
    // It's exactly like glDrawElementsInstancedBaseVertexBaseInstance except it's multi!!
    // https://github.com/litasa/Advanced-OpenGL-Examples/blob/master/src/MultidrawIndirect/MultidrawIndirect.cpp

    glDrawElementsInstancedBaseVertexBaseInstance((unsigned int)primitive, model->getIndicesCount(), GL_UNSIGNED_INT, (const void *)model->getIndiceSizeOffset(), model->getInstancesCount(), model->getVertexOffset(), model->getInstanceOffset());
  }
}