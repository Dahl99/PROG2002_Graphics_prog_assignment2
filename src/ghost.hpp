#include <random>
#include <vector>

#include "framework/entity.hpp"

class Ghost : public framework::Entity
{
private:
	GLfloat animTimer;
	int dir;

public:
	Ghost(glm::vec3 pos, std::vector<framework::Vertex>& vertices, std::vector<GLuint>& indices);
	Ghost(glm::vec3 pos, const std::string& modelpath) : Entity(pos, modelpath) {};

	void UpdateSprite(framework::Shader& shader, const framework::Direction& dir);
	void Behaviour(std::vector<int> &map, int sizeX, GLfloat &dt, framework::Shader& shader);
	bool CollisionCheck(const glm::vec3& pacmanPos);
	void GhostMovement(const glm::vec3& pacPos, const std::vector<int>& mapArray, const int& mapSizeX, const GLfloat& dt);
};