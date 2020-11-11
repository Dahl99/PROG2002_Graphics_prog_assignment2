#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace framework {

	const std::string LEVELPATH0 = "../res/levels/level0";

	const std::string PACMANMODELPATH = "../res/models/pacman/pacman.obj";
	const std::string PACMANPICTUREPATH = "../res/models/pacman/pacman_texture.png";
	
	const std::string GHOSTMODELPATH = "../res/models/ghosts/ghost.obj";
	const std::string GHOSTPICTUREPATH = "../res/models/ghosts/ghost_red.png";

	const std::string WALLPICTUREPATH = "../res/models/cube/cubetexture.png";
	const std::string WALLMODELPATH = "../res/models/cube/cube.obj";

	const std::string TILEVERTSHADERPATH = "../res/shaders/tile.vert";
	const std::string TILEFRAGSHADERPATH = "../res/shaders/tile.frag";

	const std::string CHARVERTGSHADERPATH = "../res/shaders/character.vert";
	const std::string CHARFRAGSHADERPATH = "../res/shaders/character.frag";

	const std::string SOUNDTRACKPATH = "../res/music/pacman.wav";
	const float MUSICVOLUME = 0.25f;

	const int WINDOWSIZEX = 28*28, WINDOWSIZEY = 36*28;
	const float COLLECTIBLESIZE = 0.35f;
	const int COLLECTIBLESCORE = 10;



//-------------------------------------------------------------------------------------
//									Tile Structs
//-------------------------------------------------------------------------------------

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 nor;
		glm::vec2 tex;

		bool operator==(const Vertex& other) const
		{
			return (pos == other.pos
				&& nor == other.nor
				&& tex == other.tex);
		}
	};

	struct Tile {
		Vertex botLeft, botRight, topLeft, topRight;
	};

	struct ShaderVertData {
		std::vector<framework::Vertex> wallVertices;
		std::vector<framework::Vertex> collectibleVertices;
	};

	struct EntityPosData {
		std::vector<Vertex> vertices;
		std::vector<glm::vec3> positions;
	};
}