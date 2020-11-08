#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "vao.hpp"
#include "vbo.hpp"
#include "ibo.hpp"
#include "shader.hpp"
#include "model.hpp"

namespace framework {
	enum class Direction	// Enum class used for direction
	{
		FORWARD = 0,
		BACK = 2,
		RIGHT = 1,
		LEFT = 3
	};

	class Entity 
	{
	private:
		glm::vec3 pos;
		GLfloat mvSpeed;
		GLboolean isVulnerable;

		std::unique_ptr<VertexArray> vao;
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<IndexBuffer> ibo;

		std::unique_ptr<Model> model;
	
	public:
		Entity(glm::vec3 pos, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
		Entity(glm::vec3 pos, const std::string& filepath);

		void Draw(Shader& shader) const;						// Draws entity onto screen
		void UpdatePos(const GLfloat& dt, const Direction& dir);	// Updates entity position
		inline glm::vec3 GetPos() const { return pos; };		// Fetches the position of entity
		void ModPos(int loc) { pos.x = loc; };
	};

}