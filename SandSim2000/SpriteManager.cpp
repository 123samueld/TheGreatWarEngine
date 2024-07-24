#include "SpriteManager.h"
#include <iostream>

SpriteManager* SpriteManager::_instance = nullptr;;

SpriteManager::SpriteManager()
{
	if (_instance == nullptr)
		_instance = this;
	else
		std::cout << "2 instances of SpriteManager have been created" << std::endl;

	spriteSheetList.push_back(std::make_pair("RedBaron", SpriteSheet("../resources/images/Flyers/The_Red_Baron/RedBaron.png", 3, 3)));
	spriteSheetList.push_back(std::make_pair("Tree", SpriteSheet("../resources/images/Scenery/Tree.png", 1, 1)));
	spriteSheetList.push_back(std::make_pair("PathfinderAgent", SpriteSheet("../resources/images/Flyers/The_Red_Baron/RedBaron.png", 3, 3)));
	spriteSheetList.push_back(std::make_pair("Snow_Pine", SpriteSheet("../resources/images/Scenery/snow_pine.png", 1, 1)));
}

void SpriteManager::SetRequiredTerrainSpriteSheetList(TerrainInstance** terrainMap, int mapSize)
{
	std::vector<std::string> loadedTerrain = std::vector<std::string>();

	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			TerrainInstance terrain = terrainMap[i][j];
			bool shouldAdd = true;
			if (loadedTerrain.size() != 0)
			{
				for (int x = 0; x < loadedTerrain.size(); x++)
				{
					if (terrain.terrainTypes[0].name == loadedTerrain[x])
						shouldAdd = false;
				}
			}
			if(shouldAdd)
			{
				for (int y = 0; y < terrain.terrainTypes.size(); y++)
				{
					spriteSheetList.push_back(std::make_pair(terrain.terrainTypes[y].name, SpriteSheet(terrain.terrainTypes[y].spriteSheetFilePath.c_str(), 8, 2)));
					loadedTerrain.push_back(terrain.terrainTypes[y].name);
				}
			}
		}
	}
}

sf::Sprite* SpriteManager::GetSprite(std::string spriteSheetID, int spriteIndex)
{
	for (auto it = spriteSheetList.begin(); it != spriteSheetList.end(); ++it)
	{
		if (it->first == spriteSheetID)
		{
			return it->second.getSprite(spriteIndex);
		}
	}
	std::cout << "[INVALID SPRITE NOT IN LIST]" << std::endl;
}

SpriteSheet& SpriteManager::GetSpriteSheet(std::string spriteSheetID)
{
	for (auto it = spriteSheetList.begin(); it != spriteSheetList.end(); ++it)
	{
		if (it->first == spriteSheetID)
		{
			return it->second;
		}
	}
	std::cerr << "[INVALID SPRITE NOT IN LIST]" << std::endl;
}

SpriteManager* SpriteManager::GetInstance()
{
	if (_instance == nullptr)
		_instance = new SpriteManager();
	return _instance;
}