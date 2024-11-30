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

  // vbo.set(0, nullptr);
  // ebo.set(0, nullptr);
  // ibo.set(0, nullptr);

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

void Renderer::addLight(const Light *light)
{
}

void Renderer::addModel(const Model *model)
{
  vao.bind();

  models.push_back(model);

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  size_t vCount = 0;
  size_t iCount = 0;
  for (const auto &model : models)
  {
    vCount += model->getVertices().size();
    iCount += model->getIndices().size();
  }

  vertices.reserve(vCount);
  indices.reserve(iCount);

  for (const auto &model : models)
  {
    const auto &v = model->getVertices();
    const auto &i = model->getIndices();
    vertices.insert(vertices.begin(), v.begin(), v.end());
    indices.insert(indices.begin(), i.begin(), i.end());
  }

  LOG_BREAK_BEFORE;
  LOG("Model:", model->getName());
  LOG("Vertices: ", vertices.size());
  LOG("Indices: ", indices.size());
  LOG("Vertex size: ", sizeof(Vertex));
  LOG("Index size: ", sizeof(unsigned int));
  LOG_BREAK_AFTER;

  vbo.resize(vertices.size() * sizeof(Vertex));
  ebo.resize(indices.size() * sizeof(unsigned int));

  vbo.update(vertices.size() - model->getVertices().size(), model->getVertices());
  ebo.update(indices.size() - model->getIndices().size(), model->getIndices());

  // TODO Need to improve
  vbo.bind();
  vao.set(0, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, position));
  vao.set(1, 3, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  vao.set(2, 2, VertexType::FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

  this->indices = indices.size();
}

void Renderer::addFrameBuffer(const FrameBuffer *frameBuffer)
{
}

void Renderer::addShaderProgram(ShaderProgram *shaderProgram)
{
  shader = shaderProgram;
}

template <>
Instance &Renderer::add<Instance>(const std::string &name)
{
  InstanceManager &iManager = instances[name];
  iManager.offset = instances.size() - 1;

  // TODO max instances is doubled every time it's resized, not a greate idea but it works for now
  if (instances.size() >= maxInstances)
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

    // TODO This is done above, need to refactor
    // Setup vertex attrib pointers
    vao.bind();
    vao.set(3, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, translate), 1);
    vao.set(4, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, rotation), 1);
    vao.set(5, 3, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, scale), 1);
    vao.set(6, 4, VertexType::FLOAT, false, sizeof(Instance), (void *)offsetof(Instance, color), 1);
  }

  ibo.update(iManager.offset * sizeof(Instance), sizeof(iManager.instance), &iManager.instance);

  return iManager.instance;
}

template <>
Instance &Renderer::get<Instance>(const std::string &name)
{
  return instances[name].instance;
}

void Renderer::update()
{
  const auto &dimensions = Window::GetDimensions();
  camera->setSize(dimensions.x, dimensions.y);
  camera->update();

  shader->uniformMatrix4fv("u_ViewProjection", camera->getViewProjectionMatrix());

  // TODO Need to do a dirty check here
  // And do all the updates in one call
  for (const auto &im : instances)
    ibo.update(im.second.offset * sizeof(Instance), sizeof(im.second.instance), &im.second.instance);
}

void Renderer::updateInstance(const std::string &modelName, const void *buffer)
{
}

void Renderer::updateInstance(const std::string &modelName, const std::string &instanceName, const void *buffer)
{
}

void Renderer::bindFramebuffer(const std::string &name)
{
}

void Renderer::draw(const std::string &modelName, const Primitive &primitive)
{
  update();

  vao.bind();
  ebo.bind();
  glDrawElementsInstanced((unsigned int)primitive, indices, GL_UNSIGNED_INT, 0, instances.size());
}

void Renderer::applyPostProcessingEffects()
{
}

ShaderProgram *Renderer::getShaderProgram()
{
  return shader;
}