#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "tilemap.hpp"

namespace framework {

	// Gets filepath as parameter and reads the tilemap from file
	Map::Map(const std::string& levelPath)
	{
		numWalls = numCollecs = 0;

		std::ifstream stream(levelPath);		
		if (stream) 
		{
			// Get map width and heigth and read in the appropriate amount
			stream >> sizeX >> sizeY;
			sizeArray = sizeX * sizeY;
			for (int i = 0; i < sizeArray; i++)
			{
				int x;
				stream >> x;

				// Insert read data into the first location in the array. This is because the map in the level file is read\
					top down but is drawn bottom up, since the space we opperate in goes from 0 x and y and up
				array.insert(array.begin(), x);									
			}
			CreateMap();
		}
		else std::cout << "Failed to read map!\n";

		stream.close();
	}

	Map::~Map()
	{
		if (map)
			delete[] map;
	}




	// Function to print map, used to see if its read correctly
	void Map::PrintMap() const
	{
		for (int i = 0; i < sizeArray; i++)
		{
			if (i % sizeX == 0) 
			{
			std::cout << std::endl;
			}
			std::cout << array[i] << " ";
		}

		std::cout << std::endl;
	}

	// Loops through mapsize and puts the data for each tile into one of two vector containers,
	//  one for walls and one for collectibles
	ShaderVertData Map::retMapVertices()
	{
		// Creates the return data container
		ShaderVertData mapVertices;

		// Goes through the map and puts each vertice of each tile into either the walls or collectibles container
		for (int i = 0; i < sizeArray; i++)
		{
			if(array[i] && array[i] != 2)
			{
				mapVertices.wallVertices.push_back(map[i].top.botLeft);
				mapVertices.wallVertices.push_back(map[i].top.botRight);
				mapVertices.wallVertices.push_back(map[i].top.topLeft);
				mapVertices.wallVertices.push_back(map[i].top.topRight);

				mapVertices.wallVertices.push_back(map[i].bottom.botLeft);
				mapVertices.wallVertices.push_back(map[i].bottom.botRight);
				mapVertices.wallVertices.push_back(map[i].bottom.topLeft);
				mapVertices.wallVertices.push_back(map[i].bottom.topRight);
				numWalls++;
			}
			else
			{
				mapVertices.collectibleVertices.push_back(map[i].top.botLeft);
				mapVertices.collectibleVertices.push_back(map[i].top.botRight);
				mapVertices.collectibleVertices.push_back(map[i].top.topLeft);
				mapVertices.collectibleVertices.push_back(map[i].top.topRight);

				mapVertices.collectibleVertices.push_back(map[i].bottom.botLeft);
				mapVertices.collectibleVertices.push_back(map[i].bottom.botRight);
				mapVertices.collectibleVertices.push_back(map[i].bottom.topLeft);
				mapVertices.collectibleVertices.push_back(map[i].bottom.topRight);
				numCollecs++;
			}
		}
		return mapVertices;
	}


	// Creates the indice arrays for the map
	std::vector<GLuint> Map::retMapIndices(int iterations)
	{
		// Create the return value container
		std::vector<GLuint> indices;

		// Adds the indices to create the two triangles of each square\
			the order is 1, 2, 3 and 3, 4, 1 because vertexes are added in botleft, botright, topleft, topright
		for (GLuint i = 0; i < iterations; i++)
		{
			// Top
			indices.push_back(i * 8);
			indices.push_back((i * 8) + 1);
			indices.push_back((i * 8) + 2);
			indices.push_back((i * 8) + 2);
			indices.push_back((i * 8) + 3);
			indices.push_back((i * 8) + 1);			
			
			// South
			indices.push_back((i * 8) + 4);
			indices.push_back((i * 8) + 5);
			indices.push_back((i * 8));
			indices.push_back((i * 8));
			indices.push_back((i * 8) + 1);
			indices.push_back((i * 8) + 5);			
			
			// East
			indices.push_back((i * 8) + 5);
			indices.push_back((i * 8) + 7);
			indices.push_back((i * 8) + 1);
			indices.push_back((i * 8) + 1);
			indices.push_back((i * 8) + 3);
			indices.push_back((i * 8) + 7);			
			
			// North
			indices.push_back((i * 8) + 6);
			indices.push_back((i * 8) + 7);
			indices.push_back((i * 8) + 2);
			indices.push_back((i * 8) + 2);
			indices.push_back((i * 8) + 3);
			indices.push_back((i * 8) + 7);			
			
			// West
			indices.push_back((i * 8) + 6);
			indices.push_back((i * 8) + 4);
			indices.push_back((i * 8) + 2);
			indices.push_back((i * 8) + 2);
			indices.push_back((i * 8));
			indices.push_back((i * 8) + 4);			
			
			// Bottom
			indices.push_back((i * 8) + 4);
			indices.push_back((i * 8) + 5);
			indices.push_back((i * 8) + 6);
			indices.push_back((i * 8) + 6);
			indices.push_back((i * 8) + 7);
			indices.push_back((i * 8) + 5);
		}		
		
		return indices;
	}

	// Creates a tile and gives that tile 4 vertices for each 
	//  number in the map that is read in the constructor
	void Map::CreateMap()
	{
		map = new framework::Cube[sizeArray];

		// Declaration of some variables
		glm::vec3 playerPos;
		std::vector<glm::vec3> ghostPos;

		int yPos = 0;
		int playerLoc = 0;

		// For the the map read in the constructor, check each number and assign possition and colour 
		//  starting at i = 1 because of mod
		for (int i = 1; i <= sizeArray; i++)
		{

			// The cases are Collectibles, walls, player and ghost respectively
			switch (array[i - 1])
			{
			case 0:	// Each vertice of an collectible gets added or subtracted a const
					//  to the vertice positions in order to reduce the size of the collectible

				// Top bottom left vertex
				map[i - 1].top.botLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].top.botLeft.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].top.botLeft.pos.z = 1;
				map[i - 1].top.botLeft.col.y = 1.0f;
				map[i - 1].top.botLeft.col.z = 1.0f;
				map[i - 1].top.botLeft.col.x = 1.0f;

				// Top bottom right vertex
				map[i - 1].top.botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].top.botRight.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].top.botRight.pos.z = 1;
				map[i - 1].top.botRight.col.x = 1.0f;
				map[i - 1].top.botRight.col.y = 1.0f;
				map[i - 1].top.botRight.col.z = 1.0f;

				// Top top left vertex
				map[i - 1].top.topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].top.topLeft.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].top.topLeft.pos.z = 1;
				map[i - 1].top.topLeft.col.x = 1.0f;
				map[i - 1].top.topLeft.col.y = 1.0f;
				map[i - 1].top.topLeft.col.z = 1.0f;

				// Top top right vertex
				map[i - 1].top.topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].top.topRight.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].top.topRight.pos.z = 1;
				map[i - 1].top.topRight.col.x = 1.0f;
				map[i - 1].top.topRight.col.y = 1.0f;
				map[i - 1].top.topRight.col.z = 1.0f;

				// Bottom bottom left vertex
				map[i - 1].bottom.botLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].bottom.botLeft.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].bottom.botLeft.pos.z = 0;
				map[i - 1].bottom.botLeft.col.y = 1.0f;
				map[i - 1].bottom.botLeft.col.z = 1.0f;
				map[i - 1].bottom.botLeft.col.x = 1.0f;

				// Bottom bottom right vertex
				map[i - 1].bottom.botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.botRight.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].bottom.botRight.pos.z = 0;
				map[i - 1].bottom.botRight.col.x = 1.0f;
				map[i - 1].bottom.botRight.col.y = 1.0f;
				map[i - 1].bottom.botRight.col.z = 1.0f;

				// Bottom top left vertex
				map[i - 1].bottom.topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].bottom.topLeft.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.topLeft.pos.z = 0;
				map[i - 1].bottom.topLeft.col.x = 1.0f;
				map[i - 1].bottom.topLeft.col.y = 1.0f;
				map[i - 1].bottom.topLeft.col.z = 1.0f;

				// Bottom top right vertex
				map[i - 1].bottom.topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.topRight.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.topRight.pos.z = 0;
				map[i - 1].bottom.topRight.col.x = 1.0f;
				map[i - 1].bottom.topRight.col.y = 1.0f;
				map[i - 1].bottom.topRight.col.z = 1.0f;
				break;

			case 1:
				// Top bottom left vertex
				map[i - 1].top.botLeft.pos.x = (i - 1) % sizeX;
				map[i - 1].top.botLeft.pos.y = yPos;
				map[i - 1].top.botLeft.pos.z = 1;
				map[i - 1].top.botLeft.col.y = 0.0f;
				map[i - 1].top.botLeft.col.x = 0.0f;
				map[i - 1].top.botLeft.col.z = 0.6f;

				// Top bottom right vertex
				map[i - 1].top.botRight.pos.x = ((i - 1) % sizeX) + 1;
				map[i - 1].top.botRight.pos.y = yPos;
				map[i - 1].top.botRight.pos.z = 1;
				map[i - 1].top.botRight.col.x = 0.0f;
				map[i - 1].top.botRight.col.y = 0.0f;
				map[i - 1].top.botRight.col.z = 0.6f;

				// Top top left vertex
				map[i - 1].top.topLeft.pos.x = (i - 1) % sizeX;
				map[i - 1].top.topLeft.pos.y = yPos + 1;
				map[i - 1].top.topLeft.pos.z = 1;
				map[i - 1].top.topLeft.col.x = 0.0f;
				map[i - 1].top.topLeft.col.y = 0.0f;
				map[i - 1].top.topLeft.col.z = 0.6f;

				// Top top right vertex
				map[i - 1].top.topRight.pos.x = ((i - 1) % sizeX) + 1;
				map[i - 1].top.topRight.pos.y = yPos + 1;
				map[i - 1].top.topRight.pos.z = 1;
				map[i - 1].top.topRight.col.x = 0.0f;
				map[i - 1].top.topRight.col.y = 0.0f;
				map[i - 1].top.topRight.col.z = 0.6f;

				// Bottom bottom left vertex
				map[i - 1].bottom.botLeft.pos.x = (i - 1) % sizeX;
				map[i - 1].bottom.botLeft.pos.y = yPos;
				map[i - 1].bottom.botLeft.pos.z = 0;
				map[i - 1].bottom.botLeft.col.y = 0.0f;
				map[i - 1].bottom.botLeft.col.x = 0.0f;
				map[i - 1].bottom.botLeft.col.z = 0.6f;

				// Bottom bottom right vertex
				map[i - 1].bottom.botRight.pos.x = ((i - 1) % sizeX) + 1;
				map[i - 1].bottom.botRight.pos.y = yPos;
				map[i - 1].bottom.botRight.pos.z = 0;
				map[i - 1].bottom.botRight.col.x = 0.0f;
				map[i - 1].bottom.botRight.col.y = 0.0f;
				map[i - 1].bottom.botRight.col.z = 0.6f;

				// Bottom top left vertex
				map[i - 1].bottom.topLeft.pos.x = (i - 1) % sizeX;
				map[i - 1].bottom.topLeft.pos.y = yPos + 1;
				map[i - 1].bottom.topLeft.pos.z = 0;
				map[i - 1].bottom.topLeft.col.x = 0.0f;
				map[i - 1].bottom.topLeft.col.y = 0.0f;
				map[i - 1].bottom.topLeft.col.z = 0.6f;

				// Bottom top right vertex
				map[i - 1].bottom.topRight.pos.x = ((i - 1) % sizeX) + 1;
				map[i - 1].bottom.topRight.pos.y = yPos + 1;
				map[i - 1].bottom.topRight.pos.z = 0;
				map[i - 1].bottom.topRight.col.x = 0.0f;
				map[i - 1].bottom.topRight.col.y = 0.0f;
				map[i - 1].bottom.topRight.col.z = 0.6f;
				break;

			case 2: // Sets player pos if tile type is 2, also adds collectible vertices

				playerPos = glm::vec3((float)(i % sizeX), (float)(yPos), 1.0f);

				// Top bottom left vertex
				map[i - 1].top.botLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].top.botLeft.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].top.botLeft.pos.z = 1;
				map[i - 1].top.botLeft.col.y = 1.0f;
				map[i - 1].top.botLeft.col.z = 1.0f;
				map[i - 1].top.botLeft.col.x = 1.0f;

				// Top bottom right vertex
				map[i - 1].top.botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].top.botRight.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].top.botRight.pos.z = 1;
				map[i - 1].top.botRight.col.x = 1.0f;
				map[i - 1].top.botRight.col.y = 1.0f;
				map[i - 1].top.botRight.col.z = 1.0f;

				// Top top left vertex
				map[i - 1].top.topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].top.topLeft.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].top.topLeft.pos.z = 1;
				map[i - 1].top.topLeft.col.x = 1.0f;
				map[i - 1].top.topLeft.col.y = 1.0f;
				map[i - 1].top.topLeft.col.z = 1.0f;

				// Top top right vertex
				map[i - 1].top.topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].top.topRight.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].top.topRight.pos.z = 1;
				map[i - 1].top.topRight.col.x = 1.0f;
				map[i - 1].top.topRight.col.y = 1.0f;
				map[i - 1].top.topRight.col.z = 1.0f;

				// Bottom bottom left vertex
				map[i - 1].bottom.botLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].bottom.botLeft.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].bottom.botLeft.pos.z = 0;
				map[i - 1].bottom.botLeft.col.y = 1.0f;
				map[i - 1].bottom.botLeft.col.z = 1.0f;
				map[i - 1].bottom.botLeft.col.x = 1.0f;

				// Bottom bottom right vertex
				map[i - 1].bottom.botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.botRight.pos.y = (yPos)+COLLECTIBLESIZE;
				map[i - 1].bottom.botRight.pos.z = 0;
				map[i - 1].bottom.botRight.col.x = 1.0f;
				map[i - 1].bottom.botRight.col.y = 1.0f;
				map[i - 1].bottom.botRight.col.z = 1.0f;

				// Bottom top left vertex
				map[i - 1].bottom.topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				map[i - 1].bottom.topLeft.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.topLeft.pos.z = 0;
				map[i - 1].bottom.topLeft.col.x = 1.0f;
				map[i - 1].bottom.topLeft.col.y = 1.0f;
				map[i - 1].bottom.topLeft.col.z = 1.0f;

				// Bottom top right vertex
				map[i - 1].bottom.topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.topRight.pos.y = (yPos + 1) - COLLECTIBLESIZE;
				map[i - 1].bottom.topRight.pos.z = 0;
				map[i - 1].bottom.topRight.col.x = 1.0f;
				map[i - 1].bottom.topRight.col.y = 1.0f;
				map[i - 1].bottom.topRight.col.z = 1.0f;
				break;
			case 3: // Sets ghost position if tile type is 3
				ghostPos.push_back(glm::vec3((float)(i % sizeX) - 1, (float)(yPos), 1.0f));
				break;
			default:
				break;
			}

			// Updates yPos to ensure correct coordinates
			if (i % sizeX == 0 && i != 0)
				yPos++;
		}

		// Adding vertices and entity positions to entityData
		/*entityData.vertices.push_back(map[0].botLeft);
		entityData.vertices.push_back(map[0].botRight);
		entityData.vertices.push_back(map[0].topLeft);
		entityData.vertices.push_back(map[0].topRight);*/

		//entityData.positions.push_back(playerPos); // Player pos gets added first

		for (const auto& element : ghostPos)	   // Ghost pos gets added last
			entityData.positions.push_back(element);
	}

}