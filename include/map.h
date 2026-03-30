#pragma once
#include "tile.h"
#include <vector>

struct Map
{
	Vector2 startPos = { 0.f, 0.f };
	float tileHeight = 10.f;
	float tileWidth = 10.f;
	int tileCountX = 2;
	int tileCountY = 2;

	std::vector<std::vector<Tile>> map;

	void GenerateMap(Texture& texture);
	void Map::DrawMap();
};
