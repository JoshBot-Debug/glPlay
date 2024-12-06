#pragma once

#include "Model.h"
#include "Camera.h"
#include "Core/VertexArray.h"
#include "Core/ArrayBuffer.h"

class Renderer
{
private:
  VertexArray vao;
  ArrayBuffer vbo;
  ArrayBuffer ebo;
  ArrayBuffer ibo;

public:
  Renderer() : vbo(BufferTarget::ARRAY_BUFFER), ebo(BufferTarget::ELEMENT_ARRAY_BUFFER), ibo(BufferTarget::ARRAY_BUFFER)
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

  ~Renderer() {}

  void update(Camera *camera) {};

  void draw(const Primitive &primitive = Primitive::TRIANGLES) {};
};