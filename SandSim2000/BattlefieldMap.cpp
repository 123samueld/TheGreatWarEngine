#include "BattlefieldMap.h"

int BattlefieldMap::initMap(const char* filepath)
{
    std::ifstream mapDataFile(filepath);

    if (!mapDataFile.is_open())
    {
        std::cerr << "Unable to open the map file" << std::endl;
        return 1;
    }
    mapDataFile >> mapData;
    mapDataFile.close();

    size = mapData["MapData"]["MapSize"][0];

    initDepthMap();
    initDirectionMap();
    initTerrainMap();

    return size;
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
    directionMap = new Direction*[size];

    for (int i = 0; i < size; i++)
    {
        directionMap[i] = new Direction[size];
        for (int j = 0; j < size; j++)
        {
            if (i > 0 && j < size - 1 && (depthMap[i - 1][j] > depthMap[i][j] && depthMap[i][j + 1] > depthMap[i][j]))
                directionMap[i][j] = NIW;
            else if (i < size - 1 && j < size - 1 &&
                     (depthMap[i + 1][j] > depthMap[i][j] && depthMap[i][j + 1] > depthMap[i][j]))
                directionMap[i][j] = NIE;
            else if (i < size - 1 && j > 0 &&
                     (depthMap[i + 1][j] > depthMap[i][j] && depthMap[i][j - 1] > depthMap[i][j]))
                directionMap[i][j] = SIE;
            else if (i > 0 && j > 0 && (depthMap[i - 1][j] > depthMap[i][j] && depthMap[i][j - 1] > depthMap[i][j]))
                directionMap[i][j] = SIW;

            else if (j < size - 1 && depthMap[i][j + 1] > depthMap[i][j])
                directionMap[i][j] = N;
            else if (j > 0 && depthMap[i][j - 1] > depthMap[i][j])
                directionMap[i][j] = S;
            else if (i < size - 1 && depthMap[i + 1][j] > depthMap[i][j])
                directionMap[i][j] = E;
            else if (i > 0 && depthMap[i - 1][j] > depthMap[i][j])
                directionMap[i][j] = W;

            else if (i < size - 1 && j < size - 1 && depthMap[i + 1][j + 1] > depthMap[i][j])
                directionMap[i][j] = NE;
            else if (i < size - 1 && j > 0 && depthMap[i + 1][j - 1] > depthMap[i][j])
                directionMap[i][j] = SE;
            else if (i > 0 && j > 0 && depthMap[i - 1][j - 1] > depthMap[i][j])
                directionMap[i][j] = SW;
            else if (i > 0 && j < size - 1 && depthMap[i - 1][j + 1] > depthMap[i][j])
                directionMap[i][j] = NW;
            // Liam, I've attempted to put in a new terrain type which is "Peak", it's in the terrain sprite sheets 
            // already. But not sure even how it's currently selecting the right sprite based on heights. 
            // I think it might be easist to always make the very highest depth a Peak tile no matter what the
            // neighbours are, this will allow for sharp peaks that don't need to slope up or down. 
            // 
            // Would be really nice to have cliffs to so not all hills are slopes, but that's a nice-to-have, we'll skip it
            // and keep focus on movement and shooting. 
            else if (i == -1 and j == 10)
                directionMap[i][j] = P;

            else
                directionMap[i][j] = F;
        }
    }
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

    /*
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == y && j == x) {
                directionValue = directionMap[i][j];
                break;
            }
        }
    }
    */
    
    directionValue = directionMap[y][x];
    
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
        case 9:
            vertices.push_back(sf::Vector2i(0, -25));
            vertices.push_back(sf::Vector2i(50, 0));
            vertices.push_back(sf::Vector2i(0, 25));
            vertices.push_back(sf::Vector2i(-50, 25));
            break;
        case 10:
            vertices.push_back(sf::Vector2i(0, 0));
            vertices.push_back(sf::Vector2i(50, 0));
            vertices.push_back(sf::Vector2i(0, 25));
            vertices.push_back(sf::Vector2i(-50, 0));
            break;
        case 11:
            vertices.push_back(sf::Vector2i(0, -25));
            vertices.push_back(sf::Vector2i(50, 25));
            vertices.push_back(sf::Vector2i(0, 25));
            vertices.push_back(sf::Vector2i(-50, 0));
            break;
        case 12:
            vertices.push_back(sf::Vector2i(0, -25));
            vertices.push_back(sf::Vector2i(50, 0));
            vertices.push_back(sf::Vector2i(0, 50));
            vertices.push_back(sf::Vector2i(-50, 0));
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
