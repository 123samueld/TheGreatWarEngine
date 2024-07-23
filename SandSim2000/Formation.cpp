#include "Formation.h"

void Formation::Update()
{
    // All methods should ideally be run through here so we have 1 single point of control if we need to 
    // throttle or whatever.
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

void Formation::calculateRadius()
{
    // Just use a placeholder for now, this will get more complex once we've got multiple Boids in a formation.
    radius = 10.0f;
}

void Formation::stepToNextWaypoint()
{
    // Update the position to the next waypoint and call other needed methods like formVectorField().
    // Position update is based on speed and next waypoint.
}

void Formation::formVectorField()
{
    // This is probably gonna be the most complex method of this task 
    // and we might need to come back to it a few times, maybe. 
    
    // When path finding is run it uses wave front propagation to set the heuristic for A* giving 100% accuracy.
    // Right now the wave and the values it sets are deleted. But it would now be good to keep them because 
    // we can reuse them for the first half of creating a vector field.
    // The second part of the vector field is to check neighbour cells then create 
    // a vector (aka direction to point in) based on the direction of the lowest value neighbour. 

    // Because the path finding automatically removes obstacles early, we won't need to worry about specific
    // obstacle avoidance. Even if there's a tree right in the middle of the vector field, Boids will naturally follow the 
    // vectors which won't directly be over obstacles. 

    // As the Formation steps from one waypoint to the next, some new vectors will need to be updated
    // at the front of the formation, and some old ones will need deleted at the back while
    // all the vectors in the middle will need shifted backward. This will mean only a certain amout of 
    // new vectors need to be calculated at each new waypoint step.
}

void Formation::updateBehaviorState() 
{
    // The default behavior state is Idle. 
    // Now would be a good time to implement click selection, that should probably be a sub-task I'll get onto soon.
    // Once a Formation has been selected, right clicking the map will switch it's behavior state to Seek which will be
    // the default state to follow along the waypoints, it will only seek a stationary target, as opposed to Persue.
    // As the Formation approaches it's target the Behavior State will switch to Arrive, 
    // this will fix the Agent "noise" problem from MkI Path Following.
    // 
    // Behavior States are a really important tool which will allow us to add new behavior types in a modular way.
    // 
    // Formations will have a Behavior State as also will Agents, more on them later.  
}