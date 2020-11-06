#pragma once

#include <GL/glew.h>

#include "vbo.hpp"
#include "vbl.hpp"

namespace framework
{
	class VertexArray
	{
	private:
		GLuint m_RendererID;	//	id for vao

	public:
		VertexArray();		//	Generates vao name
		~VertexArray();		//	Deletes vao

		void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);	//	Adds vbo to using vbl vao

		void Bind() const;		//	Binds the vao
		void Unbind() const;	//	Unbinds the vao
	};

}