#pragma once

#include <string>
#include <vector>

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"

class Engine
{
private:
  Shader shader;
  Camera *camera;
  std::vector<Model> models;
  std::vector<Texture2D> textures;

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
    return dynamic_cast<T *>(camera);
  }

  /**
   * @returns A pointer to a Model
   */
  Model *createModel(const char *path)
  {
    unsigned int id = models.size() + 1;
    models.emplace_back(id, path);
    return &models[id];
  }

  /**
   * @returns A pointer to a Model
   */
  Texture2D *createTexture2D(const char *path)
  {
    unsigned int id = textures.size() + 1;
    textures.emplace_back(id, path);
    return &textures[id];
  }

  /**
   * Get the shader manager to create shader programs
   * @returns The shader manager
   */
  Shader *getShader()
  {
    return &shader;
  }

  /**
   * Get the camera
   * @returns The camera
   */
  template <typename T>
  T *getCamera()
  {
    return camera;
  }

  /**
   * Get the Model by id
   * @returns The Model
   */
  Model *getModel(unsigned int id)
  {
    return &models[id];
  }

  /**
   * Gets all models
   * @returns A pointer to a vector of Model
   */
  std::vector<Model> *getModels()
  {
    return &models;
  }

  /**
   * Get the Texture2D by id
   * @returns The Texture2D
   */
  Texture2D *getTexture(unsigned int id)
  {
    return &textures[id];
  }

  /**
   * Gets all texture2ds
   * @returns A pointer to a vector of Texture2D
   */
  std::vector<Texture2D> *getTextures()
  {
    return &textures;
  }
};