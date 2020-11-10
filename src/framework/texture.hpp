#pragma once

#include <string>
#include <GL/glew.h>

namespace framework
{
	class Texture
	{
	private:
		GLuint m_RendererID;
		std::string filePath;
		unsigned char* image;
		int w, h, bitsPerPixel;

	public:
		Texture(const std::string& filepath, GLboolean flip = true);
		~Texture();

		void Bind(GLuint slot) const;
		void Unbind() const;

		inline int GetWidth() const { return w; }
		inline int GetHeight() const { return h; }
	};
}