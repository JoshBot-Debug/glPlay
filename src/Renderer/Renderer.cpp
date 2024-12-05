#include "Renderer.h"

#include <iostream>

#include "Debug.h"
#include "Window/Window.h"

Renderer::Renderer() : vbo(BufferTarget::ARRAY_BUFFER), ebo(BufferTarget::ELEMENT_ARRAY_BUFFER), ibo(BufferTarget::ARRAY_BUFFER)
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

Renderer::~Renderer()
{
}

void Renderer::setCamera(Camera *camera)
{
  this->camera = camera;
}

void Renderer::addModel(Model *model)
{
  vao.bind();

  models.push_back(model);

  size_t vCount = 0;
  size_t iCount = 0;

  for (const auto &model : models)
  {
    vCount += model->getVertices().size();
    iCount += model->getIndices().size();
  }

  LOG_BREAK_BEFORE;
  LOG("Vertices:", vCount);
  LOG("Indices:", iCount);
  LOG("Vertex size:", sizeof(Vertex));
  LOG("Index size:", sizeof(unsigned int));
  LOG_BREAK_AFTER;

  vbo.resize(vCount * sizeof(Vertex));
  ebo.resize(iCount * sizeof(unsigned int));

  int vertexOffset = vCount - model->getVertices().size();
  int indiceOffset = iCount - model->getIndices().size();

  vbo.update(vertexOffset, model->getVertices());
  ebo.update(indiceOffset, model->getIndices());

  vbo.bind();
  vao.set(0, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
  vao.set(1, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  vao.set(2, 2, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

  model->setIndiceOffset(indiceOffset);
  model->setVertexOffset(vertexOffset);
}

// template <>
// Instance &Renderer::add<Instance>(const std::string &modelName, const std::string &name)
// {
//   Model *model = models[modelName];
//   // model->setInstanceOffset();
  
//   InstanceManager *iManager = model->getInstanceManager(name);
//   iManager->offset = iboInfo.count;

//   // TODO max instances is doubled every time it's resized, not a great idea but it works for now
//   if ((iboInfo.count += 1) >= iboInfo.reservedCount)
//   {
//     LOG_BREAK_BEFORE;
//     LOG("Resizing instance vertex buffer");
//     LOG("Name:", name);
//     LOG("Previous max instances:", iboInfo.reservedCount);
//     LOG("Previous max instances size:", iboInfo.reservedCount * sizeof(Instance));

//     ibo.resize((iboInfo.reservedCount *= 2) * sizeof(Instance), VertexDraw::DYNAMIC);

//     LOG("Max instances:", iboInfo.reservedCount);
//     LOG("Max instances size:", iboInfo.reservedCount * sizeof(Instance));
//     LOG_BREAK_AFTER;

//     vao.bind();
//     vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
//     vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
//     vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
//     vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);
//   }

//   ibo.update(iManager->offset * sizeof(Instance), sizeof(iManager->instance), &iManager->instance);
//   // model->setInstanceOffset();

//   return iManager->instance;
// }


void Renderer::update()
{
  const auto &dimensions = Window::GetDimensions();
  camera->setSize(dimensions.x, dimensions.y);

  shader.uniformMatrix4fv("u_ViewProjection", camera->getViewProjectionMatrix());

  // TODO Need to do a dirty check here
  // And do all the updates in one call
  for (const auto &model : models)
    for (const auto &im : model->getInstanceManagers())
      ibo.update(im->offset * sizeof(Instance), sizeof(im->instance), &im->instance);
}

void Renderer::draw(const Primitive &primitive)
{
  update();

  const int eboSize = ebo.getBufferSize();
  const std::vector<unsigned int> eboData = ebo.getBufferData<unsigned int>();

  // LOG_BREAK_BEFORE;
  // LOG("EBO Size:", eboSize);
  // LOG("EBO data.size:", eboData.size());

  std::vector<unsigned int> square(eboData.begin() + 2880, eboData.begin() + 2916);
  std::vector<unsigned int> circle(eboData.begin(), eboData.begin() + 2880);

  glDrawElementsInstancedBaseVertexBaseInstance((unsigned int)primitive, 2880, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int)), 1, 0, 0);
  glDrawElementsInstancedBaseVertexBaseInstance((unsigned int)primitive, 36, GL_UNSIGNED_INT, (const void *)(2880 * sizeof(unsigned int)), 1, 1984, 1);

  // glDrawElementsInstancedBaseVertex((unsigned int)primitive, 36, GL_UNSIGNED_INT, (const void *)(2880 * sizeof(unsigned int)), 1, 1984);
  // glDrawElementsInstancedBaseVertex((unsigned int)primitive, 2880, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int)), 1, 0);

  // for (size_t i = 0; i < eboData.size(); i++)
  //   LOG("EBO v", i + 1, ":", eboData[i]);

  for (const auto &model : models)
  {
    model->bindTextures();

    // glDrawElementsInstancedBaseVertex((unsigned int)primitive, model->getIndices().size(), GL_UNSIGNED_INT, (const void *)(model->getIndiceOffset() * sizeof(unsigned int)), model->getInstancesCount(), model->getVertexOffset());

    // LOG("Model ebo size:", model->getIndices().size());
    // LOG("Model ebo offset(bytes):", model->getIndiceOffset());
    // LOG("Model vbo offset(bytes):", model->getVertexOffset());
    // LOG("Model instances:", model->getInstancesCount());
  }

  const int iboSize = ibo.getBufferSize();
  const std::vector<float> iboData = ibo.getBufferData<float>();

  // LOG("IBO Size:", iboSize);
  // LOG("IBO data.size:", iboData.size());

  // for (size_t i = 0; i < iboData.size() / 2; i++)
  //   LOG("IBO v", i + 1, ":", iboData[i]);

  // LOG_BREAK_AFTER;
}

Shader *Renderer::getShader()
{
  return &shader;
}