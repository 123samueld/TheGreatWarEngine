#include "Formation.h"

void Formation::Update()
{
    // Formations don't need to update every iteration of the main loop so we can reduce their cost
    // by throttling based on time.
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - data.lastUpdateTime);

    if (elapsed >= data.updateInterval)
    {
        // Call other methods from here.

        // Update the last update time
        data.lastUpdateTime = now;
    }
}

void Formation::identifyNextWaypoint()
{
    // Identify the next waypoint based on speed, time and an open path. Run canPathCheck() and if needed repath()
    // from here as part of identifying the next waypoint.
    // Faster Formations will take bigger steps over the same amount of time.
}

bool Formation::canPathCheck()
{
    // Check the next few cells in pathToFollow to see that they're all open, if not call repath().
    // Don't check all cells in pathToFollow, just the next few, say a radius worth (10 for now)
    return true;
}

void Formation::repath()
{
    // Call pathfinding again
}

void Formation::stepToNextWaypoint()
{
    // Update the Formations position to the next waypoint and call other needed methods like formVectorField().
}

void Formation::updateBehaviorState()
{
    // The default behavior state is Idle and for now isSelected will default to true.
    // Right clicking the map will switch it's behavior state to Seek which will be
    // the default state to follow along the waypoints, it will only seek a stationary target at the moment, 
    // as opposed to Persue.
    // As the Formation approaches it's target the Behavior State will switch to Arrive, 
    // this will fix the Agent "noise" problem from MkI Path Following.
    // 
    // Behavior States are a really important tool which will allow us to add new behavior types in a modular way.
    // 
    // Formations will have a Behavior State, so will individual Agents, more on them later.  
}

void Formation::calculateDirection()
{
    // Use the current position and the position of the next way point to determine the direction the 
    // Formation is pointing. I think we only need the 8 cardinal directions so just make it accurate up to that 
    // resolution.
}

void Formation::formVectorField()
{
    // This is probably gonna be the most complex method of this task
    // and don't bother with this now, I'm going to split this off and make a mini task of it.

    // When path finding is run it uses wave front propagation to set the heuristic for A* giving 100% accuracy.
    // Right now the wave and the values it sets are deleted. But it would now be good to keep them because
    // we can reuse them for the first half of creating a vector field.
    // The second part of the vector field is to check neighbour cells then create
    // a vector (aka direction to point in) based on the direction of the lowest value neighbour.

    // Because the path finding automatically removes obstacles early, we won't need to worry about specific
    // obstacle avoidance. Even if there's a tree right in the middle of the vector field, Boids will naturally follow
    // the vectors which won't directly be over obstacles.

    // As the Formation steps from one waypoint to the next, some new vectors will need to be updated
    // at the front of the formation, and some old ones will need deleted at the back while
    // all the vectors in the middle will need shifted backward. This will mean only a certain amout of
    // new vectors need to be calculated at each new waypoint step.

    // The VectorField is very similar to the GhostGrid except that it updates every waypoint step,
    // and not all cells are added only new ones. The int direction property will be used to calculate which
    // set of BattlefieldCells will be the next ones added to the VectorField.

    // Use setVectorfieldDirectionElement() here.
}

void Formation::setVectorfieldDirectionElement(uint8_t row, uint8_t col, uint8_t direction)
{
    int index = row * data.vectorFieldWidth + col;
    if (index < data.vectorField.size())
    {
        data.vectorField[index] = direction;
    }
}

const uint8_t Formation::getVectorfieldDirectionElement(uint8_t row, uint8_t col)
{
    int index = row * data.vectorFieldWidth + col;
    if (index < data.vectorField.size())
    {
        return data.vectorField[index];
    }
    // Handle out-of-bounds access as needed (e.g., throw an exception, return a default value, etc.)
    return 0;  // Default value or error handling
}