#include "Texture.h"

#include "Debug.h"
#include <stb/stb_image.h>

Texture::Texture(const std::string &name, const std::string &path) : name(name), path(path)
{
  glGenTextures(1, &texture);
  bind();
  setWrap();
  setFilter();
  setMipmapLevel(0, 5);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    LOG_BREAK_BEFORE;
    LOG("Texture: Failed to read texture", name);
    LOG_BREAK_AFTER;
  }

  stbi_image_free(data);
  unbind();
}

Texture::~Texture()
{
  unbind();
  glDeleteTextures(1, &texture);
  LOG_BREAK_BEFORE;
  LOG("Texture deleted:", texture);
  LOG_BREAK_AFTER;
  texture = 0;
}

void Texture::setWrap(TextureWrap s, TextureWrap t, TextureWrap r) const
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (unsigned int)s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (unsigned int)t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, (unsigned int)r);
}

void Texture::setFilter(TextureFilter min, TextureFilter mag) const
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (unsigned int)min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (unsigned int)mag);
}

void Texture::setMipmapLevel(int base, int max) const
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, base);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, max);
}

void Texture::setBorderColor(float color[4]) const
{
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void Texture::bind() const
{
  if (texture == 0)
    return;
  glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
