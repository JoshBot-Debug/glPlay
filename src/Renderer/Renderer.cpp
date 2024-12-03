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

void Renderer::addLight(Light *light)
{
}

void Renderer::addModel(Model *model)
{
  vao.bind();

  models.insert({model->getName(), model});

  size_t vCount = 0;
  size_t iCount = 0;

  for (const auto &pair : models)
  {
    vCount += pair.second->getVertices().size();
    iCount += pair.second->getIndices().size();
  }

  LOG_BREAK_BEFORE;
  LOG("Model:", model->getName());
  LOG("Vertices:", vCount);
  LOG("Indices:", iCount);
  LOG("Vertex size:", sizeof(Vertex));
  LOG("Index size:", sizeof(unsigned int));
  LOG_BREAK_AFTER;

  vbo.resize(vCount * sizeof(Vertex));
  ebo.resize(iCount * sizeof(unsigned int));

  vbo.update(vCount - model->getVertices().size(), model->getVertices());
  ebo.update(iCount - model->getIndices().size(), model->getIndices());

  vbo.bind();
  vao.set(0, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
  vao.set(1, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  vao.set(2, 2, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

  model->setIndicesOffset(nextIndicesOffset);

  nextIndicesOffset = iCount * sizeof(unsigned int);
}

void Renderer::addFrameBuffer(const FrameBuffer *frameBuffer)
{
}

void Renderer::addShaderProgram(ShaderProgram *shaderProgram)
{
  shader = shaderProgram;
}

template <>
Instance &Renderer::add<Instance>(const std::string &model, const std::string &name)
{

  InstanceManager *iManager = models[model]->getInstanceManager(name);
  iManager->offset = (nextInstanceOffset += 1);

  // TODO max instances is doubled every time it's resized, not a greate idea but it works for now
  if ((nextInstanceOffset + 1) >= maxInstances)
  {
    LOG_BREAK_BEFORE;
    LOG("Resizing instance vertex buffer");
    LOG("Name:", name);
    LOG("Previous max instances:", maxInstances);
    LOG("Previous max instances size:", maxInstances * sizeof(Instance));

    ibo.resize((maxInstances *= 2) * sizeof(Instance), VertexDraw::DYNAMIC);

    LOG("Max instances:", maxInstances);
    LOG("Max instances size:", maxInstances * sizeof(Instance));
    LOG_BREAK_AFTER;

    vao.bind();
    vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
    vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
    vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
    vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);
  }

  ibo.update(iManager->offset * sizeof(Instance), sizeof(iManager->instance), &iManager->instance);

  return iManager->instance;
}

template <>
Instance &Renderer::get<Instance>(const std::string &model, const std::string &name)
{
  return models[model]->getInstanceManager(name)->instance;
}

void Renderer::update()
{
  const auto &dimensions = Window::GetDimensions();
  camera->setSize(dimensions.x, dimensions.y);

  shader->uniformMatrix4fv("u_ViewProjection", camera->getViewProjectionMatrix());

  // TODO Need to do a dirty check here
  // And do all the updates in one call
  for (const auto &pair : models)
    for (const auto &im : pair.second->getInstanceManagers())
      ibo.update(im->offset * sizeof(Instance), sizeof(im->instance), &im->instance);
}

void Renderer::bindFramebuffer(const std::string &name)
{
}

void Renderer::draw(const Primitive &primitive)
{
  update();

  for (const auto &pair : models)
  {
    Model *model = pair.second;
    model->bindTextures();
    glDrawElementsInstanced((unsigned int)primitive, model->getIndices().size(), GL_UNSIGNED_INT, (const void *)model->getIndicesOffset(), model->getInstancesCount());
  }
}

void Renderer::applyPostProcessingEffects()
{
}

ShaderProgram *Renderer::getShaderProgram()
{
  return shader;
}