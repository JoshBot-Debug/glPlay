#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Instance.h"
#include "Core/VertexArray.h"
#include "Core/ArrayBuffer.h"
#include "Core/Common.h"

struct IBOInfo
{
  unsigned int count = 0;         // The number of instances in the buffer
  unsigned int reservedCount = 1; // The number of instances space reserved in the buffer
};

class Renderer
{
private:
  Camera *camera;
  Shader shader;
  std::vector<Model *> models;

  VertexArray vao; // Vertext array
  ArrayBuffer vbo; // Vertext buffer
  ArrayBuffer ebo; // Element buffer
  ArrayBuffer ibo; // Instance buffer

  IBOInfo iboInfo;

private:
  void update();

public:
  Renderer();
  ~Renderer();

  void setCamera(Camera *camera);

  void addModel(Model *model);

  void draw(const Primitive &primitive = Primitive::TRIANGLES);

  Shader *getShader();
};