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
    GameStateManager();

    void initialiseQuadTree(unsigned int& index);
    void initializeBattlefieldVector();


    GameState& getState() { return state; }
    void generateQuadTree(QuadTree* root, unsigned int& index);
    BattlefieldCell generateCell(int x, int y);

    BattlefieldCell* getCell(int x, int y);

    BattlefieldMap battlefieldMap;
    GameState state;
};
  