#pragma once
#include "raylib.h"
#include <vector>

enum TileType
{
	DIRT = 0,
	AIR
};

static std::vector<Texture> textures;
void SendTextures(std::vector<Texture> t = {});

struct Tile
{
	TileType type = AIR;
	Texture voidTexture;
	Texture tileTexture;
	bool isAtlas = false;
	Vector2 tileAtlasPos = { 0, 5 };
	float drawingPosX = 0.f; //Drawing position (top left corner)
	float drawingPosY = 0.f; //Drawing position (top left corner)
	int x = 0; //?
	int y = 0; //?
	
	void SetTexture();
	void DrawTile(Vector2 pos);
};
