#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstring>
#include <iostream>

#include "Common.h"

enum class BufferTarget
{
  ARRAY_BUFFER = GL_ARRAY_BUFFER,
  ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
};

struct BufferPartition
{
  size_t size;  // size of the partition in bytes
  size_t used;  // bytes beign used
  size_t chunk; // size of one chunk in bytes

  BufferPartition(size_t size, size_t used, size_t chunk) : size(size), used(used), chunk(chunk) {}
};

inline size_t getBufferPartitionOffsetSize(const std::vector<BufferPartition> &partitions, unsigned int partitionIndex)
{
  size_t size = 0;

  for (size_t i = 0; i < partitionIndex; i++)
    size += partitions[i].size;

  return size;
}

class ArrayBuffer
{
private:
  BufferTarget target;
  VertexDraw draw;

  unsigned int resizeFactor = 0;

  unsigned int buffer = 0;
  std::vector<BufferPartition> partitions;

public:
  /**
   * @param target Specify weather this is a vertex array buffer or an element array buffer
   *
   * @param draw VertexDraw, Static for non changing, dynamic for frequently changing, or stream
   */
  ArrayBuffer(BufferTarget target, VertexDraw draw = VertexDraw::STATIC);

  /**
   * @param target Specify weather this is a vertex array buffer or an element array buffer
   *
   * @param resizeFactor This multiplier is applied to the incoming data size when resizing a partition (e.g., during an upsert operation)
   * to accommodate more data. For example, if the data size is 12 bytes (3 floats) and the buffer needs resizing, the buffer will be resized
   * to accommodate 12 * multiplier bytes.
   *
   * @param draw VertexDraw, Static for non changing, dynamic for frequently changing, or stream
   */
  ArrayBuffer(BufferTarget target, unsigned int resizeFactor, VertexDraw draw = VertexDraw::STATIC);

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
  void set(const std::vector<T> &data, const std::vector<BufferPartition> partitions = {})
  {
    size_t size = data.size() * sizeof(T);

    if (partitions.size())
      this->partitions = partitions;
    else
      this->partitions.emplace_back(size, size, sizeof(T));

    glBindBuffer((unsigned int)target, buffer);
    glBufferData((unsigned int)target, size, data.data(), (unsigned int)draw);
  }

  /**
   * Sets the buffer data using raw data (void pointer).
   *
   * @param chunk The size of one chunk in bytes.
   * @param count The number of chunks you are inserting
   * @param data A pointer to the raw data to be uploaded to the buffer.
   * @param draw Specifies how the buffer will be used (static, dynamic, etc.). Default is STATIC.
   */
  void set(size_t chunk, unsigned int count, const void *data, const std::vector<BufferPartition> partitions = {});

  /**
   * Updates part of the buffer with a vector of generic data type.
   *
   * @param offset The offset in the buffer to start updating (in terms of number of elements).
   * @param data The new data to upload to the buffer. This is a vector of any type T.
   *
   * @tparam T The type of the elements in the vector (e.g., float, glm::vec3).
   */
  template <typename T>
  void update(unsigned int offset, const std::vector<T> &data, unsigned int partition = 0)
  {
    glBindBuffer((unsigned int)target, buffer);
    glBufferSubData((unsigned int)target, offset * sizeof(T) + getBufferPartitionOffsetSize(partitions, partition), data.size() * sizeof(T), data.data());
  }

  /**
   * Updates part of the buffer using raw data.
   *
   * @param chunk The size of one chunk in bytes.
   * @param offset The offset of chunks in the buffer.
   * @param size The size of the data to update in bytes.
   * @param data A pointer to the raw data to upload to the buffer.
   */
  void update(size_t chunk, unsigned int offset, size_t size, const void *data, unsigned int partitionID = 0);

  /**
   * Insert or updates part of the buffer with a vector of generic data type.
   * If there is not enough place in the partition, the buffer will resize.
   *
   * @param offset The offset in the buffer to start updating (in terms of number of elements).
   * @param data The new data to upload to the buffer. This is a vector of any type T.
   * @param partition The partition you want to upsert the data into.
   *
   * @tparam T The type of the elements in the vector (e.g., float, glm::vec3).
   */
  template <typename T>
  void upsert(unsigned int offset, const std::vector<T> &data, unsigned int partition = 0)
  {
    int dataSize = data.size() * sizeof(T);
    int offsetSize = offset * sizeof(T);
    int expansionSize = (offsetSize + dataSize) - partitions[partition].size;

    if (expansionSize > 0)
      resize(partition, expansionSize + (dataSize * resizeFactor), offsetSize);

    glBindBuffer((unsigned int)target, buffer);
    glBufferSubData((unsigned int)target, offsetSize + getBufferPartitionOffsetSize(partitions, partition), dataSize, data.data());
  }

  /**
   * Gets the OpenGL ID of the buffer.
   *
   * @return The OpenGL ID of the buffer.
   */
  unsigned int get() const;

  /**
   * Binds the buffer.
   */
  void bind() const;

  /**
   * Unbinds the buffer.
   */
  void unbind() const;

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

  /**
   * @param partition The partition data that you want to retrieve
   * @returns the specific partition data in the buffer.
   */
  template <typename T>
  std::vector<T> getBufferData(unsigned int partition) const
  {
    const int offset = getBufferPartitionOffsetSize(partitions, partition);
    const int size = partitions[partition].size;
    const int items = size / sizeof(T);

    std::vector<T> data(items);

    glGetBufferSubData((unsigned int)target, offset, size, data.data());

    return data;
  };

  /**
   * Resizes the buffer and copies the existing data to the new buffer.
   *
   * @param size The new size for the buffer (in bytes).
   */
  void resize(size_t size);

  /**
   * Expands a partition by resizing the buffer
   *
   * @param partition The partition that needs to be expanded
   * @param size The size (in bytes) by which you want to expand this partition
   * @param offset The offset (in bytes) by from where in the partition you want to resize
   */
  void resize(unsigned int partition, size_t size, size_t offset);

  /**
   * Creates a new partition. This function will resize the buffer
   *
   * @param chunk The size of one chunk in bytes.
   * @param count The number of chunks you are reserving space for
   */
  unsigned int addPartition(size_t chunk, unsigned int count);
};