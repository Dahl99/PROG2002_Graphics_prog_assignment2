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

		int zPos = 0;
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
				temp.pos.x = ((i - 1) % sizeX);
				temp.pos.y = 0.0;
				temp.pos.z = (zPos);
				
				map[i - 1].pos = temp.pos;
				map[i - 1].model = new Model(COLLECTIBLEMODELPATH);

				for (int j = 0; j < map[i - 1].model->m_Vertices.size(); j++)
				{
					map[i - 1].model->m_Vertices[j].pos += temp.pos + 0.5f;
					//float tempo = map[i - 1].model->m_Vertices[j].pos.y;
					//if (map[i - 1].model->m_Vertices[j].pos.z < 0)
					//	map[i - 1].model->m_Vertices[j].pos.y = 0;
					//else map[i - 1].model->m_Vertices[j].pos.y = 1;
					////map[i - 1].model->m_Vertices[j].pos.y = map[i - 1].model->m_Vertices[j].pos.z;
					//map[i - 1].model->m_Vertices[j].pos.z = tempo;
				}
				//// Bottom right vertex
				//map[i - 1].botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].botRight.pos.y = (zPos)+COLLECTIBLESIZE;


				//// Top left vertex
				//map[i - 1].topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				//map[i - 1].topLeft.pos.y = (zPos + 1) - COLLECTIBLESIZE;


				//// Top right vertex
				//map[i - 1].topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].topRight.pos.y = (zPos + 1) - COLLECTIBLESIZE;

				break;

			case 1:
				

				// Bottom left vertex
				temp.pos.x = ((i - 1) % sizeX);
				temp.pos.y = 0;
				temp.pos.z = (zPos);

				map[i - 1].pos = temp.pos;
				map[i - 1].model = new Model(WALLMODELPATH);

				for (int j = 0; j < map[i - 1].model->m_Vertices.size(); j++)
				{	
					map[i - 1].model->m_Vertices[j].pos += temp.pos + 0.5f;
					//float tempo = map[i - 1].model->m_Vertices[j].pos.y;
					//if (map[i - 1].model->m_Vertices[j].pos.z < 0)
					//	map[i - 1].model->m_Vertices[j].pos.y = 0;
					//else map[i - 1].model->m_Vertices[j].pos.y = 1;
					////map[i - 1].model->m_Vertices[j].pos.y = map[i - 1].model->m_Vertices[j].pos.z;
					//map[i - 1].model->m_Vertices[j].pos.z = tempo;
				}
				//// Bottom right vertex
				//map[i - 1].botRight.pos.x = ((i - 1) % sizeX) + 1;
				//map[i - 1].botRight.pos.y = zPos;


				//// Top left vertex
				//map[i - 1].topLeft.pos.x = (i - 1) % sizeX;
				//map[i - 1].topLeft.pos.y = zPos + 1;

				//// Top right vertex
				//map[i - 1].topRight.pos.x = ((i - 1) % sizeX) + 1;
				//map[i - 1].topRight.pos.y = zPos + 1;

				break;

			case 2: // Sets player pos if tile type is 2, also adds collectible vertices


				

				// Bottom left vertex
				temp.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				temp.pos.y = 0;
				temp.pos.z = (zPos)+COLLECTIBLESIZE;

				playerPos = glm::vec3((float)(i % sizeX), 0.f, (float)zPos);

				map[i - 1].pos = temp.pos;
				map[i - 1].model = new Model(COLLECTIBLEMODELPATH);

				for (int j = 0; j < map[i - 1].model->m_Vertices.size(); j++)
				{
					map[i - 1].model->m_Vertices[j].pos += temp.pos + 0.5f;
					/*float tempo = map[i - 1].model->m_Vertices[j].pos.y;
					map[i - 1].model->m_Vertices[j].pos.y = map[i - 1].model->m_Vertices[j].pos.z;
					map[i - 1].model->m_Vertices[j].pos.z = tempo;*/
				}

				//// Bottom right vertex
				//map[i - 1].botRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].botRight.pos.y = (zPos)+COLLECTIBLESIZE;


				//// Top left vertex
				//map[i - 1].topLeft.pos.x = ((i - 1) % sizeX) + COLLECTIBLESIZE;
				//map[i - 1].topLeft.pos.y = (zPos + 1) - COLLECTIBLESIZE;


				//// Top right vertex
				//map[i - 1].topRight.pos.x = (((i - 1) % sizeX) + 1) - COLLECTIBLESIZE;
				//map[i - 1].topRight.pos.y = (zPos + 1) - COLLECTIBLESIZE;

				break;
			default:
				break;
			}

			// Updates zPos to ensure correct coordinates
			if (i % sizeX == 0 && i != 0)
				zPos++;
		}

		// Adding vertices and entity positions to entityData
		/*entityData.vertices.push_back(map[0].botLeft);
		entityData.vertices.push_back(map[0].botRight);
		entityData.vertices.push_back(map[0].topLeft);
		entityData.vertices.push_back(map[0].topRight);*/

		characterPositions.push_back(playerPos); // Player pos gets added first
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
	ShaderVertData Map::RetMapVertices()
	{
		// Creates the return data container
		ShaderVertData mapVertices;

		// Goes through the map and puts each vertice of each tile into either the walls or collectibles container
		for (int i = 0; i < sizeArray; i++)
		{
			if(array[i] && array[i] != 2)
			{
				for (int j = 0; j < map[i].model->GetVertices().size(); j++)
					mapVertices.wallVertices.push_back(map[i].model->GetVertices()[j]);
				numWalls++;
			}
			else
			{
				for (int j = 0; j < map[i].model->GetVertices().size(); j++)
					mapVertices.collectibleVertices.push_back(map[i].model->GetVertices()[j]);
				auto x = mapVertices.wallVertices.max_size();
				numCollecs++;
			}
		}


		for (int i = 0; i < NUMGHOSTS; i++)
		{
			const auto temp = mapVertices.collectibleVertices[(numCollecs / 4) + (i * 24)].pos;

			characterPositions.push_back(temp);
		}
		return mapVertices;
	}


	// Creates the indice arrays for the map
	IndiceData Map::RetMapIndices()
	{
		// Create the return value container
		IndiceData indices;
		Model collmodel = Model(COLLECTIBLEMODELPATH);
		
		int stride = collmodel.m_Vertices.size();
		for (uint32_t i = 0; i < numCollecs; i++)
		{
			for (int j = 0; j < collmodel.GetIndices().size(); j++)
			{
				indices.collectibles.push_back(collmodel.GetIndices()[j] + (i * stride));
			}
		}

		Model wallmodel = Model(WALLMODELPATH);

		stride = wallmodel.m_Vertices.size();
		for (uint32_t i = 0; i < numWalls; i++)
		{
			for (int j = 0; j < wallmodel.GetIndices().size(); j++)
			{
				indices.walls.push_back(wallmodel.GetIndices()[j] + (i * stride));
			}
		}		
		
		return indices;
	}
}