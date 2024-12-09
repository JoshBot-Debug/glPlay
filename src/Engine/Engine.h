#pragma once

#include <string>
#include <vector>

#include "Model.h"
#include "Shader.h"
#include "Camera/Camera.h"
#include "ResourceManager.h"

class Engine
{
private:
  Camera *camera;
  Shader shader;
  ResourceManager resourceManager;

public:
  ~Engine()
  {
    delete camera;
  };

  /**
   * Get the shader manager to create shader programs
   * @returns The shader manager
   */
  Shader *getShader()
  {
    return &shader;
  }

  /**
   * Creates an instance of Camera and returns a pointer
   * @param T An instance of Model
   * @returns Camera pointer
   */
  template <typename T>
  T *createCamera()
  {
    static_assert(std::is_base_of<Camera, T>::value, "T must implement the Camera interface.");
    camera = new T();
    return dynamic_cast<T *>(camera);
  }

  /**
   * Get the camera
   * @returns The camera
   */
  template <typename T>
  T *getCamera()
  {
    return dynamic_cast<T *>(camera);
  }

  ResourceManager *getResourceManager()
  {
    return &resourceManager;
  }

  void update()
  {
    camera->update();

    // TODO Need to do a dirty check here
    // And do all the updates in one call
    // for (const auto &model : models)
    //   renderer.update(model->getID(), model->getInstances());
  };

  void begin()
  {
    shader.setUniformMatrix4fv("u_ViewProjection", camera->getViewProjectionMatrix());
  };

  void draw(const Primitive &primitive = Primitive::TRIANGLES) {

  };
};