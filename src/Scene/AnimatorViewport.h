#pragma once

#include "imgui.h"

#include "ECS/Registry.h"
#include "Window/Viewport.h"

class AnimatorViewport : public Viewport
{
private:
  Registry *registry;

public:
  AnimatorViewport() = default;
  ~AnimatorViewport() = default;

  void onInitialize(Registry *registry);
  void onUpdate() override;
  void onDrawViewport() override;
  void onResize(glm::vec2 size) override;
};