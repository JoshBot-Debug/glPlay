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
  std::vector<const Model *> models;
  std::unordered_map<std::string, InstanceManager> instances;

  VertexArray vao; // Vertext array
  ArrayBuffer vbo; // Vertext buffer
  ArrayBuffer ebo; // Element buffer
  ArrayBuffer ibo; // Instance buffer

  size_t indices;          // The number of indices to draw
  size_t maxInstances = 1; // The number of indices to draw

private:
  void update();

public:
  Renderer();
  ~Renderer();

  void setCamera(Camera *camera);

  void addLight(const Light *light);
  void addModel(const Model *model);
  void addFrameBuffer(const FrameBuffer *frameBuffer);
  void addShaderProgram(ShaderProgram *shaderProgram);

  template <typename T>
  Instance &add(const std::string &name);

  template <typename T>
  Instance &get(const std::string &name);

  void updateInstance(const std::string &modelName, const void *buffer);
  void updateInstance(const std::string &modelName, const std::string &instanceName, const void *buffer);

  void bindFramebuffer(const std::string &name);

  void draw(const std::string &modelName, const Primitive &primitive = Primitive::TRIANGLES);

  void applyPostProcessingEffects();

  ShaderProgram *getShaderProgram();
};