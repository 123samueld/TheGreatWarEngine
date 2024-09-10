#include "Agent.h"

Agent::Agent(float initialPosX, float initialPosY, int initialHealth, int initialArmour, float initialSpeed, int initialBallisticSkill, bool directional, bool friendlyUnit, std::string basicUnitType)
    : posX(initialPosX), posY(initialPosY), health(initialHealth), armour(initialArmour), speed(initialSpeed), ballisticSkill(initialBallisticSkill), directionalSprite(directional), friendly(friendlyUnit), unitType(basicUnitType) {
}


Agent::~Agent() {}

void Agent::takeDamage(int damage) {
    int damageTaken = damage - armour;
    if (damageTaken > 0) {
        health -= damageTaken;
        if (health < 0) {
            health = 0;
        }
    }
}

float Agent::getPosX() const {
    return posX;
}

float Agent::getPosY() const {
    return posY;
}

int Agent::getPosXIndex() const {
    return std::floor(posX);
}

int Agent::getPosYIndex() const {
    return std::floor(posY);
}

int Agent::getHealth() const {
    return health;
}

int Agent::getArmour() const {
    return armour;
}

float Agent::getSpeed() const {
    return speed;
}

int Agent::getBallisticSkill() const {
    return ballisticSkill;
}

bool Agent::isDirectionalSprite() const {
    return directionalSprite;
}

sf::Vector2i Agent::getCurrentDirection() const {
    return currentDirection;
}

std::string Agent::getUnitType() const
{
    return std::string(unitType);
}

void Agent::setHealth(int newHealth) {
    health = newHealth;
}

void Agent::setArmour(int newArmour) {
    armour = newArmour;
}

void Agent::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Agent::setBallisticSkill(int newBallisticSkill) {
    ballisticSkill = newBallisticSkill;
}

void Agent::setUnitType(std::string unit) {
    unitType = unit;
}