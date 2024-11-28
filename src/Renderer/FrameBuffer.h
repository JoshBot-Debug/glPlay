#pragma once

#include <string>

class FrameBuffer
{
private:
  const std::string name;

public:
  FrameBuffer(const std::string &name);
  ~FrameBuffer() = default;
};