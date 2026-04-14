#include "tile.h"

void Tile::DrawTile(Vector2 pos)
{
	DrawTextureRec(tileTexture, { 18 * tileAtlasPos.x, 18 * tileAtlasPos.y, 18 - 2, 18 - 2 }, pos, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters
}

void Tile::SetTexture()
{
	if (type != AIR)
	{
		tileTexture = textures[type];
	}
}

void Tile::SetCollision(Rectangle tileCollision)
{
	if (type == AIR)
	{
		collidable = false;
	}
	else
	{
		collidable = true;
	}

	if (collidable)
	{
		collision = tileCollision;
	}
	else
	{
		collision = {};
	}
}

void SendTextures(std::vector<Texture> t)
{
	for (int i = 0; i < std::size(t); i++)
	{
		textures.push_back(t[i]);
	}
}
