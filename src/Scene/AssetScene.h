#pragma once

#include "imgui.h"

#include "Window/Scene.h"

class AssetScene : public Scene
{
private:

public:
  AssetScene() = default;
  ~AssetScene() = default;

  void onDraw() override;
  
  void DrawMenu();
};