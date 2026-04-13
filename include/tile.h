#pragma once
#include "raylib.h"
#include <vector>

enum TileType
{
	DIRT = 0,
	GRASS,
	AIR
};


enum TileShape
{
	// no connections
	SHAPE_0000,

	// single connections
	SHAPE_0001_TOP,
	SHAPE_0010_RIGHT,
	SHAPE_0100_BOTTOM,
	SHAPE_1000_LEFT,

	// two connections
	SHAPE_0011_TOP_RIGHT,
	SHAPE_0110_RIGHT_BOTTOM,
	SHAPE_1100_BOTTOM_LEFT,
	SHAPE_1001_LEFT_TOP,
	SHAPE_1010_LEFT_RIGHT,
	SHAPE_0101_TOP_BOTTOM,

	// three connections
	SHAPE_0111_MISSING_LEFT,
	SHAPE_1011_MISSING_BOTTOM,
	SHAPE_1101_MISSING_RIGHT,
	SHAPE_1110_MISSING_TOP,

	// four connections
	SHAPE_1111_CROSS
};

static std::vector<Texture> textures;
void SendTextures(std::vector<Texture> t = {});

struct Tile
{
	TileType type = AIR;
	TileShape shape = SHAPE_0000;

	bool collidable = false;

	Texture tileTexture = {};
	Vector2 tileAtlasPos = { 0, 0 };

	float drawingPosX = 0.f;
	float drawingPosY = 0.f;

	int x = 0;
	int y = 0;

	Rectangle collision = { 0, 0, 0, 0 };

	void SetTexture();
	void DrawTile(Vector2 pos);
	void SetCollision(Rectangle tileCollision);
};
