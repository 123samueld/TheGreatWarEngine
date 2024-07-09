#pragma once

#include "SFML/Graphics.hpp"

#include <vector>
#include <list>

#include "SpriteSheet.h"

class SpriteManager
{
public:
    // Update constructor to check terrainSpriteSheetList property to choose which terrain sprites to load, rather than the hard coded verison we've got right now.
    SpriteManager();

    // void GetRequiredTerrainSpriteSheetsList();

    sf::Sprite* GetSprite(std::string spriteSheetID, int spriteIndex);
    SpriteSheet& GetSpriteSheet(std::string spriteSheetID);

    static SpriteManager* GetInstance();
    void operator=(const SpriteManager&) = delete;


private:
    //  std::list<std::string> terrainSpriteSheetList;
    static SpriteManager* _instance;

    std::list<std::pair<std::string, SpriteSheet>> spriteSheetList;
};