#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout; // forward declaration to avoid cyclical dependency

class VertexArray {
  private:
    unsigned int m_RendererID;
    
  public:
    VertexArray();
    ~VertexArray();
    
    void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
    void bind() const;
    void unBind() const;
};

