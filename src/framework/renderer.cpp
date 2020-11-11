#include "renderer.hpp"

namespace framework
{
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
