#pragma once

enum class LightType
{
  Ambient,     // Ambient light: uniform light that illuminates all objects equally, no direction
  Directional, // Directional light: a light source that has parallel rays, like the sun
  Point,       // Point light: emits light in all directions from a single point (like a light bulb)
  Spot,        // Spot light: emits light in a cone shape, creating a spotlight effect
  Hemisphere,  // Hemisphere light: simulates light coming from the sky or a hemisphere, often used for outdoor lighting
  Area,        // Area light: a light source with a defined area, creating soft shadows and diffused light
  Rectangular, // Rectangular light: a type of area light with a rectangular shape
  Sky,         // Sky light: simulates the light coming from the environment or the skybox
  IES,         // IES (Illuminating Engineering Society) light: light defined by an IES profile, used for realistic lighting
  Volume,      // Volume light: light that interacts with the environment's volume, creating effects like fog and rays
};
class Light
{
private:
  LightType type;

public:
  Light() = default;
  ~Light() = default;

  void setType(const LightType &type);
};