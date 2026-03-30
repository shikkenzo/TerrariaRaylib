#pragma once
#include "raylib.h"

struct Tile
{
	Texture tileTexture;
	float posX = 0.f;
	float posY = 0.f;
	
	void DrawTile();
};
