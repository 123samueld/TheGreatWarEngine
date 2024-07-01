#include "MobileAgent.h"

void MobileAgent::update(GameStateManager* gameStateManager)
{

	nearbyAgents = std::vector<Agent*>();

	gameStateManager->state.quadTree->getAgentsInRadius(gameStateManager->state.quadTree, getPosX() * constants.cellSize, getPosY() * constants.cellSize, constants.cellSize * 2, constants.quadTreeDepth, &nearbyAgents);
	
	if(pathfinderAgent == nullptr)
		pathfinderAgent = getPathfinderFromList();
	float dX = pathfinderAgent->getPosX() - getPosX();
	float dY = pathfinderAgent->getPosY() - getPosY();
	float length = sqrt((dX * dX) + (dY * dY));
	if (abs(length) > 0.1f)
	{
		Coherence();

		ClampVelocity();

		posX += velocity.x; posY += velocity.y;
		updateCurrentSpriteDirection();
		updateCell(gameStateManager);
	}

}

PathfinderAgent* MobileAgent::getPathfinderFromList()
{
	for (int i = 0; i < nearbyAgents.size(); i++)
	{
		if (nearbyAgents[i]->isPathfinderAgent)
			return dynamic_cast<PathfinderAgent*>(nearbyAgents[i]);
	}
	return nullptr;
}

void MobileAgent::Coherence()
{
	velocity.x = (pathfinderAgent->getPosX() - getPosX()) * 0.1f;
	velocity.y = (pathfinderAgent->getPosY() - getPosY()) * 0.1f;
}

void MobileAgent::ClampVelocity()
{
	velocity = normalize(velocity);
	velocity.x *= speed; velocity.y *= speed;
}

void MobileAgent::updateCurrentSpriteDirection()
{
	float spriteBarrier = speed * 0.5f;

	if (velocity.x > spriteBarrier) currentDirection.x = 1;
	if (velocity.x < -spriteBarrier) currentDirection.x = -1;

	if (velocity.y > spriteBarrier) currentDirection.y = 1;
	if (velocity.y < -spriteBarrier) currentDirection.y = -1;

	if (velocity.x > -spriteBarrier && velocity.x < spriteBarrier) { velocity.x = 0; }
	if (velocity.y > -spriteBarrier && velocity.y < spriteBarrier) { velocity.y = 0; }

	if (velocity.x == 0 && velocity.y != 0) currentDirection.x = 0;
	if (velocity.x != 0 && velocity.y == 0) currentDirection.y = 0;
}

void MobileAgent::updateCell(GameStateManager* gameStateManager)
{
	if (lastCellPosition != sf::Vector2i(getPosXIndex(), getPosYIndex()))
	{
		BattlefieldCell* previousCell = gameStateManager->getCell(lastCellPosition.x, lastCellPosition.y);
		BattlefieldCell* nextCell = gameStateManager->getCell(getPosXIndex(), getPosYIndex());

		if(previousCell != nullptr && nextCell != nullptr)
		{
			previousCell->removeObject(this);
			nextCell->addObject(this);
		}
	}
	lastCellPosition = sf::Vector2i(getPosXIndex(), getPosYIndex());
}

sf::Vector2f MobileAgent::normalize(sf::Vector2f value)
{
	float length = sqrt((value.x * value.x) + (value.y * value.y));
	if (length != 0)
		return sf::Vector2f(value.x / length, value.y / length);
	else
		return value;
}