#include "vao.hpp"

namespace framework
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);		//	Generate vao name
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);		//	Deleting vao
	}

	void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
	{
		//	Binding vao and vbo
		Bind();
		vbo.Bind();

		const auto& elements = layout.GetElements();	//	Fetching all vbo layouts
		GLuint offset = 0;								//	Initializing offset

		for (GLuint i = 0; i < elements.size(); i++)	//	Looping through elements in vbo layout
		{
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);				//	Enabling vertex attribute array

			//	Defining array of vertex attribute data
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);	//	Increasing offset
		}
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);			//	Binding vao
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);						//	Unbinding vao
	}
}