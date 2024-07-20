#include "MobileAgent.h"

void MobileAgent::update(GameStateManager* gameStateManager)
{

	nearbyAgents = std::vector<Agent*>();

	gameStateManager->state.quadTree->getAgentsInRadius(gameStateManager->state.quadTree, getPosX() * GlobalConstants::cellSize, getPosY() * GlobalConstants::cellSize, GlobalConstants::cellSize * 2, GlobalConstants::quadTreeDepth, &nearbyAgents);
	
	if(pathfinderAgent == nullptr)
		pathfinderAgent = getPathfinderFromList();
	float dX = pathfinderAgent->getPosX() - getPosX();
	float dY = pathfinderAgent->getPosY() - getPosY();
	float length = sqrt((dX * dX) + (dY * dY));
	if (abs(length) > 0.1f)
	{
		Coherence();
		seperation();
		Alignment();

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
	velocity.x = (pathfinderAgent->getPosX() - getPosX()) * coherenceFactor;
	velocity.y = (pathfinderAgent->getPosY() - getPosY()) * coherenceFactor;
}

void MobileAgent::seperation()
{
	float close_dx = 0; float close_dy = 0;

	for (int i = 0; i < nearbyAgents.size(); i++)
	{
		float dX = nearbyAgents[i]->getPosX() - getPosX();
		float dY = nearbyAgents[i]->getPosY() - getPosY();
		float length = sqrt((dX * dX) + (dY * dY));

		if (nearbyAgents[i]->isMobileAgent && length <= tooCloseToAgent && nearbyAgents[i]->isPathfinderAgent == false)
		{
			close_dx += getPosX() - nearbyAgents[i]->getPosX();
			close_dy += getPosY() - nearbyAgents[i]->getPosY();
		}
		else if (nearbyAgents[i]->isMobileAgent == false && length <= tooCloseToScenery && nearbyAgents[i]->isPathfinderAgent == false)
		{
			close_dx += getPosX() - nearbyAgents[i]->getPosX();
			close_dy += getPosY() - nearbyAgents[i]->getPosY();
		}
	}

	velocity.x += close_dx * avoidFactor;
	velocity.y += close_dy * avoidFactor;
}

void MobileAgent::Alignment()
{
	float xvel_avg = 0; float yvel_avg = 0;
	int boidCount = 0;
	for (Agent* agent : nearbyAgents)
	{
		if (agent->isMobileAgent)
		{
			boidCount += 1;
			MobileAgent* otherAgent = dynamic_cast<MobileAgent*>(agent);

			xvel_avg += otherAgent->velocity.x;
			yvel_avg += otherAgent->velocity.y;
		}
	}
	if (boidCount > 0)
	{
		xvel_avg = xvel_avg / boidCount;
		yvel_avg = yvel_avg / boidCount;

		velocity.x += (xvel_avg - velocity.x) * alighnmentFactor;
		velocity.y += (yvel_avg - velocity.y) * alighnmentFactor;
	}
}

void MobileAgent::ClampVelocity()
{
	velocity = normalize(velocity);
	velocity.x *= speed; velocity.y *= speed;
}

void MobileAgent::updateCurrentSpriteDirection()
{
	float spriteBarrier = speed * 0.77f;

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