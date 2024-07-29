#include "MovementManager.h"

void MovementManager::SetUnitPath(PathfinderAgent* agent, BattlefieldCell* goal, GameStateManager* gameStateManager, InputState& state, Scene& scene, Camera* camera)
{
    if (goal != nullptr)
    {
        targetCell = goal;

        //BattlefieldCell* startCell = gameStateManager->getState().quadTree->getCell(gameStateManager->state.quadTree, agent->getPosXIndex() * 100, agent->getPosYIndex() * 100, 4);
        BattlefieldCell* startCell = gameStateManager->getCell(agent->getPosXIndex(), agent->getPosYIndex());

        GridGenerator generator;
        sf::IntRect viewbounds(0, 0, camera->window.getSize().x, camera->window.getSize().y);
        ghostGrid = scene.generateGhostGridFromScene(gameStateManager->getState().quadTree, *camera, generator, viewbounds);

        propagateWaveFrontHeuristics(targetCell, &gameStateManager->state);

        if (AStar(startCell, targetCell) == 1)
        {
            std::vector<BattlefieldCell*>* path = getPath();

            agent->setPath(path);
        }
        cleanHeuristics();
    }
}

void MovementManager::propagateWaveFrontHeuristics(BattlefieldCell* goal, GameState* state)
{
    std::vector<BattlefieldCell*> waveFrontNodes;
    std::unordered_set<BattlefieldCell*, NeibourListHash> neighbourNodes;

    goal->hScore = 10;
    waveFrontNodes.push_back(goal);

    while (!waveFrontNodes.empty())
    {
        for (BattlefieldCell* node : waveFrontNodes)
        {
            int x = node->x; int y = node->y;

            BattlefieldCell* northNeighbour = ghostGrid->getCell(x, y - 1);
            BattlefieldCell* eastNeighbour = ghostGrid->getCell(x + 1, y);
            BattlefieldCell* southNeighbour = ghostGrid->getCell(x, y + 1);
            BattlefieldCell* westNeighbour = ghostGrid->getCell(x - 1, y);

            if (northNeighbour != nullptr && northNeighbour->hScore == 0 && !northNeighbour->impassableTerrain) {
                northNeighbour->hScore = node->hScore + 10;
                neighbourNodes.insert(northNeighbour);
            }

            if (eastNeighbour != nullptr && eastNeighbour->hScore == 0 && !eastNeighbour->impassableTerrain) {
                eastNeighbour->hScore = node->hScore + 10;
                neighbourNodes.insert(eastNeighbour);
            }

            if (southNeighbour != nullptr && southNeighbour->hScore == 0 && !southNeighbour->impassableTerrain) {
                southNeighbour->hScore = node->hScore + 10;
                neighbourNodes.insert(southNeighbour);
            }

            if (westNeighbour != nullptr && westNeighbour->hScore == 0 && !westNeighbour->impassableTerrain) {
                westNeighbour->hScore = node->hScore + 10;
                neighbourNodes.insert(westNeighbour);
            }
        }

        waveFrontNodes.clear();

        for (BattlefieldCell* neighbourNode : neighbourNodes) {
            waveFrontNodes.push_back(neighbourNode);
        }

        neighbourNodes.clear();
    }
}


int MovementManager::AStar(BattlefieldCell* start, BattlefieldCell* goal)
{
    if (start == nullptr)
    {
        std::cerr << "[ERROR][PATHFINDING]--> Start is nullptr" << std::endl;
        return 0;
    }
    if (goal == nullptr)
    {
        std::cerr << "[ERROR][PATHFINDING]--> Goal is nullptr" << std::endl;
        return 0;
    }

    openList.push(start);

    while (!openList.empty())
    {
        BattlefieldCell* current = openList.top();
        openList.pop();

        current->inClosedList = true;
        closedList.push_back(current);

        if (current == goal)
        {
            ReconstructPath(goal);
            return 1;
        }
        ExploreNeighbours(current, goal);
    }
    std::cout << "no path found" << std::endl;
    return -1;
}

void MovementManager::ExploreNeighbours(BattlefieldCell* current, BattlefieldCell* goal)
{
    int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

    BattlefieldCell* bestNeighbour = nullptr;
    int bestFScore = std::numeric_limits<int>::max();

    for (int i = 0; i < 8; ++i)
    {
        int nx = current->x + dx[i];
        int ny = current->y + dy[i];
        int levelInt = GlobalConstants::quadTreeDepth;

        BattlefieldCell* neighbour = ghostGrid->getCell(nx, ny);

        if (neighbour != nullptr && !neighbour->impassableTerrain && !neighbour->inClosedList)
        {
            int gScore = current->gScore + ((i % 2 == 0) ? 10 : 14);
            int fScore = neighbour->hScore + gScore;
            neighbour->gScore = gScore;
            neighbour->fScore = fScore;

            if (neighbour->pathParent == nullptr || current->fScore < neighbour->pathParent->fScore)
            {
                neighbour->pathParent = current;
            }

            if (fScore < bestFScore) {
                bestNeighbour = neighbour;
                bestFScore = fScore;
            }
        }
    }
    if (bestNeighbour != nullptr) {
        openList.push(bestNeighbour);
    }
}

void MovementManager::ReconstructPath(BattlefieldCell* goal)
{
    BattlefieldCell* current = goal;
    while (current != nullptr)
    {
        pathList.push_back(current);
        current = current->pathParent;
    }

    std::reverse(pathList.begin(), pathList.end());
}


void MovementManager::cleanHeuristics()
{
    targetCell = nullptr;

    pathList.clear();
    closedList.clear();

    while (!openList.empty())
        openList.pop();

    ghostGrid->cleanHeuristics();
}