#include "vbo.hpp"

namespace framework
{
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		glGenBuffers(1, &m_RendererID);								//	Generating the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);				//	Binding the buffer

		//	Creating and initializing buffer's data store
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const std::vector<framework::Vertex>& data)
	{
		glGenBuffers(1, &m_RendererID);								//	Generating the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);				//	Binding the buffer

		//	Creating and initializing buffer's data store
		glBufferData(GL_ARRAY_BUFFER, sizeof(framework::Vertex) * data.size(), data.data(), GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);							//	Deleting vbo
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);				//	Binding vbo
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);							//	Unbinding vbo
	}

	void VertexBuffer::UpdateData(const std::vector<framework::Vertex>& data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(framework::Vertex) * data.size(), data.data());
	}
}