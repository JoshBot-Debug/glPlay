#pragma once

#include <string>
#include <vector>

#include "Renderer.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Material.h"

class Engine
{
private:
  Renderer renderer;
  Shader *shader;
  Camera *camera;
  std::vector<Model *> models;
  std::vector<Material *> materials;
  std::vector<Texture2D *> textures;

public:
  Engine() : shader(new Shader()) {};

  ~Engine()
  {
    for (const auto *ptr : models)
      delete ptr;
    for (const auto *ptr : textures)
      delete ptr;
    for (const auto *ptr : materials)
      delete ptr;
    delete camera;
    delete shader;
  };

  /**
   * Get the shader manager to create shader programs
   * @returns The shader manager
   */
  Shader *getShader()
  {
    return shader;
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

  /**
   * @returns A pointer to a Model
   */
  Model *createModel(const char *path)
  {
    unsigned int id = models.size();
    models.push_back(new Model(id, path));
    return models[id];
  }

  /**
   * Get the Model by id
   * @returns The Model
   */
  Model *getModel(unsigned int id)
  {
    return models.at(id);
  }

  /**
   * Gets all models
   * @returns A vector of Model pointers
   */
  std::vector<Model *> &getModels()
  {
    return models;
  }

  /**
   * @returns A pointer to a Model
   */
  Texture2D *createTexture2D(const char *path)
  {
    unsigned int id = textures.size();
    textures.push_back(new Texture2D(id, path));
    return textures.at(id);
  }

  /**
   * Get the Texture2D by id
   * @returns The Texture2D
   */
  Texture2D *getTexture2D(unsigned int id)
  {
    return textures.at(id);
  }

  /**
   * Gets all texture2ds
   * @returns A vector of Texture2D pointers
   */
  std::vector<Texture2D *> &getTexture2Ds()
  {
    return textures;
  }

  /**
   * @returns A pointer to a Material
   */
  Material *createMaterial(const char *path)
  {
    unsigned int id = materials.size();
    materials.push_back(new Material(id));
    return materials.at(id);
  }

  /**
   * Get the Material by id
   * @returns The Material
   */
  Material *getMaterial(unsigned int id)
  {
    return materials.at(id);
  }

  /**
   * Gets all Materials
   * @returns A vector of Material pointers
   */
  std::vector<Material *> &getMaterials()
  {
    return materials;
  }

  void update() {
    // TODO Need to do a dirty check here
    // And do all the updates in one call
    // for (const auto &model : models)
    //   for (const auto &im : model->getInstanceManagers())
    //     ibo.update(im->offset * sizeof(Instance), sizeof(im->instance), &im->instance);
  };

  void begin()
  {
    shader->setUniformMatrix4fv("u_ViewProjection", camera->getViewProjectionMatrix());
  };

  void draw(const Primitive &primitive = Primitive::TRIANGLES) { renderer.draw(primitive); };
};