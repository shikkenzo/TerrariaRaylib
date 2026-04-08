#pragma once
#include "tile.h"
#include <vector>
#include <string>
#include <iostream>

struct Map
{
	Map(int _tileCountX, int _tileCountY, int _horizonBase, int _tileWidth, int _tileHeight, Vector2 _startPos, float _perlinStrength)
	{
		tileCountX = _tileCountX;
		tileCountY = _tileCountY;
		horizonLine = _horizonBase;
		tileWidth = _tileWidth;
		tileHeight = _tileHeight;
		startPos = _startPos;
		perlinNoiseStrength = _perlinStrength;
	}
	Vector2 startPos;
	float tileHeight;
	float tileWidth;
	int tileCountX;
	int tileCountY;

	std::vector<std::vector<Tile>> map;

	float perlinNoiseStrength = 10.f;

	void GenerateMap();
	void ClearMap();
	void GenerateMapDebug();
	template<typename F1>
	auto IterateMap(F1 functionX)
	{
		for (int y = 0; y < tileCountY; y++)
		{
			for (int x = 0; x < tileCountX; x++)
			{
				functionX(x, y);
			}
		}
	}
	template<typename F1, typename F2>
	auto IterateMap(F1 functionX, F2 functionY)
	{
		for (int y = 0; y < tileCountY; y++)
		{
			functionY(y);

			for (int x = 0; x < tileCountX; x++)
			{
				functionX(x, y);
			}
		}
	}
	void DecideTileType();
	void ApplyPerlinNoiseY();
	void ApplyPerlinNoiseX();
	void DrawMap(Rectangle cullingRect);
	void GetAdjacentTiles(Tile tile, Tile& outTop, Tile& outBottom, Tile& outLeft, Tile& outRight);
	bool CheckTile(int x, int y);
	void AutoTile();
	void SetTileTextures();
	Vector2 GetWordTileCoordFromPos(float posX, float posY);
	Vector2 Map::GetTileFromPos(float posX, float posY);
	Vector2 GetPosFromTile(int x, int y);
	void DrawGrid();

	//RULES
	int horizonLine = 50;
	std::vector<int> horizon;

private:
	Vector2 CheckAutoTileRules(int x, int y);
	TileType CheckGenerationRules(int x, int y);
};
