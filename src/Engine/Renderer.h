#pragma once

#include "Core/VertexArray.h"
#include "Core/ArrayBuffer.h"

class Model;
struct Instance;

class Renderer
{
public:
  VertexArray vao;
  ArrayBuffer vbo;
  ArrayBuffer ebo;
  ArrayBuffer ibo;

  Renderer();

  void upsertModel(Model *model);

  void upsertInstance(Model *model, Instance &instance, unsigned int id);

  void update(unsigned int partition, std::vector<Instance> instances);

  void draw(std::vector<Model *> &models, const Primitive &primitive = Primitive::TRIANGLES);

  void debug(unsigned int partition);
};