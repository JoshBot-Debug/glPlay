#pragma once

#include <GL/glew.h>
#include "Engine/Types.h"
#include <iostream>

class Renderer
{
public:
  static void Draw(const DrawElementsIndirectCommand &command, const Primitive &primitive = Primitive::TRIANGLES)
  {
    glDrawElementsIndirect((unsigned int)primitive, GL_UNSIGNED_INT, 0);
  }

  static void Draw(const std::vector<DrawElementsIndirectCommand> &commands, const Primitive &primitive = Primitive::TRIANGLES)
  {
    // glMultiDrawElementsIndirect((unsigned int)primitive, GL_UNSIGNED_INT, (const void *)0, commands.size(), 0);

    // glDrawElements((unsigned int)primitive, 36, GL_UNSIGNED_INT, (const void *)0);
    // glDrawArrays((unsigned int)primitive, 0, 24);

    // glDrawElements((unsigned int)primitive, 72, GL_UNSIGNED_INT, (const void *)0);
    // glDrawArrays((unsigned int)primitive, 0, 48);

    // glDrawElements((unsigned int)primitive, 2916, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int)));
    // glDrawArrays((unsigned int)primitive, 0, 2008);

    // glDrawElements((unsigned int)primitive, 0, GL_UNSIGNED_INT, (const void *)(2880 * sizeof(unsigned int)));
    // glDrawElements((unsigned int)primitive, 36, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int)));
    // glDrawElements((unsigned int)primitive, 2880, GL_UNSIGNED_INT, (const void *)(36 * sizeof(unsigned int)));

    // glDrawElements((unsigned int)primitive, 36, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int))); // cube
    // glDrawElements((unsigned int)primitive, 2880, GL_UNSIGNED_INT, (const void *)(36 * sizeof(unsigned int))); // circle

    // glDrawArrays((unsigned int)primitive, 0, 24); // cube
    // glDrawArrays((unsigned int)primitive, 24, 1984); // circle

    // glDrawElements((unsigned int)primitive, 59937, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int))); // Keqing
    // glDrawArrays((unsigned int)primitive, 0, 59937); // Keqing

    glDrawElements((unsigned int)primitive, 2880, GL_UNSIGNED_INT, (const void *)(0 * sizeof(unsigned int))); 
  }
};