#pragma once

#include <string>

enum class CameraType
{
  Orthographic,
  Perspective,
};

class Camera
{
private:
  CameraType type;

public:
  Camera() = default;
  ~Camera() = default;

  void setType(const CameraType &type);
};