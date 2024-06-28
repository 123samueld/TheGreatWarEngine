#include "PathfinderAgent.h"

#include <iostream>

void PathfinderAgent::setPath(std::vector<BattlefieldCell*>* TargetPath)
{
	path = std::vector<BattlefieldCell*>();
	if(TargetPath->size() > 1)
	{
		for (int i = 1; i < TargetPath->size(); i++)
		{
			path.push_back(TargetPath->at(i));
		}
		atDestination = false;
	}
}

void PathfinderAgent::update()
{
	if (atDestination == false && clock.getElapsedTime().asSeconds() > 1 / movesPerSecond)
	{
		BattlefieldCell* nextCell = path[0];

		currentDirection = sf::Vector2i(nextCell->x - posX, nextCell->y - posY);
		posX = nextCell->x; posY = nextCell->y;

		if (current != nullptr) 
		{
			current->removeObject(this);
		}


		path.erase(path.begin());

		current = nextCell;
		current->addObject(this);

		if (path.size() == 0)
			atDestination = true;

		clock.restart();
	}
}