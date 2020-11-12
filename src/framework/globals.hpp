#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <memory>
#include <vector>

namespace framework {

	const std::string LEVELPATH0 = "../res/levels/level0";

	const std::string PACMANMODELPATH = "../res/models/pacman/pacman.obj";
	const std::string PACMANPICTUREPATH = "../res/models/pacman/pacman_texture.png";
	
	const std::string GHOSTMODELPATH = "../res/models/ghosts/ghost.obj";
	const std::string GHOSTREDPICTUREPATH = "../res/models/ghosts/ghost_red.png";
	const std::string GHOSTBLUEPICTUREPATH = "../res/models/ghosts/ghost_blue.png";
	const std::string GHOSTORANGEPICTUREPATH = "../res/models/ghosts/ghost_orange.png";
	const std::string GHOSTPINKPICTUREPATH = "../res/models/ghosts/ghost_pink.png";

	const std::string WALLPICTUREPATH = "../res/models/cube/cubetexture.png";
	const std::string WALLMODELPATH = "../res/models/cube/slime.obj";

	const std::string COLLECTIBLEPICTUREPATH = "../res/models/cube/SlimeTexture.png";
	const std::string COLLECTIBLEMODELPATH = "../res/models/cube/collectible.obj";


	const std::string TILEVERTSHADERPATH = "../res/shaders/tile.vert";
	const std::string TILEFRAGSHADERPATH = "../res/shaders/tile.frag";

	const std::string VERTGSHADERPATH = "../res/shaders/shader.vert";
	const std::string FRAGSHADERPATH = "../res/shaders/shader.frag";

	const std::string SOUNDTRACKPATH = "../res/music/pacman.wav";
	const float MUSICVOLUME = 0.2f;

	const int WINDOWSIZEX = 1280, WINDOWSIZEY = 720;
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

	struct IndiceData {
		std::vector<GLuint> walls;
		std::vector<GLuint> collectibles;
	};
}