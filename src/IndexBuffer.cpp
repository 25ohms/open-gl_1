#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count) {
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // we need to bind the buffer before specifying anything else
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // we need to bind the buffer before specifying anything else
}

void IndexBuffer::unBind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // we need to bind the buffer before specifying anything else
}
