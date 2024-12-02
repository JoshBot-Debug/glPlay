#pragma once

#include "InputKey.h"

#include <glm/glm.hpp>
#include <iostream>

class Input
{
private:
  static GLFWwindow *window;
  
public:
  static void SetWindowContext(GLFWwindow *window);

  static const glm::vec2 MousePosition();

  /**
   * @param key Expects a KeyboardKey or MouseButton
   */
  template <typename T>
  static bool KeyPress(const T &key)
  {
    if (!window)
      return false;

    int state = 0;

    if constexpr (std::is_same_v<T, KeyboardKey>)
      state = glfwGetKey(window, static_cast<int>(key));

    else if constexpr (std::is_same_v<T, MouseButton>)
      state = glfwGetMouseButton(window, static_cast<int>(key));

    return state == static_cast<int>(KeyAction::PRESS);
  }

  /**
   * @param key Expects a KeyboardKey or MouseButton
   */
  template <typename T>
  static bool KeyRelease(const T &key)
  {
    if (!window)
      return false;

    int state = 0;

    if constexpr (std::is_same_v<T, KeyboardKey>)
      state = glfwGetKey(window, static_cast<int>(key));

    else if constexpr (std::is_same_v<T, MouseButton>)
      state = glfwGetMouseButton(window, static_cast<int>(key));

    return state == static_cast<int>(KeyAction::RELEASE);
  }
};