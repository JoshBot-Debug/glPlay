#include "ArrayBuffer.h"
#include "Debug.h"

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

void ArrayBuffer::set(size_t size, const void *data, VertexDraw draw)
{
  glBindBuffer((unsigned int)target, buffer);
  glBufferData((unsigned int)target, size, data, (unsigned int)draw);
}

void ArrayBuffer::update(size_t offset, size_t size, const void *data)
{
  glBindBuffer((unsigned int)target, buffer);
  glBufferSubData((unsigned int)target, offset, size, data);
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


void ArrayBuffer::resize(size_t size, VertexDraw draw)
{
  /**
   * Read the size of the current buffer into psize
   * Bind it as GL_COPY_READ_BUFFER
   */
  int psize;
  glBindBuffer(GL_COPY_READ_BUFFER, buffer);
  glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &psize);

  /**
   * Create the new vertex buffer with the new size
   */
  unsigned int nbuffer;
  glGenBuffers(1, &nbuffer);
  glBindBuffer((unsigned int)target, nbuffer);
  glBufferData((unsigned int)target, size, nullptr, (unsigned int)draw);

  /**
   * Bind the new vertex buffer as GL_COPY_WRITE_BUFFER
   */
  glBindBuffer(GL_COPY_WRITE_BUFFER, nbuffer);

  /**
   * Copy the data
   */
  glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, psize);

  /**
   * Delete the old buffer
   */
  glDeleteBuffers(1, &buffer);

  buffer = nbuffer;
}
