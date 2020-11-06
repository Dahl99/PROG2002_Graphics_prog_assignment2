#pragma once

#include <GL/glew.h>
#include <vector>

namespace framework
{
	//	Struct for storing vbo layout data
	struct VertexBufferElement
	{
		GLuint type;
		GLuint count;
		GLbyte normalized;

		static GLuint GetSizeOfType(GLuint type)	//	Returns the size of specified types
		{
			switch (type)
			{
				case GL_FLOAT:			return 4;
				case GL_UNSIGNED_INT:	return 4;
				case GL_UNSIGNED_BYTE:	return 1;
			}

			// Missing assertion
			return 0;
		}
	};

	//	Class containing abstraction of vbo layouts
	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_Elements;				//	Used for storing vbo layouts
		GLuint stride;

	public:
		VertexBufferLayout() : stride(0) {}							//	Initializing stride


		inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }	//	Returns the elements
		inline GLuint GetStride() const { return stride; }											//	Returns the stride
		
		template<typename T>
		void Push(GLuint count)
		{
			static_assert(false);
		}

		template<>
		void Push<GLfloat>(GLuint count)
		{
			m_Elements.push_back({ GL_FLOAT, count, false });
			stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
		}
		
		template<>
		void Push<GLuint>(GLuint count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
			stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
		}
		
		template<>
		void Push<GLbyte>(GLuint count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, true });
			stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
		}
	};

}