#include "renderer.hpp"
#include <iostream>

namespace framework
{
	Renderer::Renderer()
	{
		// Print OpenGL data
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
	}

	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //	Clearing screen
	}

	void Renderer::EnableBlending() const
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	void Renderer::EnableDepthTesting() const
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::EnableSRGBFramebuffer() const
	{
		glEnable(GL_FRAMEBUFFER_SRGB);
	}

	void Renderer::SetClearColor(glm::vec4& clearColor) const
	{
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	}

	void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
	{
		//	Binding/activating objects
		shader.Bind();
		vao.Bind();
		ibo.Bind();

		//	Drawing on screen
		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
