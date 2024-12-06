#pragma once

#include <Window/Window.h>

// #include "ECS/Registry.h"
#include "Engine/Engine.h"

// #include "Scene/AnimatorViewport.h"

// #include "DebugMenu.h"

class App : Window
{
private:
  Engine engine;
  // Registry registry;
  // AnimatorViewport animatorViewport;

  // DebugMenu debugMenu;

public:
  App();
  ~App() = default;

  void onUpdate() override;
  void onDraw() override;
};