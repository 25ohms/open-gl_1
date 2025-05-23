#include "Renderer.h"

#include <iostream>

void GLClearError()
{
  while(glGetError() != GL_NO_ERROR);  
}


bool GLLogCall(const char* function, const char* file, int line)
{
  while(GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error]" << "(";
    std::cout << std::hex << error;
    std::cout << ")" << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

void Renderer::draw(const VertexArray& va, const IndexBuffer &ib, const Shader &shader) const {
  shader.bind();
  va.bind();
  ib.bind();
  GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const {
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
