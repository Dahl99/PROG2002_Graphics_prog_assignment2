#pragma once

#include <GL/glew.h>
#include <vector>

#include "globals.hpp"
namespace framework
{
	//	Class containing abstraction of index buffer
	class IndexBuffer
	{
	private:
		GLuint m_RendererID;	//	id for index buffer
		GLuint m_Count;			//	Number of indices

	public:
		IndexBuffer(const GLuint* data, GLuint count);		//	Generating, bindingand buffering vbo
		IndexBuffer(const std::vector<GLuint> &data);		//	Generating, bindingand buffering vbo
		~IndexBuffer();										//	Deletes vbo

		void Bind() const;									//	Binds the ib
		void Unbind() const;								//	Unbinds the ib

		inline GLuint GetCount() const { return m_Count; }	//	Returns number of indices
	};
}
