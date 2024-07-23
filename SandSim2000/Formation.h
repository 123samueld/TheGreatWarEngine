#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class BehaviorState
{
    Idle,
    Seek,
    // Persue
    // Arrive
};

class Formation
{
   public:
    int id;
    bool isSelected;
    sf::Vector2f position;
    int direction;
    BehaviorState behaviorState;
    float radius;
    float speed;
    std::vector<std::vector<sf::Vector2f>> vectorField;
    std::vector<sf::Vector2f> pathToFollow;

    void Update();
    bool canPathCheck();
    void repath();
    void calculateRadius();
    void stepToNextWaypoint();
    void formVectorField();
    // calculateDirection();
    void updateBehaviorState();



    // Properties & Methods for Later
    int numberOfBoids;
    int sizeOfLargestBoid;
    float maximumSpeedOfSlowestBoid;

    bool collisionLineCheck();
    bool collisionRadiusCheck();
};