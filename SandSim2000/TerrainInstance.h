#pragma once

#include <string>
#include <vector>

struct TerrainType
{
	TerrainType(std::string Name, std::string SpriteSheetFilePath) : name(Name), spriteSheetFilePath(SpriteSheetFilePath) {}
	TerrainType() : name(""), spriteSheetFilePath("") {}

	std::string name, spriteSheetFilePath;
};

struct TerrainInstance
{
	TerrainInstance() {terrainTypes = std::vector<TerrainType>(); }
	TerrainInstance(std::vector<TerrainType> allTerrainTypes) {terrainTypes = allTerrainTypes;}
	TerrainInstance(TerrainType onlyTerrainType) {terrainTypes.push_back(onlyTerrainType);}

	void setSpriteIndex(int direction) { spriteIndex = direction; }

	std::vector<TerrainType> terrainTypes; 
	int spriteIndex = 0;
};