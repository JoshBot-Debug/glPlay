#pragma once

#include <string>

enum class CameraType
{
  Orthographic, // Orthographic projection: no perspective, objects appear same size
  Perspective,  // Perspective projection: objects appear smaller with distance
  Isometric,    // Isometric projection: a form of orthographic but with a fixed angle
  Fisheye,      // Fisheye lens: creates wide, distorted, circular view
  Panoramic,    // Panoramic: 360-degree view
  Stereoscopic, // Stereoscopic: creates 3D effects for depth perception (2 cameras for left and right eyes)
  TopDown,      // Top-down view: camera looks directly downward on the scene
  FirstPerson,  // First-person view: camera follows the player's perspective (like FPS)
  ThirdPerson,  // Third-person view: camera follows the player from a fixed or dynamic position
  FlyThrough,   // Fly-through camera: allows camera movement in a 3D space like a drone
  DepthOfField  // Depth of field: simulates camera lens focus with blurry background/foreground
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