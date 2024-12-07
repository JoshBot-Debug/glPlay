#pragma once

#include <Window/Window.h>

#include "Engine/Engine.h"
#include "EngineControlPanel.h"

class App : Window
{
private:
  Engine engine;
  EngineControlPanel controlPanel;

public:
  App();
  ~App() = default;

  void onUpdate() override;
  void onDraw() override;
};