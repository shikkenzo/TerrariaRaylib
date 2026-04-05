#pragma once
#include "raylib.h"
#include <vector>

enum TileType
{
	DIRT = 0,
	GRASS,
	AIR
};

static std::vector<Texture> textures;
void SendTextures(std::vector<Texture> t = {});

struct Tile
{
	TileType type = AIR;
	bool collidable = false;
	Texture voidTexture;
	Texture tileTexture;
	bool isAtlas = false;
	Vector2 tileAtlasPos = { 0, 5 };
	float drawingPosX = 0.f; //Drawing position (from top left corner)
	float drawingPosY = 0.f; //Drawing position (from top left corner)
	Rectangle collision = {drawingPosX, drawingPosY, 0, 0};
	int x = 0;
	int y = 0;
	bool valid = 1; //?
	
	void SetTexture();
	void DrawTile(Vector2 pos);
	void SetCollision(Rectangle tileCollision);
};
