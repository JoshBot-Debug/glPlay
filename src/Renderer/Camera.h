#pragma once

#include <string>

#include <glm/glm.hpp>

enum class CameraType
{
  Orthographic,
  Perspective,
};

class Camera
{
private:
  CameraType type;

  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 position = glm::vec3(0.0f);
  glm::mat4 projection;
  glm::mat4 viewProjection;

private:
  void update();

public:

  int width = -1;
  int height = -1;

  Camera();
  ~Camera() = default;

  void setType(const CameraType &type);

  /**
   * Sets the dimensions (width and height) of the camera's view.
   * This defines the size of the orthographic view that the camera will use.
   *
   * @param width The width of the camera's view.
   * @param height The height of the camera's view.
   */
  void setDimensions(int width, int height);

  /**
   * Sets the position of the camera in the world space.
   * The camera's origin is adjusted to the center of the screen,
   * so the (0, 0) position will be at the center of the screen.
   *
   * @param x The x-coordinate of the camera's position (in world space).
   * @param y The y-coordinate of the camera's position (in world space).
   * @param z The z-coordinate of the camera's position (in world space). This isn't used
   */
  void setPosition(float x, float y, float z);

  /**
   * Sets the rotation of the camera in degrees.
   * A positive rotation will rotate the camera counterclockwise.
   *
   * @param rotation The rotation angle in degrees.
   */
  void setRotation(float x, float y, float z);

  /**
   * Moves the camera by a specified amount along the X and Y axes.
   * This allows the camera to be panned incrementally, useful for user-controlled movement.
   *
   * @param deltaX The amount to move the camera along the X-axis (positive for right, negative for left).
   * @param deltaY The amount to move the camera along the Y-axis (positive for up, negative for down).
   * @param deltaZ The amount to move the camera along the Z-axis (positive for depth, negative for back). This isn't used
   */
  void move(float deltaX, float deltaY, float deltaZ);

  /**
   * Returns the view-projection matrix of the camera.
   * This matrix is used for rendering and transforms world coordinates to camera space.
   *
   * @return The current view-projection matrix of the camera.
   */
  const glm::mat4 &getUniformMatrix4fv() const;

  /**
   * Returns the current position of the camera in world space.
   * The position is the camera's center, with (0, 0) being the center of the screen.
   *
   * @return A `glm::vec2` representing the camera's current position (x, y).
   */
  const glm::vec3 &getPosition() const;

  /**
   * Returns the current rotation of the camera in degrees.
   *
   * @return The rotation angle of the camera in degrees.
   */
  const glm::vec3 &getRotation() const;
};