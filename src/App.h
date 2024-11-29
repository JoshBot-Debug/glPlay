#pragma once

#include <Window/Window.h>

#include "ECS/Registry.h"
#include "Renderer/Renderer.h"
#include "Scene/AnimatorViewport.h"

class App : Window
{
private:
  Renderer renderer;
  Registry registry;
  AnimatorViewport animatorViewport;

public:
  App();
  ~App() = default;

  void onDraw() override;
};