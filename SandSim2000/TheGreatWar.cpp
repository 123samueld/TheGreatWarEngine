#include "TheGreatWar.h"

int main()
{
    MenuOption selectedOption = displayMenu();
    if (selectedOption == MenuOption::Exit)
        return 0;
    else if (selectedOption == MenuOption::MapEditor)
        std::cout << "Map Editor selected\n";

    SpriteManager sprites = SpriteManager();
    AnimationManager animationManager;
    unsigned int mapSize = 16;
    GameStateManager gameStateManager = GameStateManager(mapSize * mapSize);
    AgentManager agentManager;
    InputState inputState;
    Camera camera;
    Scene scene;

    /*
    agentManager.placePathfinderAgent(sf::Vector2i(8, 8), &scene.gameScene, PathfinderAgent(8, 8, "PathfinderAgent"), gameStateManager);
    agentManager.placeMobileAgent(sf::Vector2i(8, 8), &scene.gameScene, MobileAgent(8, 8, 1, 1, 0.1f, 1, "RedBaron"), gameStateManager);

    agentManager.placeScenery(sf::Vector2i(7, 8), &scene.gameScene, Tree(7, 8), gameStateManager);
    agentManager.placeScenery(sf::Vector2i(9, 8), &scene.gameScene, Tree(9, 8), gameStateManager);
    agentManager.placeScenery(sf::Vector2i(8, 7), &scene.gameScene, Tree(8, 7), gameStateManager);    
    agentManager.placeScenery(sf::Vector2i(8, 9), &scene.gameScene, Tree(8, 9), gameStateManager);    
    */

    while (camera.window.isOpen())
    {
        inputState = InputStateManager::getInstance().updateInputState(camera.window, inputState);

        sf::Event event;
        while (camera.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                camera.window.close();
            }
        }
        if(inputState.isTPressed)
            agentManager.placeScenery(inputState.selectedCell, &scene.gameScene, Tree(inputState.selectedCell.x, inputState.selectedCell.y), gameStateManager);

        agentManager.onUpdate(inputState, &scene.gameScene, gameStateManager, camera, scene);

        if (!camera.Update(inputState))
            break;
        scene.UpdateGameScene(camera, gameStateManager.getState(), inputState);
        camera.Draw(scene.buildGameScene(&animationManager), inputState);
    }
    return 0;
}

// Test push to remote repo yet again.