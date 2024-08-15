#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <vector>
#include <iostream>
#include <cstdint>
#include <array>

enum class BehaviorState : uint8_t

{
    Idle,
    Seek,
    Arrive
};

struct FormationData
{
    // Grouping 8-byte aligned members first
    std::chrono::steady_clock::time_point lastUpdateTime;  // 8 bytes, A8
    std::chrono::milliseconds updateInterval;              // 8 bytes, A8

    // Grouping other members with natural alignments next
    std::vector<uint8_t> pathToFollow;                 // 24 bytes, A8
    std::array<uint8_t, 256> vectorField;               // 256 bytes, A1

    // Grouping smaller members at the end to minimize padding
    bool isSelected;              // 1 byte, A1
    BehaviorState behaviorState;  // 1 byte, A1
    uint8_t id;                   // 1 byte, A1
    uint8_t direction;            // 1 byte, A1
    uint8_t radius;               // 1 byte, A1
    uint8_t speed;                // 1 byte, A1
    uint8_t posX;                 // 1 byte, A1
    uint8_t posY;                 // 1 byte, A1
    uint8_t vectorFieldWidth;     // 1 byte, A1
    uint8_t vectorFieldHeight;    // 1 byte, A1

    FormationData(
        bool isSelected = false,
        BehaviorState behaviorState = BehaviorState::Idle,
        uint8_t id = 0,
        uint8_t direction = 0,
        uint8_t radius = 0,
        uint8_t speed = 0,
        uint8_t posX = 0,
        uint8_t posY = 0,
        uint8_t vectorFieldWidth = 16,
        uint8_t vectorFieldHeight = 16,
        std::chrono::milliseconds updateInterval = std::chrono::milliseconds(100),
        const std::vector<uint8_t>& pathToFollow = {},
        const std::array<uint8_t, 256>& vectorField = {})
        :
        isSelected(isSelected),
        behaviorState(behaviorState),
        id(id),
        direction(direction),
        radius(radius),
        speed(speed),
        posX(posX),
        posY(posY),
        vectorFieldWidth(vectorFieldWidth),
        vectorFieldHeight(vectorFieldHeight),
        lastUpdateTime(std::chrono::steady_clock::now()),
        updateInterval(updateInterval),
        pathToFollow(pathToFollow),
        vectorField(vectorField)
    {
    }
};

class Formation
{
public:
    FormationData data;

    Formation() : data() {}

    Formation(const FormationData& formationData) : data(formationData) {}

    void Update();
    void repath();
    void identifyNextWaypoint();
    bool canPathCheck();
    void stepToNextWaypoint();
    void calculateDirection();
    void updateBehaviorState();
    void formVectorField();
    void setVectorfieldDirectionElement(uint8_t row, uint8_t col, uint8_t direction);
    const uint8_t getVectorfieldDirectionElement(uint8_t row, uint8_t col);
    void calculateRadius();
    bool collisionLineCheck();
    bool collisionRadiusCheck();
};