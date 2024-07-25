#pragma once
#include <cmath>
#include <set>
#include <typeinfo>
#include <unordered_set>
#include <queue>
#include <functional>

#include "GlobalConstants.h"
#include "GameStateManager.h"
#include "InputState.h"
#include "Scenery.h"
#include "Tree.h"
#include "Camera.h"
#include "Scene.h"
#include "PathfinderAgent.h"
#include "MobileAgent.h"
#include "MovementManager.h"
#include "Formation.h"

class AgentManager
{
public:
	void onUpdate(
		InputState& state,
		std::set<std::vector<BattlefieldCell>::iterator>* gameScene,
		GameStateManager& gameStateManager,
		Camera& camera,
		Scene& scene);

	void placeScenery(sf::Vector2i isometricCell, std::set<std::vector<BattlefieldCell>::iterator>* gameScene, Scenery scenObject, GameStateManager& gameStateManager);
	void placeAgent(sf::Vector2i cell, std::set<std::vector<BattlefieldCell>::iterator>* gameScene, Agent agent, GameStateManager& gameStateManager);
	//void placeMobileAgent(sf::Vector2i cell, std::set<std::vector<BattlefieldCell>::iterator>* gameScene, MobileAgent agent, GameStateManager& gameStateManager);
	//void placePathfinderAgent(sf::Vector2i cell, std::set<std::vector<BattlefieldCell>::iterator>* gamesScene, PathfinderAgent agent, GameStateManager& gameStateManager);
    void initialiseFormations(sf::Vector2i cell, std::set<std::vector<BattlefieldCell>::iterator>* gamesScene, GameStateManager& gameStateManager, Formation formation);


	PathfinderAgent* pathfinderAgent = nullptr;
	MobileAgent* mobileAgent = nullptr;
private:
	bool leftClick = false;
	bool rightClick = false;

	MovementManager movementManager;
	GlobalConstants constants;
};
