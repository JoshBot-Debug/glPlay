#pragma once

#include <Window/Window.h>

#include "Engine/ResourceManager.h"
#include "Engine/Camera/PerspectiveCamera.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/MultiModelInstanceBuffer.h"

#include "EngineControlPanel.h"

class App : Window
{
private:
  ResourceManager resource;
  PerspectiveCamera camera;

  std::vector<MultiModelInstanceBuffer> buffers;
  std::vector<DrawElementsIndirectCommand> instancedCommands;

  EngineControlPanel controlPanel;

public:
  App();
  ~App() = default;

  void onUpdate() override;
  void onDraw() override;
};