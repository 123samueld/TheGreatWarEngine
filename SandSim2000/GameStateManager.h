#pragma once
#include <cmath>
#include <set>
#include "GameState.h"
#include "BattlefieldMap.h"
#include "BattlefieldCell.h"
#include "Agent.h"
#include "InputStateManager.h"
#include "GridGenerator.h"


class GameStateManager {
public:
    GameStateManager(const char* mapFilepath);

    void initialiseQuadTree(const char* mapFilepath, unsigned int& index);
    void initializeBattlefieldVector(const char* mapFilepath);


    GameState& getState() { return state; }
    void generateQuadTree(QuadTree* root, unsigned int& index);
    BattlefieldCell generateCell(int x, int y);

    BattlefieldCell* getCell(int x, int y);

    BattlefieldMap battlefieldMap;
    GameState state;
};
  