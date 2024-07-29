#pragma once
#include "SFML/Graphics.hpp"

#include <vector>

#include "Agent.h"
#include "TerrainInstance.h"


class BattlefieldCell {
public:

    void removeObject(Agent* agent)
    {
        auto it = std::find(Objects.begin(), Objects.end(), agent);
        if (it != Objects.end())
        {
            Objects.erase(it);
        }
    }
    void addObject(Agent* agent)
    {
        Objects.push_back(agent);
    }

    int x, y;
    int windowX, windowY;
    std::vector<sf::Vector2i> vertices;

    std::vector<Agent*> Objects = std::vector<Agent*>();
    TerrainInstance terrain;
    int YOffset;

    bool impassableTerrain = false;
    bool inClosedList = false;

    int gScore = 0;
    int hScore = 0;
    int fScore = 0;

    BattlefieldCell* pathParent = nullptr;

    int id;
};