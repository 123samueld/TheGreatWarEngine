#pragma once
#include <vector>
#include <list>
#include <string>
#include <iostream>

#include "SFML/Graphics.hpp"
#include "SpriteManager.h"
#include "TerrainInstance.h"

enum Direction
{
	F, N, NE, E, SE, S, SW, W, NW
};

class BattlefieldMap
{
public:
	BattlefieldMap() {}
	~BattlefieldMap();

	TerrainInstance getTerrainInstanceAtPosition(sf::Vector2i position) {return terrainMap[position.y][position.x]; }
	int getHeightAtPosition(sf::Vector2i position) { return depthMap[position.y][position.x]; }

	void initMap(unsigned int mapSize);
	std::vector<sf::Vector2i> getVertices(int x, int y);
	int size = 0;

private:
	TerrainType Grass = TerrainType("Grass", "../resources/images/Terrain/Terrain Sprites/grass_spritesheet.png");
	TerrainType Stone = TerrainType("Stone", "../resources/images/Terrain/Terrain Sprites/stone_spritesheet.png");
	TerrainType Water = TerrainType("Water", "../resources/images/Terrain/Terrain Sprites/water_spritesheet.png");

	int** depthMap = nullptr;
	Direction** directionMap = nullptr;
	TerrainInstance** terrainMap = nullptr;

	void initDepthMap();
	void initDirectionMap();
	void initTerrainMap();

	int testLocation(int x, int y, int height);
};