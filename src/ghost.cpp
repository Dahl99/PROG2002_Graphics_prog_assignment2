#include "ghost.hpp"

Ghost::Ghost(glm::vec3 pos, std::vector<framework::Vertex>& vertices, std::vector<GLuint>& indices)
	: Entity(pos, vertices, indices)
{
	animTimer = 0.0f;
	dir = 0;
}

//void Ghost::UpdateSprite(framework::Shader& shader, const framework::Direction& dir)
//{
//	shader.Bind();
//	GLfloat texPos[2];
//	switch (dir)
//	{
//	case framework::Direction::UP:
//		if (animTimer <= 1.0f)
//		{
//			texPos[0] = 0.0f;
//			texPos[1] = 1.0f / 2.0f;
//		}
//		else if (animTimer <= 2.0f)
//		{
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 1.0f / 2.0f;
//		}
//		else
//		{
//			animTimer = 0.0f;
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 1.0f / 2.0f;
//		}
//		break;
//	case framework::Direction::RIGHT:
//		if (animTimer <= 1.0f)
//		{
//			texPos[0] = 0.0f;
//			texPos[1] = 0.0f;
//		}
//		else if (animTimer <= 2.0f)
//		{
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 0.0f;
//		}
//		else
//		{
//			animTimer = 0.0f;
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 0.0f;
//		}
//		break;
//	case framework::Direction::DOWN:
//		if (animTimer <= 1.0f)
//		{
//			texPos[0] = 0.0f;
//			texPos[1] = 3.0f / 4.0f;
//		}
//		else if (animTimer <= 2.0f)
//		{
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 3.0f / 4.0f;
//		}
//		else
//		{
//			animTimer = 0.0f;
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 3.0f / 4.0f;
//		}
//		break;
//	case framework::Direction::LEFT:
//		if (animTimer <= 1.0f)
//		{
//			texPos[0] = 0.0f;
//			texPos[1] = 1.0f / 4.0f;
//		}
//		else if (animTimer <= 2.0f)
//		{
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 1.0f / 4.0f;
//		}
//		else
//		{
//			animTimer = 0.0f;
//			texPos[0] = 1.0f / 6.0f;
//			texPos[1] = 1.0f / 4.0f;
//		}
//		break;
//	default:
//		break;
//	}
//
//	animTimer += 0.05f;
//	shader.SetUniform2fv("posTex", texPos);
//}

// Ghost movement function
//void Ghost::Behaviour(std::vector<int> & map, int sizeX, GLfloat & dt, framework::Shader & shader)
//{
//	switch (dir) {
//	case 0:
//		if (map[(int)(((int)(GetPos().y + 1) * sizeX) + (int)(GetPos().x + 0.5))] != 1)
//		{
//			UpdatePos(dt, framework::Direction(dir));
//			UpdateSprite(shader, framework::Direction::UP);
//		}
//		else // Change dir
//		{
//			if (rand() % 2)
//				dir += 1;
//			else dir = 3;
//		}
//		break;
//	case 1:
//		if (map[(int)(((int)(GetPos().y + 0.5) * sizeX) + (int)(GetPos().x + 1))] != 1)
//		{
//			UpdatePos(dt, framework::Direction(dir));
//			UpdateSprite(shader, framework::Direction::RIGHT);
//		}
//		else // Change dir
//		{
//			if (rand() % 2)
//				dir += 1;
//			else dir -= 1;
//		}
//		break;
//	case 2:
//		if (map[(int)(((int)(GetPos().y) * sizeX) + (int)(GetPos().x + 0.5))] != 1)
//		{
//			UpdatePos(dt, framework::Direction(dir));
//			UpdateSprite(shader, framework::Direction::DOWN);
//		}
//		else //change dir
//		{
//			if (rand() % 2)
//				dir += 1;
//			else dir -= 1;
//		}
//		break;
//	case 3:
//		if (map[(int)(((int)(GetPos().y + 0.5) * sizeX) + (int)(GetPos().x))] != 1)
//		{
//			UpdatePos(dt, framework::Direction(dir));
//			UpdateSprite(shader, framework::Direction::LEFT);
//		}
//		else //change dir
//		{
//			if (rand() % 2)
//				dir = 0;
//			else dir -= 1;
//		}
//		break;
//	default:
//		break;
//	}
//
//}

void Ghost::GhostMovement(const glm::vec3& pacPos, const std::vector<int>& mapArray, const int& mapSizeX, const GLfloat& dt) {
	// Find distance and direction between ghost and pacman 
	glm::vec3 target = pacPos - position;

	// Set both values to positive in temporary value holder
	glm::vec3 temp;
	target.x > 0 ? temp.x = target.x : temp.x = -target.x;
	target.z > 0 ? temp.z = target.z : temp.z = -target.z;
	temp.y = 0;

	// If x distance is closer to pacman than z distance and not just paralel
	if (temp.x < temp.z && temp.x != 0)
	{
		// If x is negative
		if (target.x <= 0)
		{
			// Move right
			if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
				position.x += dt * mvSpeed;
			// If cant move to the left, move up or down depending on which is closest
			else if (target.z <= 0) // Needs change
			{
				if (mapArray[((position.z + 1) * mapSizeX) + position.x] != 1)
					position.z += dt * mvSpeed;
			}
			else
			{
				if (mapArray[((position.z) * mapSizeX) + position.x] != 1)
					position.z -= dt * mvSpeed;
			}
		}
		else // If x is positive
		{
			// Move left
			if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
				position.x -= dt * mvSpeed;
			// If cant move to the left, move up or down depending on which is closest
			else if (target.z <= 0)
			{
				if (mapArray[((position.z + 1) * mapSizeX) + position.x] != 1)
					position.z += dt * mvSpeed;
			}
			else if (mapArray[((position.z + 1) * mapSizeX) + position.x] != 1)
				position.z -= dt * mvSpeed;
		}
	}
	else // If z is closer
	{
		// If z is less than zero
		if (target.z <= 0)
		{
			// Move down if possible
			if (mapArray[((int)(position.z) * mapSizeX) + position.x] != 1)
				position.z -= dt * mvSpeed;

			// Otherwise right or left depending on which is closer
			else if (target.x <= 0)
			{
				if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
					position.x -= dt * mvSpeed;
			}
			else if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
				position.x += dt * mvSpeed;

		}
		else // If z is greater than 0
		{
			// Move up if possible
			if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
				position.z += dt * mvSpeed;

			// Otherwise right or left depending on which is closer
			else if (target.x <= 0)
			{
				if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
					position.x -= dt * mvSpeed;
			}
			else if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
				position.x += dt * mvSpeed;
		}
	}

}

bool Ghost::CollisionCheck(const glm::vec3& pacmanPos)
{
	const glm::vec3& pos = GetPosition();

	// If pacman and the ghost are on the same tile (adds 0.5 to calculate from the midpoint of the sprites
	if ((int)(pacmanPos.x + 0.5) == (int)(pos.x + 0.5) && (int)(pacmanPos.y + 0.5) == (int)(pos.y + 0.5))
		return true;
	else
		return false;
}
