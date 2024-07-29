#pragma once

#include "SFML/Graphics/Rect.hpp"

#include "GlobalConstants.h"

#include <array>
#include <vector>
#include <iostream>

struct QuadTree {
    virtual void f() {};

    unsigned int depth;

    sf::IntRect quadRect;
    std::array<QuadTree*, 4> children;

    int offsetX[4] = { 0, 1, 0, 1 };
    int offsetY[4] = { 0, 0, 1, 1 };

    virtual void insert(Agent* agent, int multiplier)
    {
        for (int i = 0; i < 4; i++)
        {
            if (children[i] != nullptr)
            {
                if (children[i]->quadRect.contains(sf::Vector2i(agent->getPosX() * multiplier, agent->getPosY() * multiplier)))
                {
                    children[i]->insert(agent, multiplier);
                    return;
                }
            }
        }
    }

    QuadTree* getNode(QuadTree* node, int targetX, int targetY, int targetLevel) {
        if (node == nullptr) return nullptr;

        if (node->quadRect.getPosition().x == targetX && node->quadRect.getPosition().y == targetY && node->depth == targetLevel) return node;

        for (int i = 0; i < 4; ++i) {
            int childX = node->quadRect.getPosition().x * 2 + offsetX[i];
            int childY = node->quadRect.getPosition().y * 2 + offsetY[i];
            QuadTree* result = getNode(node->children[i], targetX, targetY, targetLevel);
            if (result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }

    virtual BattlefieldCell* getCell(QuadTree* node, int targetX, int targetY, int targetLevel)
    {
        if (node == nullptr) return nullptr;

        for (int i = 0; i < 4; ++i) {
            int childX = node->quadRect.getPosition().x * 2 + offsetX[i];
            int childY = node->quadRect.getPosition().y * 2 + offsetY[i];
            BattlefieldCell* result = node->children[i]->getCell(node->children[i], targetX, targetY, targetLevel);
            if (result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }

    bool CircleInRect(float circleX, float circleY, float radius, QuadTree* node)
    {
        float testX = circleX; float testY = circleY;

        if (circleX < node->quadRect.getPosition().x)           testX = node->quadRect.getPosition().x;
        else if (circleX < node->quadRect.getPosition().x)      testX = node->quadRect.getPosition().x + node->quadRect.getSize().x;
        if (circleY < node->quadRect.getPosition().y)           testY = node->quadRect.getPosition().y;
        else if (circleY < node->quadRect.getPosition().y)      testY = node->quadRect.getPosition().y + node->quadRect.getSize().y;

        float distanceX = circleX - testX;
        float distanceY = circleY - testY;

        float distance = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));
        if (distance <= radius)
            return true;
        return false;
    }

    virtual void getAgentsInRadius(QuadTree* node, float targetX, float targetY, float radius, int targetLevel, std::vector<Agent*>* agents)
    {
        if (node == nullptr) return;

        for (int i = 0; i < 4; ++i)
        {
            bool ishit = CircleInRect(targetX, targetY, radius, node->children[i]);
            if (ishit)
                node->children[i]->getAgentsInRadius(node->children[i], targetX, targetY, radius, targetLevel, agents);
        }
    }

    QuadTree(const sf::IntRect& rect, const unsigned int& depth)
        : depth(depth), quadRect(rect), children{ nullptr, nullptr, nullptr, nullptr } {}

    ~QuadTree() {
        for (QuadTree* child : children)
            delete[] child;
    }
};

struct QuadTreeLeaf : public QuadTree {
    std::vector<BattlefieldCell>::iterator iter;

    void insert(Agent* agent, int multiplier) override
    {
        iter->Objects.push_back(agent);
    }

    BattlefieldCell* getCell(QuadTree* node, int targetX, int targetY, int targetLevel) override
    {
        if (node->quadRect.getPosition().x == targetX && node->quadRect.getPosition().y == targetY && node->depth == targetLevel + 1) return iter._Unwrapped();

        return nullptr;
    }

    void getAgentsInRadius(QuadTree* node, float targetX, float targetY, float radius, int targetLevel, std::vector<Agent*>* agents)
    {
        if (node == nullptr) return;

        for (int i = 0; i < iter->Objects.size(); i++)
        {
            float differenceX = abs(targetX - iter->Objects[i]->getPosX() * GlobalConstants::cellSize);
            float differenceY = abs(targetY - iter->Objects[i]->getPosY() * GlobalConstants::cellSize);
            float distance = std::sqrt((differenceX * differenceX) + (differenceY * differenceY));

            if(distance <= radius)
                agents->push_back(iter->Objects[i]);
        }
    }

    QuadTreeLeaf(const sf::IntRect& rect, const unsigned int& depth, std::vector<BattlefieldCell>::iterator& _iter)
        : QuadTree(rect, depth), iter(_iter) {}

private:
    sf::IntRect calculateChildRect(int index) const
    {
        int halfWidth = quadRect.width / 2;
        int halfHeight = quadRect.height / 2;
        int childLeft = quadRect.left + (index % 2) * halfWidth;
        int childTop = quadRect.top + (index / 2) * halfHeight;
        return sf::IntRect(childLeft, childTop, halfWidth, halfHeight);
    }
};