#pragma once

#include <string>
#include <vector>

#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "Batch/Batch.h"

class Engine
{
private:
  Shader shader;
  Camera *camera;
  std::vector<Batch> batches;

public:
  Engine() = default;
  ~Engine() = default;

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
    return camera;
  }

  /**
   * Creates an instance of Batch, used to batch models to together
   *
   * @returns A pointer to a Batch
   */
  Batch *createBatch()
  {
    batches.emplace_back(batches.size());
    return &batches[batches.size() - 1];
  }

  /**
   * Get the shader manager to create shader programs
   * @returns The shader manager
   */
  Shader *getShader()
  {
    return &shader;
  }
};