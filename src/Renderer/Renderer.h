#pragma once

#include <string>

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "FrameBuffer.h"

enum class RenderType
{
  Instanced
};

class Renderer
{
private:
public:
  Renderer() = default;
  ~Renderer() = default;

  void setCamera(const Camera *camera);
  
  void addLight(const Light *light);
  void addModel(const Model *model, const RenderType &type);
  void addFrameBuffer(const FrameBuffer *frameBuffer);
  void addShaderProgram(const ShaderProgram *shaderProgram);

  void createInstance(const std::string &name);

  void bindShader(const std::string &name);

  void updateCamera() const;
  void updateLights() const;

  void updateInstance(const std::string &modelName, const void *buffer);
  void updateInstance(const std::string &modelName, const std::string &instanceName, const void *buffer);

  void bindFramebuffer(const std::string &name);

  void draw(const std::string &modelName);

  void applyPostProcessingEffects();
};