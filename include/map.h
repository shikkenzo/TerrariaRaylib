#pragma once
#include "tile.h"
#include <vector>
#include <functional>
#include <string>
#include <iostream>

struct Map
{
	Vector2 startPos = { 0.f, 0.f };
	float tileHeight = 10.f;
	float tileWidth = 10.f;
	int tileCountX = 2;
	int tileCountY = 2;

	std::vector<std::vector<Tile>> map;

	float perlinNoiseStrength = 10.f;

	void GenerateMap();
	void IterateMap(std::function<void(int x, int y)> functionX, std::function<void()> functionY = nullptr);
	void DecideTileType();
	void ApplyPerlinNoise();
	void DrawMap();
	void AutoTile();
	void SetTileTextures();
	Vector2 GetTileFromPos(float x, float y);
	Vector2 GetPosFromTile(int x, int y);
	void DrawGrid();


	//RULES
	int horizonLine = 50;
	std::vector<int> horizon;

private:
	void CheckAutoTileRules(int x, int y);
	TileType CheckGenerationRules(int x, int y);
};
