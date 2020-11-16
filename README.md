# Assignment 2: 3D Pac-Man

## About

This project contains a 3D remake of Pac-Man in C++ using OpenGL 4.3. In the game you're able to move around as Pac-Man in 3rd person using both WASD and the arrow keys to collect pellets.
There's also 4 ghosts roaming around the map and trying to catch you. All of the ghosts and Pac-Man are loaded using a model, but only Pac-Man is animated using a second model.
The level is created using a tilemap and with models that are batched together into 1 draw call for walls and 1 for pellets. The current score is also being kept track of
and displayed in the upper left corner. The soundtrack from the original game is also playing in the background while the game is running. Being caught by a ghost causes
the game to just *terminate*. The game can also be terminated by pressing *escape*. Lighting is done by having ghosts and pac-man as point lights using the phong lighting model with attenuation.

## Built With

This assignment was built using the following libraries:

* OpenGL ver. 4.3
* GLEW ver. 2.1.0
* GLFW ver. 3.3.2
* GLM ver. 0.9.9
* ImGui
* IrrKlang
* STB
* tinyobjloader

## Getting Started

### Windows

This is an example on how to set up the project locally using Visual Studio 2019 with Cmake addon.

#### Prerequisites

Before you begin make sure you have the following installed:

* Visual Studio 2019
* Cmake addon for Visual Studio 2019

#### Installation

1. Clone the repository using git
2. Right click on the root folder containing all files in repository
3. Click on "Open in Visual Studio"
4. The project will now be open in Visual Studio and Cmake should automatically generate the project
5. The project should now be set up locally and is ready to build

### Linux

As both of us developed the project on Windows; we have no idea how to set it up on Linux. But it should be possible to just use Cmake as well.

## Creators

This project was developed by:

* Kristoffer Madsen
* Daniel Dahl

## Acknowledgements
