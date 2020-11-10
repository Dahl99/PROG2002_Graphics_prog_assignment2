#include "ibo.hpp"

namespace framework
{
	IndexBuffer::IndexBuffer(const GLuint* data, GLuint count) : m_Count(count)
	{
		glGenBuffers(1, &m_RendererID);							//	Generate buffer object name
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//	Binding buffer object

		//	creating and initializing buffer object's data store
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(const std::vector<GLuint> &data) : m_Count(data.size())
	{
		glGenBuffers(1, &m_RendererID);							//	Generate buffer object name
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//	Binding buffer object

		//	creating and initializing buffer object's data store
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(GLuint), data.data(), GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);						//	Deleting ibo
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//	Binding buffer object
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);				//	Unbinding buffer object
	}
}