#pragma once
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <json.hpp>
#include <fstream>
#include <cmath>

#include "SFML/Graphics.hpp"

#include "GlobalConstants.h"
#include "SpriteManager.h"
#include "TerrainInstance.h"


enum Direction
{
	F, N, NE, E, SE, S, SW, W, NW,
	NIW, NIE, SIE, SIW
};

class BattlefieldMap
{
public:
	BattlefieldMap() {}
	~BattlefieldMap();

	TerrainInstance getTerrainInstanceAtPosition(sf::Vector2i position) {return terrainMap[position.y][position.x]; }
	int getHeightAtPosition(sf::Vector2i position) { return depthMap[position.y][position.x]; }

	int initMap(const char* filepath);
	std::vector<sf::Vector2i> getVertices(int x, int y);
	int size = 0;

private:
	std::vector<TerrainType> terrainTypes = std::vector<TerrainType>(
		{
			TerrainType("Grass", "../resources/images/Terrain/Terrain Sprites/grass_spritesheet.png"),
			TerrainType("Stone", "../resources/images/Terrain/Terrain Sprites/stone_spritesheet.png"),
			TerrainType("Water", "../resources/images/Terrain/Terrain Sprites/water_spritesheet.png"),

			TerrainType("GrassStone", "../resources/images/Terrain/Terrain Sprites/grass_stone_spritesheet.png")
		}
	);

	nlohmann::json mapData;

	int** depthMap = nullptr;
	Direction** directionMap = nullptr;
	TerrainInstance** terrainMap = nullptr;

	void initDepthMap();
	void initDirectionMap();
	void initTerrainMap();

	int testLocation(int x, int y, int height);
};