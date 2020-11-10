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

		void Bind(GLuint slot) const;				// Binds texture to specified texture slot
		void Unbind() const;						// Unbinds the texture

		inline int GetWidth() const { return w; }	// Getter for width
		inline int GetHeight() const { return h; }	// Getter for height
	};
}