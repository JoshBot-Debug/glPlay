#include "Time.h"

#include <GLFW/glfw3.h>

double Time::time = 0.0f;
double Time::deltaTime = 0.0f;

void Time::UpdateDeltaTime()
{
  double time = glfwGetTime();
  Time::deltaTime = time - Time::time;
  Time::time = time;
}

double Time::GetTime()
{
  return glfwGetTime();
}

double Time::GetDeltaTime()
{
  return Time::deltaTime;
}