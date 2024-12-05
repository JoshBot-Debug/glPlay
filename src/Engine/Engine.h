#pragma once

#include <string>
#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

class Engine
{
private:
  Shader *shader;
  Camera *camera;
  std::vector<Model> models;

public:
  Engine() = default;
  ~Engine() = default;

  /**
   * Creates an instance of Camera and returns a pointer
   * @param T An instance of Model
   * @returns Camera pointer
   */
  template <typename T>
  const T *createCamera() const
  {
    static_assert(std::is_base_of<Camera, T>::value, "T must implement the Camera interface.");
    camera = new T();
    return camera;
  }

  /**
   * Creates an instance of Model and returns the ID
   * @param T An instance of Model
   * @returns Model ID
   */
  template <typename T>
  const unsigned int createModel() const
  {
    static_assert(std::is_base_of<Model, T>::value, "T must implement the Model interface.");
    models.push_back(new T());
    return models.size() - 1;
  }

  /**
   * Creates an instance of Model and returns the ID
   * @param T An instance of Model
   * @returns Model ID
   */
  template <typename T>
  const unsigned int createShader() const
  {
    static_assert(std::is_base_of<Shader, T>::value, "T must implement the Shader interface.");
    shader = new T();
    return shader;
  }
};