#pragma once

#include "vao.hpp"
#include "ibo.hpp"
#include "shader.hpp"

namespace framework
{
	class Renderer
	{
		public:
			Renderer();

			void Clear() const;					// Clears screen

			void EnableBlending() const;		// Enables blending in OpenGL
			void EnableDepthTesting() const;	// Enables depth testing in OpenGL

			void SetClearColor(glm::vec4& clearColor) const;	// Setter for clear color
	
			//	Draws buffers using indices and shader
			void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	};
}
