#pragma once

#include "Agent.h"

class Scenery : public Agent {
public:
    Scenery(int initialPosX, int initialPosY, std::string sceneryType) :Agent(initialPosX, initialPosY, -1, -1, -1, -1, false, false, sceneryType) {};

private:


};
