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
    GridGenerator gridGenerator;
    Scene scene(gridGenerator); 

    agentManager.loadAgentsFromMap(currentMap, &scene.gameScene, gameStateManager);

    // Add this new method.
    // agentManager.initialiseFormations(args);
    
    
    // Obsolete MkI methods.
    /*
    agentManager.placePathfinderAgent(sf::Vector2i(2, 2), &scene.gameScene, PathfinderAgent(2, 2, "PathfinderAgent"), gameStateManager);
    int numberOfAgents = 400;
    for(int i = 0; i < numberOfAgents; i++)
        agentManager.placeMobileAgent(sf::Vector2i(2, 2), &scene.gameScene, MobileAgent(2 , 2, 1, 1, 0.1f, 1, "RedBaron"), gameStateManager);
    */

    while (camera.window.isOpen())
    {
        inputState = InputStateManager::getInstance().updateInputState(camera.window, inputState);

        sf::Event event;
        while (camera.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                camera.window.close();
            }
        }

        agentManager.onUpdate(inputState, &scene.gameScene, gameStateManager, camera, scene);
        if (!camera.Update(inputState))
            break;
        scene.UpdateGameScene(camera, gameStateManager.getState(), inputState);
        std::vector<DrawableSprite> spriteMap = scene.buildGameScene(&animationManager);
        camera.Draw(spriteMap, inputState);
    }
    return 0;
}