#include "Renderer.h"

void Renderer::setCamera(const Camera *camera)
{
}

void Renderer::addLight(const Light *light)
{
}

void Renderer::addModel(const Model *model, const RenderType &type)
{
}

void Renderer::addFrameBuffer(const FrameBuffer *frameBuffer)
{
}

void Renderer::addShaderProgram(const ShaderProgram *shaderProgram)
{
}

void Renderer::createInstance(const std::string &name)
{
}

void Renderer::bindShader(const std::string &name)
{
}

void Renderer::updateCamera() const
{
}

void Renderer::updateLights() const
{
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

void Renderer::draw(const std::string &modelName)
{
}

void Renderer::applyPostProcessingEffects()
{
}
