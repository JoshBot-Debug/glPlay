#pragma once

#include <Window/Window.h>

#include "Renderer/Renderer.h"

class App : Window
{
private:
  Renderer renderer;

public:
  App();
  ~App() = default;

  void onDraw() override;
};