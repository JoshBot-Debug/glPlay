#include "ArrayBuffer.h"
#include "Debug.h"

ArrayBuffer::ArrayBuffer(BufferTarget target, VertexDraw draw) : target(target), draw(draw) {}

ArrayBuffer::ArrayBuffer(BufferTarget target, unsigned int resizeFactor, VertexDraw draw) : target(target), draw(draw), resizeFactor(resizeFactor) {};

ArrayBuffer::~ArrayBuffer()
{
  if (buffer)
    glDeleteBuffers(1, &buffer);
}

void ArrayBuffer::generate()
{
  if (!buffer)
    glGenBuffers(1, &buffer);
}

void ArrayBuffer::set(unsigned int chunk, unsigned int count, const void *data, const std::vector<BufferPartition> partitions)
{
  unsigned int size = count * chunk;

  if (partitions.size())
    this->partitions = partitions;
  else
    this->partitions.emplace_back(size, size, chunk);

  glBindBuffer((unsigned int)target, buffer);
  glBufferData((unsigned int)target, size, data, (unsigned int)draw);
}

void ArrayBuffer::update(unsigned int chunk, unsigned int offset, unsigned int size, const void *data, unsigned int partition)
{
  glBindBuffer((unsigned int)target, buffer);
  glBufferSubData((unsigned int)target, (offset * chunk) + getBufferPartitionOffsetSize(partitions, partition), size, data);
}

unsigned int ArrayBuffer::get() const { return buffer; }

void ArrayBuffer::bind() const { glBindBuffer((unsigned int)target, buffer); }

void ArrayBuffer::unbind() const { glBindBuffer((unsigned int)target, 0); }

int ArrayBuffer::getBufferSize() const
{
  glBindBuffer((unsigned int)target, buffer);

  int size = 0;
  glGetBufferParameteriv((unsigned int)target, GL_BUFFER_SIZE, &size);

  return size;
}

void ArrayBuffer::resize(unsigned int size)
{
  int currentSize;
  glBindBuffer(GL_COPY_READ_BUFFER, buffer);
  glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &currentSize);

  unsigned int next;
  glGenBuffers(1, &next);
  glBindBuffer((unsigned int)target, next);
  glBufferData((unsigned int)target, size, nullptr, (unsigned int)draw);

  glBindBuffer(GL_COPY_WRITE_BUFFER, next);
  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, currentSize);
  glDeleteBuffers(1, &buffer);

  buffer = next;
}

void ArrayBuffer::resize(unsigned int partition, unsigned int size, unsigned int offset)
{
  int currentSize;
  glBindBuffer(GL_COPY_READ_BUFFER, buffer);
  glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &currentSize);

  unsigned int next;
  glGenBuffers(1, &next);
  glBindBuffer((unsigned int)target, next);
  glBufferData((unsigned int)target, currentSize + size, nullptr, (unsigned int)draw);

  glBindBuffer(GL_COPY_WRITE_BUFFER, next);

  int partitionOffset = getBufferPartitionOffsetSize(partitions, partition);

  const size_t remaining = currentSize - (partitionOffset + offset);

  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, partitionOffset + offset);

  if (remaining > 0)
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, partitionOffset + offset, partitionOffset + offset + size, remaining);

  glDeleteBuffers(1, &buffer);

  buffer = next;

  partitions[partition].size += size;
}

unsigned int ArrayBuffer::addPartition(unsigned int chunk, unsigned int count)
{
  this->partitions.emplace_back(chunk * count, 0, chunk);

  resize(getBufferSize() + (chunk * count));

  return this->partitions.size() - 1;
}
