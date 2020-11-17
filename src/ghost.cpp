#include <iostream>
#include "ghost.hpp"

Ghost::Ghost(glm::vec3 pos, std::vector<framework::Vertex>& vertices, std::vector<GLuint>& indices)
	: Entity(pos, vertices, indices)
{
	animTimer = 0.0f;
	dir = 0;
}


// Ghost movement function
void Ghost::Behaviour(std::vector<int> & map, int sizeX, GLfloat & dt)
{
	switch (dir) {
	// Up
	case 0:
		if (map[(int)(((int)(GetPosition().z) * sizeX) + (int)(GetPosition().x))] != 1)
		{
			Move(dt, framework::Direction(dir));
		}
		else // Change dir
		{
			if (rand() % 2)
				dir += 1;
			else dir = 3;
		}
		break;

	// Right
	case 1:
		if (map[(int)(((int)(GetPosition().z - 0.8) * sizeX) + (int)(GetPosition().x -1))] != 1)
		{
			Move(dt, framework::Direction(dir));

		}
		else // Change dir
		{
			if (rand() % 2)
				dir += 1;
			else dir -= 1;
		}
		break;

	// Down
	case 2:
		if (map[(int)(((int)(GetPosition().z - 1.1) * sizeX) + (int)(GetPosition().x))] != 1)
		{
			Move(dt, framework::Direction(dir));

		}
		else //change dir
		{
			if (rand() % 2)
				dir += 1;
			else dir -= 1;
		}
		break;

	// Left
	case 3:
		if (map[(int)(((int)(GetPosition().z - 1) * sizeX) + (int)(GetPosition().x + 0.1))] != 1)
		{
			Move(dt, framework::Direction(dir));

		}
		else //change dir
		{
			if (rand() % 2)
				dir = 0;
			else dir -= 1;
		}
		break;
	default:
		break;
	}

}


// Attempt at smarter ghosts, they are moving somewhat, but i was unable to find the correct numbers and perfect the logic.
// The Script tries to make the ghosts always move towards pacman by moving one step at a time
// The two parts that did not really work was the collision (though that would easily be solved just by changing some numbers) 
//  and getting the function to end up in the correct if each time, unfortunately i had some troubles with or condition not working properly
// Uncomment line in main around line 280 and comment out the line below to see it in action.
void Ghost::GhostMovement(const glm::vec3& pacPos, const std::vector<int>& mapArray, const int& mapSizeX, const GLfloat& dt) {
	// Find distance and direction between ghost and pacman 
	glm::vec3 target = pacPos - position;

	// Set both values to positive in temporary value holder
	glm::vec3 temp;
	target.x > 0 ? temp.x = target.x : temp.x = -target.x;
	target.z > 0 ? temp.z = target.z : temp.z = -target.z;
	temp.y = 0;

	if (((temp.z < 0.1 && temp.z > -0.1)) && (temp.x > 0.1 && temp.x < -0.1))
		std::cout << "ITS TRUE";

	if (temp.x < temp.z || (temp.z < 0.1 && temp.z > -0.1))
		std::cout << "ITS TRUE!!!";
	// If x distance is closer to pacman than z distance and not just paralel
	if (temp.x < temp.z || (temp.z < 0.1 && temp.z > -0.1))
	{
		// If target is to the left of the ghost
		if (target.x <= 0)
		{
			// Move left
			if (mapArray[((int)(position.z - 1) * mapSizeX) + position.x] != 1)
				position.x += dt * mvSpeed;
			// If cant move to the left, move up or down depending on which is closest
			else if (target.z <= 0)
			{
				// Up
				if (mapArray[((int)(position.z) * mapSizeX) + position.x] != 1)
					position.z += dt * mvSpeed;

				// Down
				else if (mapArray[((int)(position.z - 1) * mapSizeX) + position.x] != 1)
					position.z -= dt * mvSpeed;
			}
			else
			{
				// Down
				if (mapArray[((int)(position.z - 1) * mapSizeX) + position.x] != 1)
					position.z -= dt * mvSpeed;

				// Up
				else if (mapArray[((int)(position.z + 1) * mapSizeX) + position.x] != 1)
					position.z += dt * mvSpeed;
			}
		}
		else // If target is to the right of the ghost
		{
			// Move right
			if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
				position.x -= dt * mvSpeed;
			// If cant move to the left, move up or down depending on which is closest
			else if (target.z <= 0)
			{
				// Up
				if (mapArray[((int)(position.z + 1) * mapSizeX) + position.x] != 1)
					position.z += dt * mvSpeed;

				// Down
				else if (mapArray[((int)(position.z + 1) * mapSizeX) + position.x] != 1)
					position.z -= dt * mvSpeed;
			}
			else
			{
				// Down
				if (mapArray[((int)(position.z + 1) * mapSizeX) + position.x] != 1)
					position.z -= dt * mvSpeed;

				// Up
				else if (mapArray[((int)(position.z + 1) * mapSizeX) + position.x] != 1)
					position.z += dt * mvSpeed;
			}
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
				// Left
				if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
					position.x += dt * mvSpeed;
				// Right
				else if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
					position.x -= dt * mvSpeed;
			}
			else
			{
				// Right
				if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
					position.x -= dt * mvSpeed;

				// Left
				else if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
					position.x += dt * mvSpeed;
			}

		}
		else // If z is greater than 0
		{
			// Move up if possible
			if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
				position.z += dt * mvSpeed;

			// Otherwise right or left depending on which is closer
			else if (target.x <= 0)
			{
				// Left
				if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
					position.x += dt * mvSpeed;

				// Right
				else if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
					position.x -= dt * mvSpeed;
			}
			else
			{
				// Right
				if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x - 1] != 1)
					position.x += dt * mvSpeed;

				// Left
				else if (mapArray[(((int)position.z - 1) * mapSizeX) + position.x] != 1)
					position.x -= dt * mvSpeed;
			}
		}
	}
}

bool Ghost::CollisionCheck(const glm::vec3& pacmanPos)
{
	const glm::vec3& pos = GetPosition();

	// If pacman and the ghost are on the same tile (adds 0.5 to calculate from the midpoint of the sprites
	if ((int)(pacmanPos.x + 0.5) == (int)(pos.x + 0.5) && (int)(pacmanPos.z + 0.5) == (int)(pos.z + 0.5))
		return true;
	else
		return false;
}
