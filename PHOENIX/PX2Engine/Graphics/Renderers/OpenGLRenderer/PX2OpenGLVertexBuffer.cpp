// PX2OpenGLVertexBuffer.cpp

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLVertexBuffer.hpp"
#include "PX2OpenGLMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer(Renderer*, const VertexBuffer* vbuffer)
{
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);

	glBufferData(GL_ARRAY_BUFFER, vbuffer->GetNumBytes(), 0,
		gOGLBufferUsage[vbuffer->GetUsage()]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	void* data = Lock(Buffer::BL_WRITE_ONLY);
	memcpy(data, vbuffer->GetData(), vbuffer->GetNumBytes());
	Unlock();
}
//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer()
{
	glDeleteBuffers(1, &mBuffer);
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable(Renderer*, unsigned int, unsigned int,
	unsigned int)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable(Renderer*, unsigned int)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//----------------------------------------------------------------------------
void* PdrVertexBuffer::Lock(Buffer::Locking mode)
{
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);

	GLvoid* videoMemory = glMapBuffer(GL_ARRAY_BUFFER,
		gOGLBufferLocking[mode]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return videoMemory;
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//----------------------------------------------------------------------------

#endif
