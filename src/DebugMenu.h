#pragma once

#include <string>
#include <unordered_map>

#include "Renderer/Camera.h"
#include "Renderer/Instance.h"
#include "Renderer/Shader.h"
#include "imgui.h"

inline const char *cameraTypes[] =
    {
        "Orthographic",
        "Perspective"};

class DebugMenu
{
private:
  Camera *camera;
  ShaderProgram *shaderProgram;
  std::unordered_map<std::string, Instance *> instances;

public:
  DebugMenu() {}

  void setCamera(Camera *camera)
  {
    this->camera = camera;
  }

  void addShaderProgram(ShaderProgram *shaderProgram)
  {
    this->shaderProgram = shaderProgram;
  }

  void addInstance(const std::string &name, Instance *instance)
  {
    this->instances[name] = instance;
  }

  void shaderMenu()
  {
    ImGui::Begin("Shaders");

    if (ImGui::Button("Recompile shaders"))
      shaderProgram->recompile();

    ImGui::End();
  }

  void instanceMenu()
  {
    for (const auto &pair : instances)
    {
      Instance *instance = pair.second;
      ImGui::Begin(("Instance " + pair.first).c_str());

      ImGui::SeparatorText("Translate");

      ImGui::DragFloat("Translate X", &instance->translate.x, 0.1f);
      ImGui::DragFloat("Translate Y", &instance->translate.y, 0.1f);
      ImGui::DragFloat("Translate Z", &instance->translate.z, 0.1f);

      ImGui::SeparatorText("Rotation");

      ImGui::DragFloat("Rotation X", &instance->rotation.x, 0.1f);
      ImGui::DragFloat("Rotation Y", &instance->rotation.y, 0.1f);
      ImGui::DragFloat("Rotation Z", &instance->rotation.z, 0.1f);

      ImGui::SeparatorText("Scale");

      ImGui::DragFloat("Scale X", &instance->scale.x, 0.1f);
      ImGui::DragFloat("Scale Y", &instance->scale.y, 0.1f);
      ImGui::DragFloat("Scale Z", &instance->scale.z, 0.1f);

      ImGui::SeparatorText("Color");

      ImGui::DragFloat("Color X", &instance->color.x, 0.1f, 0.0f, 1.0f);
      ImGui::DragFloat("Color Y", &instance->color.y, 0.1f, 0.0f, 1.0f);
      ImGui::DragFloat("Color Z", &instance->color.z, 0.1f, 0.0f, 1.0f);

      ImGui::End();
    }
  }

  void cameraMenu()
  {
    ImGui::Begin("Camera");

    ImGui::SeparatorText("General");
    ImGui::Combo("Camera Type", &(int &)camera->type, cameraTypes, IM_ARRAYSIZE(cameraTypes), IM_ARRAYSIZE(cameraTypes));

    ImGui::SeparatorText("Position");

    ImGui::DragFloat("Position X", &camera->position.x, 0.1f);
    ImGui::DragFloat("Position Y", &camera->position.y, 0.1f);
    ImGui::DragFloat("Position Z", &camera->position.z, 0.1f);

    ImGui::SeparatorText("Rotation");

    ImGui::DragFloat("Rotation X", &camera->rotation.x, 0.1f);
    ImGui::DragFloat("Rotation Y", &camera->rotation.y, 0.1f);
    ImGui::DragFloat("Rotation Z", &camera->rotation.z, 0.1f);

    ImGui::SeparatorText("Offset");

    ImGui::DragFloat("Offset X", &camera->offsetX, 0.1f);
    ImGui::DragFloat("Offset Y", &camera->offsetY, 0.1f);

    ImGui::SeparatorText("Perspective");

    ImGui::DragFloat("FOV", &camera->fov, 0.1f);
    ImGui::DragFloat("Near", &camera->nearPlane, 0.1f);
    ImGui::DragFloat("Far", &camera->farPlane, 0.1f);

    ImGui::End();
  }

  void draw()
  {
    ImGui::Begin("Debug Menu");

    this->shaderMenu();

    this->cameraMenu();

    this->instanceMenu();

    ImGui::End();
  }
};