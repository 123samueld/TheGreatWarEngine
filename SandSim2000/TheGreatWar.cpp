#include "TheGreatWar.h"

int main()
{
    MenuOption selectedOption = displayMenu();
    if (selectedOption == MenuOption::Exit)
        return 0;
    else if (selectedOption == MenuOption::MapEditor)
        std::cout << "Map Editor selected\n";

    const char* currentMap = "../resources/maps/map.json";

    SpriteManager sprites = SpriteManager();
    AnimationManager animationManager;
    GameStateManager gameStateManager = GameStateManager(currentMap);
    AgentManager agentManager;
    InputState inputState;
    Camera camera;
    Scene scene;

    agentManager.loadAgentsFromMap(currentMap, &scene.gameScene, gameStateManager);

    agentManager.placePathfinderAgent(sf::Vector2i(2, 2), &scene.gameScene, PathfinderAgent(2, 2, "PathfinderAgent"), gameStateManager);

    for(int i = 0; i < 10; i++)
        agentManager.placeMobileAgent(sf::Vector2i(2, 2), &scene.gameScene, MobileAgent(2, 2, 1, 1, 0.1f, 1, "RedBaron"), gameStateManager);

    sf::Clock timer;
    int count = 0;
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

        count += 1;
        if (timer.getElapsedTime().asSeconds() > 1)
        {
            std::cout << count << " frames per second\n";
            timer.restart();
            count = 0;
        }
    }
    return 0;
}