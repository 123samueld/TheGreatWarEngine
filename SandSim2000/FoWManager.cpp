#include "FoWManager.h"

#include "PathfinderUtilities.h"

void FoWManager::UpdateFogOfWarState(GameStateManager& gamestateManager)
{
	//ResetFogOfWarState(gamestateManager);

	for (Agent* a : gamestateManager.getState().Units)
	{
		if(a->friendly)
		{
			for (int x = max(0, a->getPosXIndex() - a->revealRange); x < min(gamestateManager.battlefieldMap.size, a->getPosXIndex() + a->revealRange); x++)
			{
				for (int y = max(0, a->getPosYIndex() - a->revealRange); y < min(gamestateManager.battlefieldMap.size, a->getPosYIndex() + a->revealRange); y++)
				{
					if (abs(a->getPosXIndex() - x) + abs(a->getPosYIndex() - y) <= a->revealRange)
						gamestateManager.getCell(x, y)->FoW = true;
				}
			}
		}
	}
}

void FoWManager::ResetFogOfWarState(GameStateManager& gamestateManager)
{
	for (BattlefieldCell& cell : gamestateManager.state.BattlefieldVector)
		cell.FoW = false;
}
 