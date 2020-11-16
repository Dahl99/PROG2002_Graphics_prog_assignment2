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
	protected:
		glm::vec3 position, rotationAxis, scale;
		GLfloat mvSpeed, rotation;
		GLboolean isVulnerable;

		std::unique_ptr<VertexArray> vao;
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<IndexBuffer> ibo;

		std::unique_ptr<Model> model;
	
	public:
		Entity(glm::vec3 pos, std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
		Entity(glm::vec3 pos, const std::string& modelpath);

		// Setter for rotation axis vector
		inline void SetRotationAxis(glm::vec3 rotationAxis) { this->rotationAxis = rotationAxis; }	

		// Setter for rotation in degrees
		inline void SetRotation(GLfloat rotation) { this->rotation = rotation; }					
		inline GLfloat GetRotation() const { return rotation; }

		// Setter for scale vector
		inline void SetScale(glm::vec3 scale) { this->scale = scale; }								

		inline glm::vec3 GetPosition() const { return position; };					// Getter for position vector
		inline void SetPosition(glm::vec3 position) { this->position = position; }	// Setter for position vector

		// Updates entity position based on direction using delta time
		void Move(const GLfloat& dt, const Direction& direction);

		void ModPos(int loc) { position.x = loc; };
		void Draw(Shader& shader, glm::mat4& view, glm::mat4& proj) const;		// Draws entity onto screen
	};
}