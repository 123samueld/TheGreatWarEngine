#pragma once
#include "Scenery.h"
class Tree : public Scenery
{
public:
    Tree(int initialPosX,int initialPosY):Scenery(initialPosX,initialPosY,"Tree"){}
};