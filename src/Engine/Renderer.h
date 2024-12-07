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

  void upsertInstance(Model *model, Instance *instance);

  void draw(const Primitive &primitive = Primitive::TRIANGLES);
};