#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace framework
{
	class Shader
	{
	private:
		std::string vertPath, fragPath;
		GLuint m_RendererID;
		std::unordered_map<std::string, GLint> m_UniformCache;	// Caches the uniform locations

	public:
		Shader(const std::string& vertPath, const std::string& fragPath);	//	Creates shader
		~Shader();								//	Deletes shader

		void Bind() const;						//	Binds/activates shader
		void Unbind() const;					//	Unbinds/disables shader

		//				Functions for setting uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform2fv(const std::string& name, const float values[2]);
		void SetUniform3fv(const std::string& name, const glm::vec3& values);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		GLint getUniformLocation(const std::string& name);

	private:

		//				Functions for reading, creating and compiling a shader program
		static std::string ParseShader(const std::string& filepath);
		static GLuint CompileShader(unsigned int type, const std::string& source);
		static GLuint CreateShader(const std::string& vertShader, const std::string& fragShader);
	};

}