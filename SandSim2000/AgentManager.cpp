#include "AgentManager.h"

/*                        +++++++++++++ Agent Manager +++++++++++++                             */
/* Method: UpdateImpassableTerrainNodes - Sets selected nodes as impassable terrain             */
/* Method: PropagateWaveFrontHeuristics - Sets heuristics for nodes in a wave out from target   */
/* Method: AStar - Performs main loop of AStar on a grid with heuristics preset from WFP        */
/* Method: ExploreNeighbours - Neighbours of current cell passed to it                          */
/* Method: ReconstructPath - Builds list of path nodes based on parent nodes in path            */
/*                        +++++++++++++ Debug Methods +++++++++++++                             */
/* Method: PrintGhostGrid - Prints the ghost grid, has commented out code to print the hScore   */
/*             +++++++++++++     +++++++++++++        +++++++++++++                             */

void AgentManager::onUpdate(
    InputState& state,
    std::set<std::vector<BattlefieldCell>::iterator>* gameScene,
    GameStateManager& gameStateManager,
    Camera& camera,
    Scene& scene)
{

    if (pathfinderAgent != nullptr)
    {
        pathfinderAgent->update();


        std::vector<Agent*> agents = std::vector<Agent*>();
        gameStateManager.getState().quadTree->getAgentsInRadius(gameStateManager.getState().quadTree, pathfinderAgent->getPosXIndex() * GlobalConstants::cellSize, pathfinderAgent->getPosYIndex() * GlobalConstants::cellSize, GlobalConstants::cellSize * 2, GlobalConstants::quadTreeDepth, &agents);
    }
    if (mobileAgent.size() != 0)
    {
        for(int i = 0; i < mobileAgent.size(); i++)
            mobileAgent[i]->update(&gameStateManager);
    }

    if (state.isLeftMouseButtonPressed && leftClick == false && pathfinderAgent != nullptr)
    {
        GameState& gamestate = gameStateManager.getState();
        BattlefieldCell* targetCell = gameStateManager.getCell(state.selectedCell.x, state.selectedCell.y);
        
        movementManager.SetUnitPath(pathfinderAgent, targetCell, &gameStateManager, state, scene, &camera);

        leftClick = true;
    }
    else if (state.isRightMouseButtonPressed && rightClick == false)
    {
        rightClick = true;
    }
    else if (state.isLeftMouseButtonPressed == false)
    {
        leftClick = false;
    }
    else if (state.isRightMouseButtonPressed == false)
    {
        rightClick = false;
    }
}

void AgentManager::placeScenery(sf::Vector2i isometricCell, std::set<std::vector<BattlefieldCell>::iterator>* gameScene, Scenery sceneObject, GameStateManager& gameStateManager)
{
    Scenery* tree = new Scenery(isometricCell.x, isometricCell.y, sceneObject.getSpriteString());

    gameStateManager.getState().Units.push_back(tree);

    gameStateManager.getState().quadTree->insert(tree, constants.cellSize);

    BattlefieldCell* cell = gameStateManager.getCell(isometricCell.x, isometricCell.y);

    if (cell != nullptr)
        cell->impassableTerrain = true;
}

void AgentManager::placeAgent(sf::Vector2i cell, std::set<std::vector<BattlefieldCell>::iterator>* gameScene, Agent agent, GameStateManager& gameStateManager)
{
    Agent* newAgent = new Agent(agent);

    gameStateManager.getState().Units.push_back(newAgent);
    gameStateManager.getState().quadTree->insert(newAgent, constants.cellSize);
}

void AgentManager::placeMobileAgent(sf::Vector2i cell, std::set<std::vector<BattlefieldCell>::iterator>* gameScene, MobileAgent agent, GameStateManager& gameStateManager)
{
    MobileAgent* newAgent = new MobileAgent(agent);

    gameStateManager.getState().Units.push_back(newAgent);
    gameStateManager.getState().quadTree->insert(newAgent, constants.cellSize);

    mobileAgent.push_back(newAgent);
}

void AgentManager::placePathfinderAgent(sf::Vector2i cell, std::set<std::vector<BattlefieldCell>::iterator>* gamesScene, PathfinderAgent agent, GameStateManager& gameStateManager)
{
    BattlefieldCell* currentCell = gameStateManager.getCell(cell.x, cell.y);

    PathfinderAgent* newAgent = new PathfinderAgent(agent);
    newAgent->setStartingCell(gameStateManager.getState().quadTree->getCell(gameStateManager.getState().quadTree, cell.x * constants.cellSize, cell.y * constants.cellSize, constants.quadTreeDepth));


    gameStateManager.getState().Units.push_back(newAgent);
    gameStateManager.getState().quadTree->insert(newAgent, constants.cellSize);

    pathfinderAgent = newAgent;
    pathfinderAgent->current = currentCell;
}

void AgentManager::loadAgentsFromMap(const char* filepath, std::set<std::vector<BattlefieldCell>::iterator>* gameScene, GameStateManager& gameStateManager)
{
    std::ifstream mapDataFile(filepath);

    if (!mapDataFile.is_open())
    {
        std::cerr << "missing filepath: " << filepath << std::endl;
        return;
    }
    nlohmann::json mapData;
    mapDataFile >> mapData;
    mapDataFile.close();

    for (int i = 0; i < mapData["SceneryData"].size(); i++)
    {
        sf::Vector2i position = sf::Vector2i(mapData["SceneryData"][i]["position"][0], mapData["SceneryData"][i]["position"][1]);
        placeScenery(position, gameScene, Scenery(position.x, position.y, mapData["SceneryData"][i]["AgentType"]), gameStateManager);
    }
}