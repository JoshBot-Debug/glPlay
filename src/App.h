#pragma once

#include <Window/Window.h>

#include "ECS/Registry.h"
#include "Renderer/Renderer.h"
#include "Scene/AnimatorViewport.h"

#include "DebugMenu.h"

class App : Window
{
private:
  Renderer renderer;
  Registry registry;
  AnimatorViewport animatorViewport;

  Camera camera;
  DebugMenu debugMenu;

public:
  App();
  ~App() = default;

  // void onUpdate() override;
  void onDraw() override;
};