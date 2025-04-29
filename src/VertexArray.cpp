#include "VertexArray.h"
#include "Renderer.h"


VertexArray::VertexArray() {
  GLCall(glGenVertexArraysAPPLE(1, &m_RendererID));
}
VertexArray::~VertexArray(){
  GLCall(glDeleteVertexArraysAPPLE(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout){
  bind();
  vb.bind();
  const auto &elements = layout.getElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto &element = elements[i];
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type,
          element.normalized, layout.getStride(), (const void*) offset)); // specifies to C++ the structure of vertices
    offset += element.count * VertexBufferElement::getSizeOfType(element.type);
  }
}

void VertexArray::bind() const {
  GLCall(glBindVertexArrayAPPLE(m_RendererID));
}

void VertexArray::unBind() const {
  GLCall(glBindVertexArrayAPPLE(0));
}
