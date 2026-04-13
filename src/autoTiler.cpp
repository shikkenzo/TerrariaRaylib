#include "autoTiler.h"

static Vector2 dirtAtlas[16] =
{
	{1,1}, // 0000 isolated

	{6,3}, // 0001 top
	{9,0}, // 0010 right
	{6,0}, // 0100 bottom
	{12,0}, // 1000 left

	{0,4}, // 0011 top + right
	{0,3}, // 0110 right + bottom
	{1,3}, // 1100 bottom + left
	{3,4}, // 1001 left + top
	{6,4}, // 1010 left + right
	{5,0}, // 0101 top + bottom

	{0,0}, // 0111 missing left
	{1,2}, // 1011 missing bottom
	{4,0}, // 1101 missing right
	{1,0}, // 1110 missing top

	{1,1}  // 1111 fully surrounded
};
Vector2 dirtVariations[16] =
{
	{3,1}, // 0000 isolated

	{1,1}, // 0001 top
	{1,1}, // 0010 right
	{1,1}, // 0100 bottom
	{1,1}, // 1000 left

	{1,1}, // 0011 top + right
	{1,1}, // 0110 right + bottom
	{1,1}, // 1100 bottom + left
	{1,1}, // 1001 left + top
	{1,1}, // 1010 left + right
	{1,1}, // 0101 top + bottom

	{1,1}, // 0111 missing left
	{1,1}, // 1011 missing bottom 
	{1,1}, // 1101 missing right
	{1,1}, // 1110 missing top

	{3,1}  // 1111 fully surrounded
};

static Vector2 grassAtlas[16] =
{
	{9,3}, // 0000 isolated

	{6,3}, // 0001 top
	{9,0}, // 0010 right
	{6,0}, // 0100 bottom
	{12,0}, // 1000 left

	{0,4}, // 0011 top + right
	{0,3}, // 0110 right + bottom
	{1,3}, // 1100 bottom + left
	{3,4}, // 1001 left + top
	{6,4}, // 1010 left + right
	{5,0}, // 0101 top + bottom

	{0,0}, // 0111 missing left
	{1,2}, // 1011 missing bottom
	{4,0}, // 1101 missing right
	{1,0}, // 1110 missing top

	{1,1}  // 1111 fully surrounded
};
Vector2 grassVariations[16] =
{
	{3,1}, // 0000 isolated

	{1,1}, // 0001 top
	{1,1}, // 0010 right
	{1,1}, // 0100 bottom
	{1,1}, // 1000 left

	{1,1}, // 0011 top + right
	{1,1}, // 0110 right + bottom
	{1,1}, // 1100 bottom + left
	{1,1}, // 1001 left + top
	{1,1}, // 1010 left + right
	{1,1}, // 0101 top + bottom

	{1,1}, // 0111 missing left
	{1,1}, // 1011 missing bottom
	{1,1}, // 1101 missing right
	{1,1}, // 1110 missing top

	{3,1}  // 1111 fully surrounded
};

bool IsBlendable(TileType a, TileType b)
{
	if (a == b) return true;

	if ((a == DIRT && b == GRASS) ||
		(a == GRASS && b == DIRT))
		return true;

	return false;
}

int ComputeMask(
	TileType center,
	TileType top,
	TileType right,
	TileType bottom,
	TileType left)
{
	int mask = 0;

	if (IsBlendable(top, center))    mask |= 1;
	if (IsBlendable(right, center))  mask |= 2;
	if (IsBlendable(bottom, center)) mask |= 4;
	if (IsBlendable(left, center))   mask |= 8;

	return mask;
}

TileShape GetShapeFromMask(int mask)
{
	switch (mask)
	{
	case 0:  return SHAPE_0000;

	case 1:  return SHAPE_0001_TOP;
	case 2:  return SHAPE_0010_RIGHT;
	case 4:  return SHAPE_0100_BOTTOM;
	case 8:  return SHAPE_1000_LEFT;

	case 3:  return SHAPE_0011_TOP_RIGHT;
	case 6:  return SHAPE_0110_RIGHT_BOTTOM;
	case 12: return SHAPE_1100_BOTTOM_LEFT;
	case 9:  return SHAPE_1001_LEFT_TOP;
	case 10: return SHAPE_1010_LEFT_RIGHT;
	case 5:  return SHAPE_0101_TOP_BOTTOM;

	case 7:  return SHAPE_0111_MISSING_LEFT;
	case 11: return SHAPE_1011_MISSING_BOTTOM;
	case 13: return SHAPE_1101_MISSING_RIGHT;
	case 14: return SHAPE_1110_MISSING_TOP;

	case 15: return SHAPE_1111_CROSS;
	}

	return SHAPE_0000;
}

Vector2 GetUV(TileType type, TileShape shape, int x, int y)
{
	int index = (int)shape;

	Vector2 grid =
		(type == GRASS)
		? grassVariations[index]
		: dirtVariations[index];

	int gridX = (int)grid.x;
	int gridY = (int)grid.y;

	if (gridX <= 0 || gridY <= 0)
	{
		return (type == GRASS)
			? grassAtlas[index]
			: dirtAtlas[index];
	}

	unsigned int r =
		x * 845678u ^
		y * 123432u ^
		index * 37543u;

	int variationX = (gridX > 1) ? (r % gridX) : 0;
	int variationY = (gridY > 1) ? ((r / gridX) % gridY) : 0;

	Vector2 base =
		(type == GRASS)
		? grassAtlas[index]
		: dirtAtlas[index];

	return { base.x + variationX, base.y + variationY };
}
