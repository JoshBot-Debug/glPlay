#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "FrameBuffer.h"
#include "Instance.h"
#include "Core/VertexArray.h"
#include "Core/ArrayBuffer.h"
#include "Core/Common.h"

class Renderer
{
private:
  Camera *camera;
  ShaderProgram *shader;
  std::unordered_map<std::string, Model *> models;

  VertexArray vao; // Vertext array
  ArrayBuffer vbo; // Vertext buffer
  ArrayBuffer ebo; // Element buffer
  ArrayBuffer ibo; // Instance buffer

  size_t maxInstances = 1;       // The number of indices to draw
  size_t nextInstanceOffset = -1; // The next offset in the instance vertex buffer
  size_t nextIndicesOffset = 0; // The next offset in the instance vertex buffer

private:
  void update();

public:
  Renderer();
  ~Renderer();

  void setCamera(Camera *camera);

  void addLight(Light *light);
  void addModel(Model *model);
  void addFrameBuffer(const FrameBuffer *frameBuffer);
  void addShaderProgram(ShaderProgram *shaderProgram);

  template <typename T>
  Instance &add(const std::string &model, const std::string &name);

  template <typename T>
  Instance &get(const std::string &model, const std::string &name);

  void bindFramebuffer(const std::string &name);

  void draw(const Primitive &primitive = Primitive::TRIANGLES);

  void applyPostProcessingEffects();

  ShaderProgram *getShaderProgram();
};