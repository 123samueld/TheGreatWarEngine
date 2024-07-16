#include "BattlefieldMap.h"

#include <cmath>
#include <iostream>

void BattlefieldMap::initMap(unsigned int mapSize)
{
    std::ifstream mapDataFile("../resources/maps/map.json");

    if (!mapDataFile.is_open())
    {
        std::cerr << "Unable to open the map file" << std::endl;
        return;
    }
    mapDataFile >> mapData;
    mapDataFile.close();

    size = mapData["MapData"]["MapSize"][0];

    initDepthMap();
    initDirectionMap();
    initTerrainMap();
}

void BattlefieldMap::initDepthMap()
{
    depthMap = new int* [size];
    
    for (int i = 0; i < size; i++)
    {
        depthMap[i] = new int[size];
        for (int j = 0; j < size; j++)
        {
            depthMap[i][j] = mapData["MapData"]["HeightMap"][i][j];
        }
    }
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

void BattlefieldMap::initTerrainMap() {
    terrainMap = new TerrainInstance * [size];
    for (int i = 0; i < size; ++i) {
        terrainMap[i] = new TerrainInstance[size];
        for (int j = 0; j < size; ++j) {
            std::string terrainKey = mapData["MapData"]["TerrainMap"][i][j];
            std::string thisTerrainTypeName;

            if (mapData["MapData"]["RequiredTerrainTypes"].contains(terrainKey)) {
                thisTerrainTypeName = mapData["MapData"]["RequiredTerrainTypes"][terrainKey];
            }
            else {
                std::cerr << "Unknown terrain type key: " << terrainKey << std::endl;
                continue;
            }

            bool found = false;
            for (const auto& terrainType : terrainTypes) {
                if (terrainType.name == thisTerrainTypeName) {
                    terrainMap[i][j] = TerrainInstance(terrainType);
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cerr << "Unknown terrain type name: " << thisTerrainTypeName << std::endl;
            }
        }
    }

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
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
