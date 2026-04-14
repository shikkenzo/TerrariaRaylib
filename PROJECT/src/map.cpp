#include "map.h"

void Map::GenerateMap()
{
	for (int y = 0; y < tileCountY; y++)
	{
		std::vector<Tile> row;
		for (int x = 0; x < tileCountX; x++)
		{
			Tile tile;
			tile.x = x;
			tile.y = y;

			row.push_back(tile);

			if (y == 0)
			{
				horizon.push_back(horizonLine);
			}
		}
		map.push_back(row);
	}

	ApplyPerlinNoiseY();
	//ApplyPerlinNoiseX();
	DecideTileType();
	SetTileTextures();
	AutoTile();
}

void Map::GenerateMapDebug()
{
	for (int y = 0; y < tileCountY; y++)
	{
		std::vector<Tile> row;
		for (int x = 0; x < tileCountX; x++)
		{
			Tile tile;
			tile.x = x;
			tile.y = y;
			row.push_back(tile);

			if (y == 0)
			{
				horizon.push_back(horizonLine);
			}
		}
		map.push_back(row);
	}

	ApplyPerlinNoiseY();
	DecideTileType();
	ApplyPerlinNoiseX();
	DecideTileType();
	SetTileTextures();
	AutoTile();
}

void Map::ClearMap()
{
	IterateMap([this](int x, int y)
		{
			map[y][x].type = AIR;
		});
}

void Map::DecideTileType()
{
	IterateMap([this](int x, int y)
		{
			Tile& tile = map[y][x];
			tile.type = CheckGenerationRules(x, y);
			tile.SetCollision({ tile.drawingPosX, tile.drawingPosY, tileWidth, tileHeight });
		});
}

TileType Map::CheckGenerationRules(int x, int y)
{
	TileType result = AIR;

	if (y == horizon[x])
	{
		result = GRASS;
	}
	else if (y > horizon[x])
	{
		result = DIRT;
	}
	else
	{
		result = AIR;
	}

	return result;
}

void Map::ApplyPerlinNoiseY()
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

			horizon[x] = horizonLine - heightDiff;
			//std::cout << horizon[x] << std::endl;
		}
	);
}

void Map::ApplyPerlinNoiseX()
{
	Image noise = GenImagePerlinNoise(tileCountX, tileCountY, 0, 0, 1);
	ExportImage(noise, "perlinRaylib.png");

	Color* pixels = LoadImageColors(noise);
	Color c;

	int n = tileCountX * tileCountX;
	int average = 0;

	IterateMap([this, pixels, c, &average](int x, int y)
		{
			Tile& tile = map[y][x];

			if (y >= horizonLine) return;
			int widthDisplacement = (c.r / 255.f) * (horizonLine / (float)y) * 50.f;

			if (CheckTile(x - widthDisplacement, y))
			{
				//std::cout << widthDisplacement << std::endl;

				map[y][x - widthDisplacement].type = tile.type;
				tile.type = AIR;
			}

			//tile.x -= widthDisplacement;
			//average += widthDisplacement;
			//std::cout << horizon[x] << std::endl;
		},
		[this, pixels, &c](int y)
		{
			c = pixels[y * tileCountX];
			//std::cout << (int)((c.r / 255.f) * 200.f) << std::endl;
		}
	);

	//average /= n;

	//IterateMap([this, pixels, c, &average](int x, int y)
	//	{
	//		Tile& tile = map[y][x];
	//		if (tile.y > horizonLine) return;

	//		int widthDisplacement = 100;

	//		tile.x += widthDisplacement;
	//		//std::cout << horizon[x] << std::endl;
	//	}
	//);
}

void Map::DrawMap(Rectangle cullingRect)
{
	IterateMap([this, cullingRect](int x, int y)
		{
			Tile& tile = map[y][x];

			tile.drawingPosX = startPos.x + tile.x * tileWidth + (1 / 2) * tileWidth;
			tile.drawingPosY = startPos.y + tile.y * tileHeight + (1 / 2) * tileHeight;

			if (!CheckCollisionRecs(tile.collision, cullingRect)) return;

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
			Tile& tile = map[y][x];

			Tile t, b, l, r;
			GetAdjacentTiles(tile, t, b, l, r);

			int mask = ComputeMask(
				tile.type,
				t.type,
				r.type,
				b.type,
				l.type
			);

			tile.shape = GetShapeFromMask(mask);
			tile.tileAtlasPos = GetUV(tile.type, tile.shape, x, y);
		});
}

void Map::GetAdjacentTiles(Tile tile, Tile& outTop, Tile& outBottom, Tile& outLeft, Tile& outRight)
{
	int x = tile.x;
	int y = tile.y;

	if (y > 0) outTop = map[y - 1][x];

	if (y < tileCountY - 1) outBottom = map[y + 1][x];

	if (x > 0) outLeft = map[y][x - 1];

	if (x < tileCountX - 1) outRight = map[y][x + 1];
}

bool Map::CheckTile(int x, int y)
{
	if (
		y < 0 ||
		y >= tileCountY ||
		x < 0 ||
		x >= tileCountX
		)
		return false;
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
	//IterateMap([this](int x, int y)
	//	{
	//		Tile tile = map[y][x];

	//		DrawLine(0, tile.drawingPosY, tile.drawingPosX, tile.drawingPosY, WHITE);
	//		DrawLine(tile.drawingPosX, 0, tile.drawingPosX, tile.drawingPosY, WHITE);

	//		DrawCircle(GetPosFromTile(x, y).x, GetPosFromTile(x, y).y, 2, BLUE);
	//		//DrawText(std::to_string(x).c_str(), GetPosFromTile(x, y).x, GetPosFromTile(x, y).y, 2, WHITE);
	//		//DrawText(std::to_string(y).c_str(), GetPosFromTile(x, y).x, GetPosFromTile(x, y).y, 2, WHITE);

	//	});

	DrawLine(-GetScreenWidth(), 0, GetScreenWidth(), 0, BLACK);
	DrawLine(0, -GetScreenHeight(), 0, GetScreenHeight(), BLACK);
	DrawCircle(0, 0, 10, RED);
}

Vector2 Map::GetWordTileCoordFromPos(float posX, float posY)
{
	float x = std::floor(posX / tileWidth);
	float y = std::floor(posY / tileHeight);

	return { x, y };
}

Vector2 Map::GetTileFromPos(float posX, float posY)
{
	float x = std::floor((posX - startPos.x) / tileWidth);
	float y = std::floor((posY - startPos.y) / tileHeight);

	return { x, y };
}

Vector2 Map::GetPosFromTile(int x, int y)
{
	Tile tile = map[y][x];
	return { tile.drawingPosX + tileWidth / 2, tile.drawingPosY + tileHeight / 2 };
}
