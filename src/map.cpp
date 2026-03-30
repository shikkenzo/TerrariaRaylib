#include "map.h"

void Map::GenerateMap(Texture& texture)
{
	for (int y = 0; y < tileCountY; y++)
	{
		std::vector<Tile> row;
		for (int x = 0; x < tileCountX; x++)
		{
			Tile tile;
			tile.tileTexture = texture;
			tile.posX = startPos.x + x * tileWidth + (1 / 2) * tileWidth;
			tile.posY = startPos.y + y * tileHeight + (1 / 2) * tileHeight;

			row.push_back(tile);
		}
		map.push_back(row);
	}
}

void Map::DrawMap()
{
	for (int y = 0; y < tileCountY; y++)
	{
		for (int x = 0; x < tileCountX; x++)
		{
			Tile tile = map[y][x];
			DrawTexture(tile.tileTexture, tile.posX, tile.posY, WHITE);
		}
	}
}
