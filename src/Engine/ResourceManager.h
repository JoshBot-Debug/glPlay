#pragma once

#include <string>
#include <unordered_map>

#include "Model.h"

class ResourceManager
{
private:
  std::vector<Model *> models;

public:
  ResourceManager() = default;

  ResourceManager(const ResourceManager &) = delete;

  ~ResourceManager();

  /**
   * @returns A pointer to a Model
   */
  Model *loadModel(const char *path)
  {
    unsigned int id = models.size();

    Model *model = new Model(id, path);

    models.push_back(model);

    return model;
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
};