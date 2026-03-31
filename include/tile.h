#pragma once
#include "raylib.h"

enum TileType
{

};

struct Tile
{
	TileType type;
	Texture tileTexture;
	Vector2 tileAtlasPos = { 0, 5 };
	float posX = 0.f;
	float posY = 0.f;
	
	void DrawTile(Vector2 pos);
};
