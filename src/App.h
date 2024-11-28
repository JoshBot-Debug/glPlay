#pragma once

#include <Window/Window.h>

#include "Renderer/Renderer.h"

const WindowOptions options = {.title = "OpenGL", .width = 800, .height = 600, .imguiEnableDocking = true};

class App : Window
{
private:
  Renderer renderer;

public:
  App();
  ~App() = default;

  void onDraw() override;
};