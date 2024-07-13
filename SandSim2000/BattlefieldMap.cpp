#include "BattlefieldMap.h"
#include <cmath>
#include <iostream>

void BattlefieldMap::initMap(unsigned int mapSize)
{
    size = static_cast<int>(mapSize);
        
    initDepthMap();
    initDirectionMap();
    initTerrainMap();
}

void BattlefieldMap::initDepthMap()
{
    depthMap = new int* [size];
    
    depthMap[0] = new int[size]     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    depthMap[1] = new int[size]     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    depthMap[2] = new int[size]     {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[3] = new int[size]     {0, 0, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[4] = new int[size]     {0, 0, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[5] = new int[size]     {0, 0, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[6] = new int[size]     {0, 0, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[7] = new int[size]     {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[8] = new int[size]     {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 0, 0};
    depthMap[9] = new int[size]     {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 0, 0};
    depthMap[10] = new int[size]    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[11] = new int[size]    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[12] = new int[size]    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[13] = new int[size]    {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    depthMap[14] = new int[size]    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    depthMap[15] = new int[size]    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
}


void BattlefieldMap::initDirectionMap()
{
    directionMap = new Direction * [size];
    
    directionMap[0] = new Direction[size]   { F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F };
    directionMap[1] = new Direction[size]   { F, NE, E, E, E, E, E, E, E, E, E, E, E, E, SE, F };
    directionMap[2] = new Direction[size]   { F, N, NE, E, E, SE, F, F, F, F, F, F, F, F, S, F };
    directionMap[3] = new Direction[size]   { F, N, N, F, F, S, F, F, F, F, F, F, F, F, S, F };
    directionMap[4] = new Direction[size]   { F, N, N, F, F, S, F, F, F, F, F, F, F, F, S, F };
    directionMap[5] = new Direction[size]   { F, N, N, F, F, S, F, F, F, F, F, F, F, F, S, F };
    directionMap[6] = new Direction[size]   { F, N, N, F, F, S, F, F, F, F, F, F, F, F, S, F };
    directionMap[7] = new Direction[size]   { F, N, NW, W, W, SW, F, F, F, NE, E, E, E, SE, S, F };
    directionMap[8] = new Direction[size]   { F, N, F, F, F, F, F, F, F, N, F, F, F, S, S, F };
    directionMap[9] = new Direction[size]   { F, N, F, F, F, F, F, F, F, N, F, F, F, S, S, F };
    directionMap[10] = new Direction[size]  { F, N, F, F, F, F, F, F, F, NW, W, W, W, SW, S, F };
    directionMap[11] = new Direction[size]  { F, N, F, F, F, F, F, F, F, F, F, F, F, F, S, F };
    directionMap[12] = new Direction[size]  { F, N, F, F, F, F, F, F, F, F, F, F, F, F, S, F };
    directionMap[13] = new Direction[size]  { F, N, F, F, F, F, F, F, F, F, F, F, F, F, S, F };
    directionMap[14] = new Direction[size]  { F, NW, W, W, W, W, W, W, W, W, W, W, W, W, SW, F };
    directionMap[15] = new Direction[size]  { F, F, F, F, F, F, F, F, F, F, F, F, F, F, F, F };
    
}

void BattlefieldMap::initTerrainMap()
{
    terrainMap = new TerrainInstance* [size];

    TerrainInstance grass = TerrainInstance(Grass);
    TerrainInstance stone = TerrainInstance(Stone);
    TerrainInstance water = TerrainInstance(Water);
    
    TerrainInstance grassStone = TerrainInstance(std::vector<TerrainType>({Grass, Stone}));
    TerrainInstance stoneWater = TerrainInstance(std::vector<TerrainType>({Water, Stone}));

    terrainMap[0] = new TerrainInstance[size]{ grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass };
    terrainMap[1] = new TerrainInstance[size]{ grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass };
    terrainMap[2] = new TerrainInstance[size]{ grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass };
    terrainMap[3] = new TerrainInstance[size]{ grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass };
    terrainMap[4] = new TerrainInstance[size]{ grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass, grass };
    terrainMap[5] = new TerrainInstance[size]{ grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone, grassStone };
    terrainMap[6] = new TerrainInstance[size]{ stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone };
    terrainMap[7] = new TerrainInstance[size]{ stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone };
    terrainMap[8] = new TerrainInstance[size]{ stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone };
    terrainMap[9] = new TerrainInstance[size]{ stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone };
    terrainMap[10] = new TerrainInstance[size]{ stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone, stone };
    terrainMap[11] = new TerrainInstance[size]{ stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater, stoneWater };
    terrainMap[12] = new TerrainInstance[size]{ water, water, water, water, water, water, water, water, water, water, water, water, water, water, water, water };
    terrainMap[13] = new TerrainInstance[size]{ water, water, water, water, water, water, water, water, water, water, water, water, water, water, water, water };
    terrainMap[14] = new TerrainInstance[size]{ water, water, water, water, water, water, water, water, water, water, water, water, water, water, water, water };
    terrainMap[15] = new TerrainInstance[size]{ water, water, water, water, water, water, water, water, water, water, water, water, water, water, water, water };

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            terrainMap[y][x].setSpriteIndex(directionMap[y][x]);
        }
    }
    SpriteManager::GetInstance()->SetRequiredTerrainSpriteSheetList(terrainMap, size);
}

int BattlefieldMap::testLocation(int x, int y, int height)
{
    if (x > 0 && x < size - 1 && y > 0 && y < size - 1)
        return depthMap[y][x] - height;
    return 0;
}

std::vector<sf::Vector2i> BattlefieldMap::getVertices(int x, int y)
{
    std::vector<sf::Vector2i> vertices;
    int directionValue;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == y && j == x) {
                directionValue = directionMap[i][j];
                break;
            }
        }
    }

    
    switch (directionValue)
    {
        case 0:
            vertices.push_back(sf::Vector2i(0, 0));
            vertices.push_back(sf::Vector2i(50, 25));
            vertices.push_back(sf::Vector2i(0, 50));
            vertices.push_back(sf::Vector2i(-50, 25));
            break;
        case 1: 
            vertices.push_back(sf::Vector2i(0, 0));
            vertices.push_back(sf::Vector2i(50, 0));
            vertices.push_back(sf::Vector2i(0, 25));
            vertices.push_back(sf::Vector2i(-50, 25));
            break;

        case 2:
            vertices.push_back(sf::Vector2i(0, 0));
            vertices.push_back(sf::Vector2i(50, 25));
            vertices.push_back(sf::Vector2i(-50, 25));
            break;
        case 3:
            vertices.push_back(sf::Vector2i(0, 0));
            vertices.push_back(sf::Vector2i(50, 25));
            vertices.push_back(sf::Vector2i(0, 25));
            vertices.push_back(sf::Vector2i(-50, 0));
            break;
        case 4:
            vertices.push_back(sf::Vector2i(0, 0));
            vertices.push_back(sf::Vector2i(50, 25));
            vertices.push_back(sf::Vector2i(0, 50));
            vertices.push_back(sf::Vector2i(-50, 0));
            break;
        case 5:
            vertices.push_back(sf::Vector2i(0, -25));
            vertices.push_back(sf::Vector2i(50, 25));
            vertices.push_back(sf::Vector2i(0, 50));
            vertices.push_back(sf::Vector2i(-50, 0));
            break;
        case 6:
            vertices.push_back(sf::Vector2i(0, -25));
            vertices.push_back(sf::Vector2i(50, 50));
            vertices.push_back(sf::Vector2i(0, 50));
            vertices.push_back(sf::Vector2i(-50, 25));
            break;
        case 7:
            vertices.push_back(sf::Vector2i(0, -25));
            vertices.push_back(sf::Vector2i(50, 0));
            vertices.push_back(sf::Vector2i(0, 50));
            vertices.push_back(sf::Vector2i(-50, 25));
            break;
        case 8:
            vertices.push_back(sf::Vector2i(0, -25));
            vertices.push_back(sf::Vector2i(50, -25));
            vertices.push_back(sf::Vector2i(0, 50));
            vertices.push_back(sf::Vector2i(-50, 25));
            break;
        default: 
            break;
    } 

    return vertices;
}

// Add TerrainTypes to this destructor
BattlefieldMap::~BattlefieldMap()
{
    for (int i = 0; i < size; i++) {
        delete[] depthMap[i];
        delete[] directionMap[i];
        delete[] terrainMap[i];
    }
    delete[] depthMap;
    delete[] directionMap;
    delete[] terrainMap;
}
