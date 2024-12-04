#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstring>

#include "Common.h"

enum class BufferTarget
{
  ARRAY_BUFFER = GL_ARRAY_BUFFER,
  ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
};

class ArrayBuffer
{
private:
  BufferTarget target;
  unsigned int buffer = 0;

public:
  ArrayBuffer(BufferTarget target) : target(target) {};

  /**
   * Destructor for the ArrayBuffer.
   *
   * This function deletes the OpenGL buffer object (buffer) when the ArrayBuffer is destroyed.
   * It ensures that the buffer is properly cleaned up to free OpenGL resources.
   *
   * If the buffer is valid (not zero), it calls `glDeleteBuffers` to delete the buffer from the GPU memory.
   */
  ~ArrayBuffer();

  /**
   * Disable copy constructor
   */
  ArrayBuffer(const ArrayBuffer &) = delete;

  /**
   * Disable assignment operator
   */
  ArrayBuffer &operator=(const ArrayBuffer &) = delete;

  /**
   * Define a move constructor
   */
  ArrayBuffer(ArrayBuffer &&other) noexcept {};

  /**
   * Generates a buffer if it doesn't already exist.
   *
   * This function checks if the vertex buffer object (buffer) is already generated,
   * and if not, it calls OpenGL to generate one.
   */
  void generate();

  /**
   * Sets the buffer data with a vector of generic data type.
   *
   * @param data The data to store in the buffer. This is a vector of any type T.
   * @param draw Specifies how the buffer will be used (static, dynamic, etc.). Default is STATIC.
   *
   * @tparam T The type of the elements in the vector (e.g., float, glm::vec3).
   */
  template <typename T>
  void set(const std::vector<T> &data, VertexDraw draw = VertexDraw::STATIC)
  {
    glBindBuffer((unsigned int)target, buffer);
    glBufferData((unsigned int)target, data.size() * sizeof(T), data.data(), (unsigned int)draw);
  }

  /**
   * Sets the buffer data using raw data (void pointer).
   *
   * @param size The size of the data in bytes.
   * @param data A pointer to the raw data to be uploaded to the buffer.
   * @param draw Specifies how the buffer will be used (static, dynamic, etc.). Default is STATIC.
   */
  void set(size_t size, const void *data, VertexDraw draw = VertexDraw::STATIC);

  /**
   * Updates part of the buffer with a vector of generic data type.
   *
   * @param offset The offset in the buffer to start updating (in terms of number of elements).
   * @param data The new data to upload to the buffer. This is a vector of any type T.
   *
   * @tparam T The type of the elements in the vector (e.g., float, glm::vec3).
   */
  template <typename T>
  void update(unsigned int offset, const std::vector<T> &data)
  {
    glBindBuffer((unsigned int)target, buffer);
    glBufferSubData((unsigned int)target, offset * sizeof(T), data.size() * sizeof(T), data.data());
  }

  /**
   * Updates part of the buffer using raw data.
   *
   * @param offset The offset in the buffer to start updating (in bytes).
   * @param size The size of the data to update (in bytes).
   * @param data A pointer to the raw data to upload to the buffer.
   */
  void update(size_t offset, size_t size, const void *data);

  /**
   * Resizes the buffer and copies the existing data to the new buffer.
   *
   * @param size The new size for the buffer (in bytes).
   * @param draw Specifies how the buffer will be used (static, dynamic, etc.). Default is STATIC.
   */
  void resize(size_t size, VertexDraw draw = VertexDraw::STATIC);

  /**
   * Binds the buffer.
   */
  void bind() const;

  /**
   * Unbinds the buffer.
   */
  void unbind() const;

  /**
   * Gets the OpenGL ID of the buffer.
   *
   * @return The OpenGL ID of the buffer.
   */
  unsigned int get() const;

  /**
   * @returns the size of the buffer object in bytes.
   */
  int getBufferSize() const;

  /**
   * @returns the data in the buffer.
   */
  template <typename T>
  std::vector<T> getBufferData() const
  {
    const int size = getBufferSize();
    const int items = size / sizeof(T);

    std::vector<T> data(items);

    glGetBufferSubData((unsigned int)target, 0, size, data.data());

    return data;
  };
};