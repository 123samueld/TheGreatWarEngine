#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

#include "Agent.h"


enum class BehaviorState
{
    Idle,
    Seek,
    Arrive

    // Persue
    // 
    // Patrol
};

class Formation
{
   public:
    int id;
    bool isSelected; // For now there will only be 1 formation so initialise it to selected
    sf::Vector2f position;
    int direction;
    BehaviorState behaviorState;
    float radius;
    float speed;
    std::vector<std::vector<sf::Vector2f>> vectorField;
    std::vector<sf::Vector2f> pathToFollow;
    // Timer
    std::chrono::steady_clock::time_point lastUpdateTime;
    std::chrono::milliseconds updateInterval;


    // Constructor
    Formation(
        int id, 
        bool isSelected, 
        sf::Vector2f position, 
        int direction, 
        BehaviorState behaviorState, 
        float radius,
        float speed, 
        const std::vector<std::vector<sf::Vector2f>>& vectorField,
        const std::vector<sf::Vector2f>& pathToFollow, 
        int numberOfBoids, 
        int sizeOfLargestBoid,
        float maximumSpeedOfSlowestBoid, 
        std::chrono::milliseconds interval = std::chrono::milliseconds(100) 
    ): 
        id(id),
        isSelected(isSelected),
        position(position),
        direction(direction),
        behaviorState(behaviorState),
        radius(radius),
        speed(speed),
        vectorField(vectorField),
        pathToFollow(pathToFollow),
        // numberOfBoids(numberOfBoids),
        // sizeOfLargestBoid(sizeOfLargestBoid),
        // maximumSpeedOfSlowestBoid(maximumSpeedOfSlowestBoid)
          updateInterval(interval)
        {
            lastUpdateTime = std::chrono::steady_clock::now();
        }


    void Update();
    bool canPathCheck();
    void repath();
    void identifyNextWaypoint();
    void stepToNextWaypoint();

    void calculateDirection();
    void updateBehaviorState();



    // Properties & Methods for Later
    //int numberOfBoids;
    //int sizeOfLargestBoid;
    //float maximumSpeedOfSlowestBoid;

    void formVectorField();
    void calculateRadius();
    bool collisionLineCheck();
    bool collisionRadiusCheck();
};