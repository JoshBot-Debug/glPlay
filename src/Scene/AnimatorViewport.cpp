#include <string>

#include "AnimatorViewport.h"

#include "Window/Input.h"
#include "Window/Time.h"

void AnimatorViewport::onInitialize(Renderer *renderer, Registry *registry)
{
  this->setTitle("Animator");
  this->registry = registry;
  this->renderer = renderer;
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