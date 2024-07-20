#pragma once

#include <vector>
#include <cmath>

#include "SFML/System.hpp"

#include "GlobalConstants.h"
#include "Agent.h"
#include "GameStateManager.h"
#include "PathfinderAgent.h"

class MobileAgent : public Agent
{
public:
	MobileAgent(int initialPosX, int initialPosY, int initialHealth, int initialArmour, float initialSpeed, int initialBallisticSkill, sf::String basicUnitType) : Agent(initialPosX, initialPosY, initialHealth, initialArmour, initialSpeed, initialBallisticSkill, true, basicUnitType) { lastCellPosition = sf::Vector2i(initialPosX, initialPosY); isMobileAgent = true; }

	void update(GameStateManager* gameStateManager);
private:
	std::vector<Agent*> nearbyAgents;
	sf::Vector2f velocity;
	PathfinderAgent* pathfinderAgent = nullptr;

	void Coherence();
	void seperation();
	void Alignment();

	void ClampVelocity();

	void updateCurrentSpriteDirection();
	void updateCell(GameStateManager* gameStateManager);

	sf::Vector2f normalize(sf::Vector2f value);
	PathfinderAgent* getPathfinderFromList();

	sf::Vector2i lastCellPosition;

	float rangeCheck = 4;
	float tooCloseToAgent = 1.4f;
	float tooCloseToScenery = 0.9f;

	float avoidFactor = 0.07f;
	float coherenceFactor = 0.04f;
	float alighnmentFactor = 0.05f;
};

