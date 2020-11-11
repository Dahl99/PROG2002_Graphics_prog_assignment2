#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "tilemap.hpp"
#include "framework/model.hpp"

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


	// Creates a tile and gives that tile 4 vertices for each 
	//  number in the map that is read in the constructor
	void Map::CreateMap()
	{
		map = new framework::Object[sizeArray];

		// Declaration of some variables
		glm::vec3 playerPos;
		std::vector<glm::vec3> ghostPos;

		int yPos = 0;
		int playerLoc = 0;

		// For the the map read in the constructor, check each number and assign possition and colour 
		//  starting at i = 1 because of mod
		for (int i = 1; i <= sizeArray; i++)
		{
			framework::Vertex temp;
			// The cases are Collectibles, walls, player and ghost respectively
			switch (array[i - 1])
			{
			case 0:	// Each vertice of an collectible gets added or subtracted a const
					//  to the vertice positions in order to reduce the size of the collectible
				
				// Bottom left vertex
				temp.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				temp.pos.y = (yPos)+COLLECTIBLESIZE;
				temp.pos.z = 0;
				
				map[i - 1].pos = temp.pos;
				map[i - 1].model = std::make_unique<Model>(WALLMODELPATH);


				//// Bottom right vertex
				//map[i - 1].botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].botRight.pos.y = (yPos)+COLLECTIBLESIZE;


				//// Top left vertex
				//map[i - 1].topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				//map[i - 1].topLeft.pos.y = (yPos + 1) - COLLECTIBLESIZE;


				//// Top right vertex
				//map[i - 1].topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].topRight.pos.y = (yPos + 1) - COLLECTIBLESIZE;

				break;

			case 1:
				

				// Bottom left vertex
				temp.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				temp.pos.y = (yPos)+COLLECTIBLESIZE;
				temp.pos.z = 0;

				map[i - 1].pos = temp.pos;
				map[i - 1].model = std::make_unique<Model>(WALLMODELPATH);
				//// Bottom right vertex
				//map[i - 1].botRight.pos.x = ((i - 1) % sizeX) + 1;
				//map[i - 1].botRight.pos.y = yPos;


				//// Top left vertex
				//map[i - 1].topLeft.pos.x = (i - 1) % sizeX;
				//map[i - 1].topLeft.pos.y = yPos + 1;

				//// Top right vertex
				//map[i - 1].topRight.pos.x = ((i - 1) % sizeX) + 1;
				//map[i - 1].topRight.pos.y = yPos + 1;

				break;

			case 2: // Sets player pos if tile type is 2, also adds collectible vertices


				

				// Bottom left vertex
				temp.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				temp.pos.y = (yPos)+COLLECTIBLESIZE;
				temp.pos.z = 0;

				playerPos = glm::vec3((float)(i % sizeX), (float)(yPos), 1.0f);

				map[i - 1].pos = temp.pos;
				map[i - 1].model = std::make_unique<Model>(WALLMODELPATH);

				//// Bottom right vertex
				//map[i - 1].botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].botRight.pos.y = (yPos)+COLLECTIBLESIZE;


				//// Top left vertex
				//map[i - 1].topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				//map[i - 1].topLeft.pos.y = (yPos + 1) - COLLECTIBLESIZE;


				//// Top right vertex
				//map[i - 1].topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].topRight.pos.y = (yPos + 1) - COLLECTIBLESIZE;

				break;
			case 3: // Sets ghost position if tile type is 3
				ghostPos.push_back(glm::vec3((float)(i % sizeX)-1, (float)(yPos), 1.0f));
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

		entityData.positions.push_back(playerPos); // Player pos gets added first

		for (const auto& element : ghostPos)	   // Ghost pos gets added last
			entityData.positions.push_back(element);
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

	//// Loops through mapsize and puts the data for each tile into one of two vector containers,
	////  one for walls and one for collectibles
	//ShaderVertData Map::retMapVertices()
	//{
	//	// Creates the return data container
	//	ShaderVertData mapVertices;

	//	// Goes through the map and puts each vertice of each tile into either the walls or collectibles container
	//	for (int i = 0; i < sizeArray; i++)
	//	{
	//		if(array[i] && array[i] != 2)
	//		{
	//			mapVertices.wallVertices.push_back(map[i].botLeft);
	//			mapVertices.wallVertices.push_back(map[i].botRight);
	//			mapVertices.wallVertices.push_back(map[i].topLeft);
	//			mapVertices.wallVertices.push_back(map[i].topRight);
	//			numWalls++;
	//		}
	//		else
	//		{
	//			mapVertices.collectibleVertices.push_back(map[i].botLeft);
	//			mapVertices.collectibleVertices.push_back(map[i].botRight);
	//			mapVertices.collectibleVertices.push_back(map[i].topLeft);
	//			mapVertices.collectibleVertices.push_back(map[i].topRight);
	//			numCollecs++;
	//		}
	//	}
	//	return mapVertices;
	//}


	// Creates the indice arrays for the map
	std::vector<GLuint> Map::retMapIndices(int iterations)
	{
		// Create the return value container
		std::vector<GLuint> indices;

		// Adds the indices to create the two triangles of each square\
			the order is 1, 2, 3 and 3, 4, 1 because vertexes are added in botleft, botright, topleft, topright
		for (GLuint i = 0; i < iterations; i++)
		{
			indices.push_back(i * 4);
			indices.push_back((i * 4) + 1);
			indices.push_back((i * 4) + 2);
			indices.push_back((i * 4) + 2);
			indices.push_back((i * 4) + 3);
			indices.push_back((i * 4) + 1);
		}		
		
		return indices;
	}
}