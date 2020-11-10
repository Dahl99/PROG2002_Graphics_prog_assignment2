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
		IndexBuffer(const GLuint* data, GLuint count);		//	Generating, binding and buffering ibo
		IndexBuffer(const std::vector<GLuint> &data);		//	Generating, binding and buffering ibo
		~IndexBuffer();										//	Deletes ibo

		void Bind() const;									//	Binds the index buffer
		void Unbind() const;								//	Unbinds the index buffer

		inline GLuint GetCount() const { return m_Count; }	//	Getter for number of indices
	};
}
