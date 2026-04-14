#pragma once
#include "tile.h"

Vector2 GetUV(TileType type, TileShape shape, int x, int y);

TileShape GetShapeFromMask(int mask);

int ComputeMask(
	TileType center,
	TileType top,
	TileType right,
	TileType bottom,
	TileType left
);

bool IsBlendable(TileType a, TileType b);
