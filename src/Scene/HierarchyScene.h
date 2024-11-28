#pragma once

#include "Window/Scene.h"
#include "ECS/Registry.h"

class HierarchyScene : public Scene
{
private:
  Registry *registry;
  
public:
  HierarchyScene() = default;
  ~HierarchyScene() = default;

  void onInitialize(Registry *registry);
  void onDraw() override;
};