#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "model.hpp"
#include "globals.hpp"

namespace framework{

	struct Object {
		glm::vec3 pos;
		glm::vec3 nor;

		Model* model;
	};


	class Map {
	private:
		int sizeX, sizeY, sizeArray;	// Map dimensions information variables
		int numWalls, numCollecs;		// Number of each kind of object to draw
		std::vector<int> array;			// Where the actual map is stored
		framework::Object* map;			// Where tile data of the map is stored
		std::vector<glm::vec3> characterPositions;

	public:
		Map(const std::string& levelPath);
		~Map();

		void CreateMap();
		void PrintMap() const;

		ShaderVertData retMapVertices();
		std::vector<GLuint> retMapIndices(int iterations);

		// Functions that return the private data of this class
		inline std::vector<int> GetArray() const { return array; }
		inline framework::Object* GetMap() const { return map; }
		inline int GetNumWalls() const{ return numWalls; }
		inline int GetNumCollecs() const{ return numCollecs; }
		inline int GetSizeX() const{ return sizeX; }
		inline int GetSizeY() const{ return sizeY; }
		inline std::vector<glm::vec3> GetPGPos() const { return characterPositions; }

	};

}