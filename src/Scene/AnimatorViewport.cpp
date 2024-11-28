#include <string>

#include "AnimatorViewport.h"

void AnimatorViewport::onInitialize(Registry *registry)
{
  this->setTitle("Animator");
  this->registry = registry;
}

void AnimatorViewport::onUpdate()
{
}

void AnimatorViewport::onDrawViewport()
{
}

void AnimatorViewport::onResize(glm::vec2 size)
{
}