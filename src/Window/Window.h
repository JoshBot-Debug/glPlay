#pragma once

#include <string>
#include <iostream>

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

public:
  Window(const WindowOptions &options);
  ~Window();

  void open();
};