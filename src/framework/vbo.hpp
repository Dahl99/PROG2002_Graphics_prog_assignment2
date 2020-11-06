#pragma once

#include <GL/glew.h>
#include <vector>

#include"globals.hpp"

namespace framework
{
	// Class containing abstraction of vbo
	class VertexBuffer
	{
	private:
		GLuint m_RendererID;	//	id for vbo

	public:
		VertexBuffer(const void* data, unsigned int size);			//	Generating, binding and buffering vbo
		VertexBuffer(const std::vector<framework::Vertex>& data);	//	Generating, binding and buffering vbo
		~VertexBuffer();											//	Deletes vbo

		void Bind() const;		//	Binds the vbo
		void Unbind() const;	//	Unbinds the vbo
		void UpdateData(const std::vector<framework::Vertex>& data);
	};
}