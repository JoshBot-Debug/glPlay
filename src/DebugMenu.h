#pragma once

#include <string>
#include <unordered_map>

#include "Window/Time.h"
#include "Window/Input.h"
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

  glm::vec2 mouse;

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

      ImGui::DragFloat("Color R", &instance->color.r, 0.1f, 0.0f, 1.0f);
      ImGui::DragFloat("Color G", &instance->color.g, 0.1f, 0.0f, 1.0f);
      ImGui::DragFloat("Color B", &instance->color.b, 0.1f, 0.0f, 1.0f);
      ImGui::DragFloat("Color A", &instance->color.a, 0.1f, 0.0f, 1.0f);

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

  void update()
  {
    ImGuiIO &io = ImGui::GetIO();

    if (io.WantCaptureKeyboard || io.WantCaptureMouse)
      return;

    float speed = 10.0f;
    double delta = Time::GetDeltaTime();

    glm::vec3 translate(0.0f);

    if (Input::KeyPress(KeyboardKey::UP))
      translate.y += speed * delta;

    if (Input::KeyPress(KeyboardKey::DOWN))
      translate.y -= speed * delta;

    if (Input::KeyPress(KeyboardKey::RIGHT))
      translate.x += speed * delta;

    if (Input::KeyPress(KeyboardKey::LEFT))
      translate.x -= speed * delta;

    if (Input::KeyPress(MouseButton::LEFT))
    {
      glm::vec2 dm = glm::mix(glm::vec2(0.0f), Input::MousePosition() - mouse, 0.1f);
      camera->rotate(-dm.y, dm.x, 0.0f);
    }

    camera->translate(translate.x, translate.y, translate.z);

    mouse = Input::MousePosition();
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