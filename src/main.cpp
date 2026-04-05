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

int main()
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
	Texture grassAtlas = LoadTexture("Terraria - Tiles - Grass.png");


	SendTextures({ dirtAtlas, grassAtlas });

	int tileCountX = 350;
	int tileCountY = 300;
	int horizonBase = tileCountY / 2;
	int tileWidth = 16;
	int tileHeight = 15;
	Vector2 startPos = { -(tileWidth * tileCountX / 2.f), -(tileHeight * tileCountY / 2.f) };
	float perlinStrength = 150.f;

	Map map = Map(tileCountX, tileCountY, horizonBase, tileWidth, tileHeight, startPos, perlinStrength);

	map.GenerateMapDebug();

	Player player;
	player.MoveTo({ 0.f, -((float)tileCountY / 4 * (float)tileHeight) });

	Camera2D camera = { 0 };
	camera.offset = { (screenWidth / 2.f), (screenHeight / 2.f) };
	camera.rotation = 0.0f;
	camera.zoom = 0.75f;
	camera.target = player.position;

	int i = 0;

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Vector2 p = GetScreenToWorld2D(GetMousePosition(), camera);

			int x = map.GetTileFromPos(p.x, p.y).x;
			int y = map.GetTileFromPos(p.x, p.y).y;

			if (map.CheckTile(x, y))
			{
				std::cout << x << " + " << y << ", " << map.map[y][x].type << ", " << map.map[y][x].collidable << std::endl;
				//std::cout << map.horizon[i++] << std::endl;
			}
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
		if (IsKeyPressed(KEY_R))
		{
			map.ClearMap();
			map.GenerateMapDebug();
		}
		player.AdjustFallingSpeed();


		player.CalculateVelocity(playerInput, GetFrameTime());

		player.MoveTo(player.position + player.velocity);

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
					if (collisionDirection == 3) player.isGrounded = true;
					else player.isGrounded = false;

					if (player.isGrounded) player.canJump = true;
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
