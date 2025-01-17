#include "Scene.h"
#define MAX_TILE_DEPTH 5
#define TILE_SIZE 100
const size_t SPRITE_POOL_SIZE = 1300;

void Scene::UpdateGameScene(Camera& cam, GameState& gameState, InputState& inputState)
{
    sf::IntRect viewbounds(-GlobalConstants::cellSize, 0, cam.window.getSize().x + GlobalConstants::cellSize,
                           cam.window.getSize().y + GlobalConstants::cellSize);

    gameScene.clear();                                                          
    findViewportIterators(gameState.quadTree, cam, gridGenerator, viewbounds);  
    getBattlefieldCellFromMouseClick(cam, gridGenerator, inputState);           
}


GhostGrid* Scene::generateGhostGridFromScene(QuadTree* root, Camera& cam, GridGenerator& gridGenerator, sf::IntRect& viewbounds)
{
	std::function<void(QuadTree*)> populateGhostGrid = [&](QuadTree* node) {
		int screenX, screenY;
		sf::Vector2f isometricPosition = gridGenerator.cartesianToIsometricTransform(sf::Vector2f(node->quadRect.getPosition().x / tileSize, node->quadRect.getPosition().y / tileSize));
		cam.WorldToScreen(isometricPosition.x + static_cast<float>(cam.window.getSize().x) / 2, isometricPosition.y, screenX, screenY);

		int sizeX = node->quadRect.getSize().x * cam.scaleX;
		int sizeY = static_cast<float>(node->quadRect.getSize().y) / 2 * cam.scaleY
			+ static_cast<float>(tileSize) / 2 * quadTreeDepth * cam.scaleY;

		sf::IntRect isometricNodeRect(screenX - sizeX / 2, screenY, sizeX, sizeY);

		if (!viewbounds.intersects(isometricNodeRect)) return;

		if (typeid(*node) == typeid(QuadTreeLeaf)) {
			auto leaf = static_cast<QuadTreeLeaf*>(node);
			BattlefieldCell* cell = &(*leaf->iter);
			int x = cell->x;
			int y = cell->y;

			if (x >= ghostGrid.ghostGridBuffer.size()) {
				ghostGrid.ghostGridBuffer.resize(x + 1);
			}
			if (y >= ghostGrid.ghostGridBuffer[x].size()) {
				ghostGrid.ghostGridBuffer[x].resize(y + 1, nullptr);
			}

			ghostGrid.ghostGridBuffer[x][y] = cell;
		}
		else {
			for (QuadTree* child : node->children) {
				populateGhostGrid(child);
			}
		}
	};

	populateGhostGrid(root);
	return &ghostGrid;
}

void Scene::findViewportIterators(QuadTree* root, Camera& cam, GridGenerator& gridGenerator, sf::IntRect& viewbounds) {
	int screenX, screenY;
	sf::Vector2f isometricPosition = gridGenerator.cartesianToIsometricTransform(sf::Vector2f(root->quadRect.getPosition().x / TILE_SIZE, root->quadRect.getPosition().y / TILE_SIZE));
	cam.WorldToScreen(isometricPosition.x + static_cast<float>(cam.window.getSize().x) / 2, isometricPosition.y, screenX, screenY);

	int sizeX = root->quadRect.getSize().x * cam.scaleX;
	int sizeY = static_cast<float>(root->quadRect.getSize().y) / 2 * cam.scaleY
		+ static_cast<float>(TILE_SIZE) / 2 * MAX_TILE_DEPTH * cam.scaleY;

	sf::IntRect isometricNodeRect(screenX - sizeX / 2, screenY, sizeX, sizeY);

	if (!viewbounds.intersects(isometricNodeRect)) return;

	if (typeid(*root) == typeid(QuadTreeLeaf)) {
		gameScene.insert(((QuadTreeLeaf*)root)->iter);
	} else {
		for (QuadTree* child : ((QuadTree*)root)->children)
			findViewportIterators(child, cam, gridGenerator, viewbounds);
	}
}

sf::Sprite& Scene::getOrCreateSprite()
{
    // If the pool index exceeds the pool size, reset it.
    if (spritePoolIndex >= spritePool.size())
    {
        spritePoolIndex = 0;
    }
    return spritePool[spritePoolIndex++];
}

std::vector<DrawableSprite> Scene::buildGameScene(AnimationManager* animationManager)
{
    std::vector<DrawableSprite> sprites;
    std::unordered_map<std::string, SpriteSheet*> spriteSheetsCache;

    // Initialize sprite pool if empty
    if (spritePool.empty())
    {
        spritePool.resize(SPRITE_POOL_SIZE);
    }

    for (auto iter = gameScene.begin(); iter != gameScene.end(); iter++)
    {
        BattlefieldCell currentCell = **iter;
        const auto& terrainTypes = currentCell.terrain.terrainTypes;

        for (size_t i = 0; i < terrainTypes.size(); ++i)
        {
            const auto& terrainTypeName = terrainTypes[i].name;

            auto spriteSheetIter = spriteSheetsCache.find(terrainTypeName);
            if (spriteSheetIter == spriteSheetsCache.end())
            {
                spriteSheetIter =
                    spriteSheetsCache
                        .emplace(terrainTypeName, &SpriteManager::GetInstance()->GetSpriteSheet(terrainTypeName))
                        .first;
            }
            SpriteSheet* spriteSheet = spriteSheetIter->second;

            sf::Sprite& terrainSprite = getOrCreateSprite();
            terrainSprite = *spriteSheet->getSprite(currentCell.terrain.spriteIndex);
            terrainSprite.setTexture(spriteSheet->texture);
            terrainSprite.setScale(scaleX, scaleY);



            sf::Vector2f isometricPosition =
                gridGenerator.cartesianToIsometricTransform(sf::Vector2f(currentCell.x, currentCell.y));
            terrainSprite.setPosition(isometricPosition.x, isometricPosition.y - currentCell.YOffset);
            terrainSprite.setColor(sf::Color(255, 255, 255, 255 / static_cast<int>(terrainTypes.size())));

			DrawableSprite s;
			s.sprite = terrainSprite;
			s.inFoW = currentCell.FoW;

            sprites.push_back(s);
        }

        if (!currentCell.Objects.empty() && currentCell.FoW)
        {
            for (Agent* currentAgent : currentCell.Objects)
            {
                currentAgent->agentHeightAxis += (currentCell.YOffset - currentAgent->agentHeightAxis) * 0.1f;

                sf::Sprite& objectSprite = getOrCreateSprite();
                objectSprite = animationManager->getAgentSpriteFromDirection(currentAgent);
                objectSprite.setTexture(
                    SpriteManager::GetInstance()->GetSpriteSheet(currentAgent->getSpriteString()).texture);
                objectSprite.setScale(scaleX, scaleY);

                sf::Vector2f isometricPosition = gridGenerator.cartesianToIsometricTransform(
                    sf::Vector2f(currentAgent->getPosX(), currentAgent->getPosY()));
                objectSprite.setPosition(isometricPosition.x, isometricPosition.y - currentAgent->agentHeightAxis);

				DrawableSprite s;
				s.sprite = objectSprite;
				s.inFoW = true;

                sprites.push_back(s);
            }
        }
    }

    return sprites;
}

sf::Vector2i Scene::getScreenPositionOfCell(const BattlefieldCell& cell, Camera& cam, GridGenerator& gridGenerator) {
	sf::Vector2f isometricPosition = gridGenerator.cartesianToIsometricTransform(sf::Vector2f(cell.x, cell.y));

	int screenX, screenY;
	cam.WorldToScreen(isometricPosition.x + cam.window.getSize().x / 2, isometricPosition.y - cell.YOffset, screenX, screenY);

	screenX += 50;
	screenY += 100;

	return sf::Vector2i(screenX, screenY);
}


void Scene::getBattlefieldCellFromMouseClick(Camera& cam, GridGenerator& gridGenerator, InputState& inputState) {
	sf::Vector2i placeholder(-1, -1);

	if (gameScene.empty()) {
		std::cout << "Scene::getBattlefieldCellFromMouseClick Error: No cells in viewport." << std::endl;
		inputState.selectedCell = placeholder;
		return;
	}

	const int radius = 100;
	sf::Vector2i mousePos = inputState.mousePosition;
	sf::IntRect boundingBox(mousePos.x - radius, mousePos.y - radius, 2 * radius, 2 * radius);

	int cellsInBoundingBox = 0;

	for (auto iter = gameScene.begin(); iter != gameScene.end(); ++iter) {
		BattlefieldCell& cell = *(*iter);

		sf::Vector2i screenPosition = getScreenPositionOfCell(cell, cam, gridGenerator);

		if (boundingBox.contains(screenPosition)) {
			cellsInBoundingBox++;

			std::vector<sf::Vector2i> screenVertices;

			for (const auto& vertex : cell.vertices) {
				screenVertices.emplace_back(screenPosition.x + vertex.x, screenPosition.y + vertex.y);
			}

			if (pointInPolygon(mousePos, screenVertices)) {
				inputState.selectedCell = sf::Vector2i(cell.x, cell.y);
				return;
			}
		}
	}
	inputState.selectedCell = placeholder;
}


bool Scene::pointInPolygon(const sf::Vector2i& point, const std::vector<sf::Vector2i>& vertices) {
    int i, j, nvert = vertices.size();
    bool c = false;

    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((vertices[i].y > point.y) != (vertices[j].y > point.y)) &&
            (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x)) {
            c = !c;
        }
    }

    return c;
}