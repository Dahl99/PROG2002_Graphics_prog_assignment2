#pragma once

#include "vao.hpp"
#include "ibo.hpp"
#include "shader.hpp"

namespace framework
{
	class Renderer
	{
		public:
			void Clear() const;	//	Clears screen
	
			//	Draws buffers using indices and shader
			void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	};
}
