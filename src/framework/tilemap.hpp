#pragma once
#include <string>
#include <vector>
#include "globals.hpp"

namespace framework{

	class Map {
	private:
		int sizeX, sizeY, sizeArray;	// Map dimensions information variables
		int numWalls, numCollecs;		// Number of each kind of object to draw
		std::vector<int> array;			// Where the actual map is stored
		framework::Cube* map;			// Where tile data of the map is stored
		EntityPosData entityData;		// Position of pacman and ghosts

	public:
		Map(const std::string& levelPath);
		~Map();

		void CreateMap();
		void PrintMap() const;

		ShaderVertData retMapVertices();
		std::vector<GLuint> retMapIndices(int iterations);

		// Functions that return the private data of this class
		inline std::vector<int> GetArray() const { return array; }
		inline framework::Cube* GetMap() const { return map; }
		inline int GetNumWalls() const{ return numWalls; }
		inline int GetNumCollecs() const{ return numCollecs; }
		inline int GetSizeX() const{ return sizeX; }
		inline int GetSizeY() const{ return sizeY; }
		inline framework::EntityPosData GetPGPos() const { return entityData; }

	};

}