#include "Input.h"

GLFWwindow* Input::window = nullptr;

void Input::SetWindowContext(GLFWwindow *window)
{
  Input::window = window;
}