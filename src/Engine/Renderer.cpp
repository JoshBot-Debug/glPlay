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

void Renderer::upsertInstance(Model *model, Instance &instance, unsigned int id)
{
  assert(ibo.addPartition(0) == model->getID());
  ibo.upsert(sizeof(Instance), id, sizeof(instance), (const void *)&instance, model->getID());

  vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
  vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
  vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
  vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);

  // debug(model->getID());
  // Instance* inst = new Instance();
  // inst->color.a = 99.0f;

  // std::cout << sizeof(Instance) << std::endl;
  // ibo.update(0, std::vector{inst}, model->getID());
  // debug(model->getID());
}

void Renderer::update(unsigned int partition, std::vector<Instance> instances)
{
  ibo.update(0, instances, partition);
}

void Renderer::draw(std::vector<Model *> &models, const Primitive &primitive)
{
  vao.bind();
  ebo.bind();

  glDrawElementsInstancedBaseVertexBaseInstance((unsigned int)primitive, 36, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int)), 1, 0, 0);


  // glDrawElementsInstancedBaseVertexBaseInstance((unsigned int)primitive, 2880, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int)), 1, 0, 0);
  // glDrawElementsInstancedBaseVertexBaseInstance((unsigned int)primitive, 36, GL_UNSIGNED_INT, (const void *)(2880 * sizeof(unsigned int)), 1, 1984, 1);

  for (const auto &model : models)
  {
    model->bindTextures();

    // glDrawElementsInstancedBaseVertex((unsigned int)primitive, model->getIndices().size(), GL_UNSIGNED_INT, (const void *)(model->getIndiceOffset() * sizeof(unsigned int)), model->getInstanceCount(), model->getVertexOffset());

    // LOG("Model ebo size:", model->getIndices().size());
    // LOG("Model ebo offset(bytes):", model->getIndiceOffset());
    // LOG("Model vbo offset(bytes):", model->getVertexOffset());
    // LOG("Model instances:", model->getInstanceCount());
  }
}

void Renderer::debug(unsigned int partition)
{
  std::vector<float> instances = ibo.getBufferData<float>(partition);

  for (const auto &instanceData : instances)
  {
    std::cout << instanceData << std::endl;
  }
}
