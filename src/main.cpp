/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <iostream>
#include "map.h"
#include "player.h"
#include "collisionSystem.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	//SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	SetConfigFlags(FLAG_VSYNC_HINT);

	// Create the window and OpenGL context
	const int screenWidth = 1280;
	const int screenHeight = 800;
	InitWindow(screenWidth, screenHeight, "Terraria");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Texture dirtAtlas = LoadTexture("PC _ Computer - Terraria - Tiles - Dirt.png");
	Texture airTexture = LoadTexture("PC _ Computer - Terraria - Tiles - Dirt.png");
	
	SendTextures({dirtAtlas});

	Map map;
	map.tileCountX = 350;
	map.tileCountY = 300;
	map.horizonLine = map.tileCountY / 2;
	map.tileWidth = (dirtAtlas.width / 16) - 2.f;
	map.tileHeight = (dirtAtlas.height / 15) - 2.f;
	map.startPos =  {-(map.tileWidth * map.tileCountX / 2.f), -(map.tileHeight * map.tileCountY / 2.f) };
	map.perlinNoiseStrength = 50.f;
	map.GenerateMap();
	map.ApplyPerlinNoise();
	map.DecideTileType();
	//map.AutoTile();
	map.SetTileTextures();

	Player player;

	Camera2D camera = { 0 };
	camera.offset = { (screenWidth / 2.f), (screenHeight / 2.f) };
	camera.rotation = 0.0f;
	camera.zoom = 1.f;
	camera.target = player.position;
	
	int i = 0;

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			auto p = GetScreenToWorld2D(GetMousePosition(), camera);
			//std::cout << map.GetTileFromPos(p.x, p.y).x << " + " << map.GetTileFromPos(p.x, p.y).y << std::endl;
			//std::cout << p.x << std::endl;
			//std::cout << map.horizon[i++] << std::endl;
		}

		Vector2 playerInput = { 0, 0 };
		if (IsKeyDown(KEY_A))
		{
			playerInput.x += -1;
		}
		if (IsKeyDown(KEY_D))
		{
			playerInput.x += 1;
		}
		if (IsKeyPressed(KEY_SPACE))
		{
			player.Jump();
		}
		

		player.CalculateVelocity(playerInput, GetFrameTime());

		// Move X
		player.position.x += player.velocity.x;

		// Move Y
		player.position.y += player.velocity.y;

		player.AdjustCollider();

		map.IterateMap([map, &player](int x, int y)
			{
				Tile tile = map.map[y][x];
				Rectangle tileCollision = { tile.drawingPosX, tile.drawingPosY, map.tileWidth, map.tileHeight };
				if (tile.type == AIR) return;

				int collisionDirection;
				if (CheckCollisionAABB(tileCollision, player.collision, collisionDirection))
				{
					//std::cout << "collision" << std::endl;
					ResolvePlayerCollision(player, tileCollision, collisionDirection);
					if (collisionDirection == 3) player.canJump = true;
				}


			});

		// drawing
		BeginDrawing();

			// Setup the back buffer for drawing (clear color and depth buffers)
			ClearBackground(SKYBLUE);
		
			BeginMode2D(camera);
				
				camera.target = player.position;

				map.DrawMap();
				//map.DrawGrid();
				player.Draw();

			EndMode2D();

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(dirtAtlas);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
