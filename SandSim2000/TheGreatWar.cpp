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

    agentManager.placePathfinderAgent(sf::Vector2i(2, 2), &scene.gameScene, PathfinderAgent(2, 2, "PathfinderAgent"), gameStateManager);

    int numberOfAgents = 4;
    for(int i = 0; i < 5; i++)
        agentManager.placeMobileAgent(sf::Vector2i(2, 2), &scene.gameScene, MobileAgent(2 , 2 - numberOfAgents / 2 + i, 1, 1, 0.1f, 1, "RedBaron"), gameStateManager);

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
        sf::Clock timer;
        timer.restart();
        bool output = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

        agentManager.onUpdate(inputState, &scene.gameScene, gameStateManager, camera, scene);
        if (output)
        {
            std::cout << timer.getElapsedTime().asMicroseconds() << " agentManager::onUpdate\n";
            timer.restart();
        }

        if (!camera.Update(inputState))
            break;
        if (output)
        {
            std::cout << timer.getElapsedTime().asMicroseconds() << " camera::Update\n";
            timer.restart();
        }




        

        scene.UpdateGameScene(camera, gameStateManager.getState(), inputState);
        if(output)
        {
            std::cout << timer.getElapsedTime().asMicroseconds() << " SCENE::UPDATEGAMESCENE\n";
            timer.restart();
        }

        std::vector<sf::Sprite> spriteMap = scene.buildGameScene(&animationManager);
        if(output)
        {
            std::cout << timer.getElapsedTime().asMicroseconds() << " SCENE::BUILDGAMESCENE\n";
            timer.restart();
        }


        camera.Draw(spriteMap, inputState);
        if(output)
        {
            std::cout << timer.getElapsedTime().asMicroseconds() << " CAMERA::DRAW\n";
            timer.restart();

            std::cout << "------------------------------------------------\n";
        }
    }
    return 0;
}