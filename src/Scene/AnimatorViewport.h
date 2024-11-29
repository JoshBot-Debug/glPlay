#pragma once

#include "imgui.h"

#include "ECS/Registry.h"
#include "Window/Viewport.h"
#include "Renderer/Renderer.h"

class AnimatorViewport : public Viewport
{
private:
  Registry *registry;
  Renderer *renderer;

public:
  AnimatorViewport() = default;
  ~AnimatorViewport() = default;

  void onInitialize(Renderer *renderer, Registry *registry);
  void onUpdate() override;
  void onDrawViewport() override;
  void onResize(glm::vec2 size) override;
};