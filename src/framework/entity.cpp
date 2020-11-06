#include "entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace framework 
{
	Entity::Entity(glm::vec3 pos, std::vector<Vertex>& vertices, std::vector<GLuint>& indices) 
		: pos(pos), col(glm::vec3(1.0f)), mvSpeed(5.0f), isVulnerable(GL_TRUE)
	{
		vao = std::make_unique<VertexArray>();			// Initializing vao
		vbo = std::make_unique<VertexBuffer>(vertices); // Initializing vbo
		ibo = std::make_unique<IndexBuffer>(indices);	// Initializing ibo

		framework::VertexBufferLayout vbl;          // Create a vertex buffer layout
		vbl.Push<GLfloat>(2);                       // Adding position floats to layout
		vbl.Push<GLfloat>(3);                       // Adding color floats to layout
		vbl.Push<GLfloat>(2);                       // Adding tex coords floats to layout

		vao->AddBuffer(*vbo, vbl);					// Populating the vertex buffer
	}

	void Entity::Draw(Shader& shader) const
	{
		shader.Bind();
		vao->Bind();
		ibo->Bind();

		// Setting the model matrix and passing it to shader as an uniform
		auto model = glm::translate(glm::mat4(1.f), pos);
		shader.SetUniformMat4f("u_Model", model);

		glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Entity::UpdatePos(const GLfloat& dt, const Direction& dir)
	{
		// Updates position based on direction
		switch (dir)
		{
		case Direction::UP:
			pos.y += dt * mvSpeed;
			break;
		case Direction::RIGHT:
			pos.x += dt * mvSpeed;
			break;
		case Direction::DOWN:
			pos.y -= dt * mvSpeed;
			break;
		case Direction::LEFT:
			pos.x -= dt * mvSpeed;
			break;
		default:
			break;
		}
	}
}