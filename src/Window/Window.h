#pragma once

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"

struct WindowOptions
{
  std::string title;

  unsigned int width;
  unsigned int height;

  bool enableDepth = false;
  bool enableVSync = false;

  bool imguiEnableKeyboard = false;
  bool imguiEnableGamepad = false;
  bool imguiEnableDocking = false;

  bool darkMode = false;
};

class Window : Scene
{
private:
  GLFWwindow *window;

  static glm::ivec2 dimensions;

  static void setFrameBufferSize(GLFWwindow *window, int w, int h);

public:
  Window(const WindowOptions &options);
  ~Window();

  void open();

  static const glm::ivec2 &GetDimensions();
};