#include "GameStateManager.h"

GameStateManager::GameStateManager(const char* mapFilepath) {
    initializeBattlefieldVector(mapFilepath);
}

void GameStateManager::initialiseQuadTree(const char* mapFilepath, unsigned int& index) {
    unsigned int mapSize = battlefieldMap.initMap(mapFilepath);
    state.quadTree = new QuadTree(sf::IntRect(0, 0, mapSize * GlobalConstants::cellSize, mapSize * GlobalConstants::cellSize), 0);
    generateQuadTree(state.quadTree, index);

}

void GameStateManager::generateQuadTree(QuadTree* root, unsigned int& index) {
    float size = root->quadRect.getSize().x / 2;
    if (size > GlobalConstants::cellSize) {

        std::array<QuadTree*, 4> children;

        for (int i = 0; i < 4; i++) {

            children[i] = new QuadTree(
                sf::IntRect(
                    root->quadRect.getPosition().x + size * (i % 2),
                    root->quadRect.getPosition().y + size * ((int)(i > 1)),
                    size, size
                ), 
                root->depth + 1
            );

            generateQuadTree((QuadTree*)children[i], index);
        }
        root->children = children;
    } else {

        std::array<QuadTree*, 4> children;

        for (int i = 0; i < 4; i++) {
            int x = root->quadRect.getPosition().x + size * (i % 2);
            int y = root->quadRect.getPosition().y + size * ((int)(i > 1)); 

            state.BattlefieldVector[index] = generateCell(x / GlobalConstants::cellSize, y/ GlobalConstants::cellSize);

            std::vector<BattlefieldCell>::iterator iter = state.BattlefieldVector.begin() + (index++);

            children[i] = new QuadTreeLeaf(
                sf::IntRect(x, y, size, size), 
                root->depth + 1,
                iter
            );
        }
        root->children = children;
    }
}

BattlefieldCell GameStateManager::generateCell(int x, int y)
{
    BattlefieldCell newCell;
    newCell.x = x;
    newCell.y = y;
    newCell.vertices = battlefieldMap.getVertices(x, y);
    newCell.terrain = battlefieldMap.getTerrainInstanceAtPosition({ x, y });
    newCell.YOffset = battlefieldMap.getHeightAtPosition({ x, y }) * GlobalConstants::cellSize / 4;

    return newCell;
}

BattlefieldCell* GameStateManager::getCell(int x, int y) {
    for (auto& cell : state.BattlefieldVector) {
        if (cell.x == x && cell.y == y) {
            return &cell;
        }
    }
    return nullptr;
}

void GameStateManager::initializeBattlefieldVector(const char* mapFilepath) 
{
    state.BattlefieldVector.clear();
    state.BattlefieldVector.resize(GlobalConstants::mapSize * GlobalConstants::mapSize); 
    unsigned int index = 0;
    initialiseQuadTree(mapFilepath, index);
}
