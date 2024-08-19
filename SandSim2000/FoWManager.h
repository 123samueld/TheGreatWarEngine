#pragma once

#include <vector>

#include "GameStateManager.h"
#include "Agent.h"

class FoWManager
{
public:
	FoWManager() {}
	~FoWManager() {}

	void UpdateFogOfWarState(GameStateManager& gamestateManager);
	void ResetFogOfWarState(GameStateManager& gamestateManager);
};