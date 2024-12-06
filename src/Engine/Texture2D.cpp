#include "Texture2D.h"

#include "Debug.h"
#include <stb/stb_image.h>

Texture2D::Texture2D(unsigned int id, const char *filepath): id(id)
{
  glGenTextures(1, &texture);
  bind();
  setWrap();
  setFilter();
  setMipmapLevel(0, 5);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    LOG_BREAK_BEFORE;
    LOG("Texture: Failed to read texture", filepath);
    LOG_BREAK_AFTER;
  }

  stbi_image_free(data);
  unbind();
}

Texture2D::~Texture2D()
{
  unbind();
  glDeleteTextures(1, &texture);
  texture = 0;
}

void Texture2D::setWrap(TextureWrap s, TextureWrap t, TextureWrap r) const
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (unsigned int)s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (unsigned int)t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (unsigned int)r);
}

void Texture2D::setFilter(TextureFilter min, TextureFilter mag) const
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (unsigned int)min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (unsigned int)mag);
}

void Texture2D::setMipmapLevel(int base, int max) const
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, base);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, max);
}

void Texture2D::setBorderColor(float color[4]) const
{
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void Texture2D::bind(int activate) const
{
  if (texture == 0)
    return;
  if (activate > -1)
    glActiveTexture(GL_TEXTURE0 + activate);
  glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture2D::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
