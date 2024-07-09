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
	spriteSheetList.push_back(std::make_pair("GrassTerrain", SpriteSheet("../resources/images/Terrain/grass/grass_spritesheet.png", 8, 2)));
	spriteSheetList.push_back(std::make_pair("Tree", SpriteSheet("../resources/images/Scenery/Tree.png", 1, 1)));
	spriteSheetList.push_back(std::make_pair("PathfinderAgent", SpriteSheet("../resources/images/Flyers/The_Red_Baron/RedBaron.png", 3, 3)));
	
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