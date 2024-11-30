#pragma once

#include <glm/gtx/transform.hpp>

struct Instance {

  glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

  glm::vec3 scale = glm::vec3(1.0f);

  glm::vec4 color = glm::vec4(1.0f);
};

struct InstanceManager
{
  Instance instance; 

  unsigned int offset;
};
