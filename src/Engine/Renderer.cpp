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

  assert(vbo.addPartition(0) == model->getID());
  vbo.upsert(0, model->getVertices(), model->getID());

  vao.set(0, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
  vao.set(1, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  vao.set(2, 2, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
}

void Renderer::upsertInstance(Model *model, Instance &instance, const unsigned int id)
{
  ibo.addPartition(0);
  ibo.upsert(sizeof(Instance), id, sizeof(instance), (const void *)&instance, model->getID());

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
  vao.bind();
  ebo.bind();

  for (const auto &model : models)
  {
    model->bindTextures();

    /**
     * TODO need to move all these functions inside model. Don't want to use Instance or Vertex here
     * All this crap should be moved to a Struct that gets updated whenever changes are made, it should not be
     * calculated on the fly.
     */
    const unsigned int indices = model->getIndicesCount();
    const unsigned int instances = model->getInstancesCount();
    const size_t indiceSizeOffset = ebo.getBufferPartitionOffsetSize(model->getID());
    const size_t vertexOffset = vbo.getBufferPartitionOffsetSize(model->getID()) / sizeof(Vertex);
    const size_t instanceOffset = ibo.getBufferPartitionOffsetSize(model->getID()) / sizeof(Instance);

    glDrawElementsInstancedBaseVertexBaseInstance((unsigned int)primitive, indices, GL_UNSIGNED_INT, (const void *)indiceSizeOffset, instances, vertexOffset, instanceOffset);
  }
}