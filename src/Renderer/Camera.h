#pragma once

#include <string>

#include <glm/glm.hpp>

enum class CameraType {
  Orthographic,
  Perspective
};

class Camera
{
private:
  CameraType type;

  float width = 1.0f;
  float height = 1.0f;
  
  float offsetX = 1.0f;
  float offsetY = 1.0f;

  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 position = glm::vec3(0.0f);

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(0.0f);

  float fov = 45.0f;
  float farPlane = 100.0f;
  float nearPlane = 0.1f;

private:
  void update();

public:
  void setType(CameraType type);

  void setSize(float width, float height);

  void setOffset(float offsetX, float offsetY);

  void setPosition(float x, float y, float z);

  void setRotation(float pitch, float yaw, float roll);

  void translate(float deltaX, float deltaY, float deltaZ);

  void rotate(float deltaPitch, float deltaYaw, float deltaRoll);

  void setProjectionParams(float fov, float nearPlane, float farPlane);

  const glm::mat4 &getViewProjectionMatrix() const;
};