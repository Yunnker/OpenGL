#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GlCall(glGenBuffers(1, &m_RendererID));                             //Allocates memory for one buffer in GPU and returns its ID to m_RendererID
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));                //Binds the buffer - Sets it like a global variable
    GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));  //Creates the buffer and initializes it with the data
}

VertexBuffer::~VertexBuffer()
{
    GlCall(glDeleteBuffers(1, &m_RendererID));                          
}

void VertexBuffer::Bind() const
{
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
