#pragma once

#include <string>
#include <unordered_map>

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
  std::unordered_map<std::string, Model *> models;

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

  template <typename T>
  T &add(const std::string &model, const std::string &name);

  template <typename T>
  T &get(const std::string &model, const std::string &name);

  void draw(const Primitive &primitive = Primitive::TRIANGLES);

  Shader *getShader();
};