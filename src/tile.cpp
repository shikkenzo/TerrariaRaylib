#include "tile.h"

void Tile::DrawTile()
{
	DrawTexture(tileTexture, posX, posY, WHITE);
}
