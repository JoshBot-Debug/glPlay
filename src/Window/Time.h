#pragma once

class Time
{
private:
  static double time;
  static double deltaTime;

public:
  /**
   * DO NOT CALL THIS METHOD
   * This method is for internal implimentation
   */
  static void UpdateDeltaTime();

  /**
   * Get the current time since the window initialized
   */
  static double GetTime();

  /**
   * Get the delta time for this loop
   */
  static double GetDeltaTime();
};