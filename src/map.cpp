#include "map.h"

void Map::GenerateMap()
{
	for (int y = 0; y < tileCountY; y++)
	{
		std::vector<Tile> row;
		for (int x = 0; x < tileCountX; x++)
		{
			Tile tile;
			tile.drawingPosX = startPos.x + x * tileWidth + (1 / 2) * tileWidth;
			tile.drawingPosY = startPos.y + y * tileHeight + (1 / 2) * tileHeight;
			
			tile.x = x; //?
			tile.y = y; //?

			row.push_back(tile);

			if (y == 0)
			{
				horizon.push_back(horizonLine);
			}
		}
		map.push_back(row);
	}
}

void Map::IterateMap(std::function<void(int x, int y)> functionX, std::function<void()> functionY)
{
	for (int y = 0; y < tileCountY; y++)
	{
		for (int x = 0; x < tileCountX; x++)
		{
			functionX(x, y);
		}
		
		if (functionY) functionY();
	}
}

void Map::DecideTileType()
{
	IterateMap([this](int x, int y)
		{
			map[y][x].type = CheckGenerationRules(x, y);
		});
}

TileType Map::CheckGenerationRules(int x, int y)
{
	//CHECK HORIZON

	TileType result = AIR;

	if (y > horizon[x])
	{
		result = DIRT;
	}
	else
	{
		result = AIR;
	}

	return result;
}

void Map::ApplyPerlinNoise()
{
	Image noise = GenImagePerlinNoise(tileCountX, tileCountY, 0, 0, 1);
	ExportImage(noise, "perlinRaylib.png");

	Color* pixels = LoadImageColors(noise);

	IterateMap([this, pixels](int x, int y)
		{
			if (y != 0) return;
			Tile tile = map[y][x];

			Color c = pixels[x + (y * tileCountX)];

			int heightDiff = (c.r / 255.f) * perlinNoiseStrength;

			horizon[x] += heightDiff;
			//std::cout << horizon[x] << std::endl;
		}
	);
}

void Map::DrawMap()
{
	IterateMap([this](int x, int y) 
		{
			Tile tile = map[y][x];
			if (tile.type != AIR)
			{
				tile.DrawTile({ tile.drawingPosX, tile.drawingPosY });
			}
		});
}

void Map::AutoTile()
{
	IterateMap([this](int x, int y)
		{
			CheckAutoTileRules(x, y);
		});
}

void Map::CheckAutoTileRules(int x, int y)
{
	Tile tile = map[y][x];

	if (y > 0) Tile top = map[y-1][x];
	if (y < tileCountY) Tile bottom = map[y+1][x];
	if (x > 0) Tile left = map[y][x-1];
	if (x < tileCountX) Tile right = map[y][x+1];
}

void Map::SetTileTextures()
{
	IterateMap([this](int x, int y)
		{
			map[y][x].SetTexture();
		});
}

void Map::DrawGrid()
{
	IterateMap([this](int x, int y)
		{
			Tile tile = map[y][x];

			DrawLine(0, tile.drawingPosY, tile.drawingPosX, tile.drawingPosY, WHITE);
			DrawLine(tile.drawingPosX, 0, tile.drawingPosX, tile.drawingPosY, WHITE);

			DrawCircle(GetPosFromTile(x, y).x, GetPosFromTile(x, y).y, 2, BLUE);
			//DrawText(std::to_string(x).c_str(), GetPosFromTile(x, y).x, GetPosFromTile(x, y).y, 2, WHITE);
			//DrawText(std::to_string(y).c_str(), GetPosFromTile(x, y).x, GetPosFromTile(x, y).y, 2, WHITE);

		});

	DrawLine(-GetScreenWidth(), 0, GetScreenWidth(), 0, BLACK);
	DrawLine(0, -GetScreenHeight(), 0, GetScreenHeight(), BLACK);
	DrawCircle(0, 0, 10, RED);
}

Vector2 Map::GetTileFromPos(float x, float y)
{
	return { std::floor(x / tileWidth), std::floor(y / tileHeight) };
}

Vector2 Map::GetPosFromTile(int x, int y)
{
	Tile tile = map[y][x];
	return { tile.drawingPosX + tileWidth / 2, tile.drawingPosY + tileHeight / 2 };
}
