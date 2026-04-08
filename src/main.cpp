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

constexpr bool DEBUG_MOVEMENT = false;

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
	player.MoveTo({ 0.f, -((float)tileCountY * (float)tileHeight) });

	Camera2D camera = { 0 };
	camera.offset = { (screenWidth / 2.f), (screenHeight / 2.f) };
	camera.rotation = 0.0f;
	camera.zoom = 0.75f;
	camera.target = player.position;

	constexpr float maxDeltaTime = 0.033f;

	SetTargetFPS(30);
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float deltaTime = std::min(GetFrameTime(), maxDeltaTime);
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
		if (IsKeyDown(KEY_W))
		{
			playerInput.y += -1;
		}
		if (IsKeyDown(KEY_S))
		{
			playerInput.y += 1;
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


		if (!DEBUG_MOVEMENT) player.CalculateVelocity(playerInput, deltaTime);
		else player.velocity = playerInput * deltaTime * player.baseSpeed;

		Hit hit;
		Vector2 closerHitPos;
		float closestT = 1.f;
		bool wasHit = false;
		map.IterateMap([&map, &player, &closerHitPos, &hit, &wasHit, &closestT](int x, int y)
			{
				Tile& tile = map.map[y][x];
				tile.collision = { tile.drawingPosX, tile.drawingPosY, map.tileWidth, map.tileHeight };
				if (tile.type == AIR) return;

				Vector2 sumPos = { tile.collision.x + tile.collision.width / 2, tile.collision.y + tile.collision.height / 2 };
				Rectangle sum = { sumPos.x, sumPos.y, (tile.collision.width / 2 + player.collision.width / 2) * 2, (tile.collision.height / 2 + player.collision.height / 2) * 2 };
				sum.x -= sum.width / 2;
				sum.y -= sum.height / 2;

				hit = ShapecastAABB(player.position, player.position + player.velocity * 1.1f, sum); //!
				if (hit.isHit)
				{
					wasHit = true;

					if (hit.isHit && hit.time < closestT)
					{
						closestT = hit.time;
						closerHitPos = hit.position;
						std::cout << "shapecast" << std::endl;
					}
				}
			});

		if (wasHit)
		{
			player.MoveTo(closerHitPos);
			//player.velocity = {};
		}
		else
		{
			
		}

		player.MoveTo(player.position + player.velocity);

		

		//map.IterateMap([&map, &player](int x, int y)
		//	{
		//		Tile& tile = map.map[y][x];
		//		tile.collision = { tile.drawingPosX, tile.drawingPosY, map.tileWidth, map.tileHeight };
		//		if (tile.type == AIR) return;

		//		int collisionDirection;
		//		if (CheckCollisionAABB(tile.collision, player.collision, collisionDirection))
		//		{
		//			//std::cout << "collision" << std::endl;
		//			ResolvePlayerCollision(player, tile.collision, collisionDirection);
		//			if (collisionDirection == 3) player.isGrounded = true;
		//			else player.isGrounded = false;

		//			if (player.isGrounded) player.canJump = true;
		//		}
		//	});
		

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(SKYBLUE);

		BeginMode2D(camera);

		camera.target = player.position;

		map.DrawMap();
		map.DrawGrid();
		player.Draw();

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			/*Rectangle collisionAABB = { 0.f, -((float)tileCountY / 3 * (float)tileHeight), 100.f, 100.f };
			Vector2 collisionPos = { collisionAABB.x + collisionAABB.width / 2, collisionAABB.y + collisionAABB.height / 2 };*/

			Vector2 cursorPos = GetScreenToWorld2D(GetMousePosition(), camera);
			Vector2 cursorCollisionPos = cursorPos - Vector2{ player.collision.width / 2,  player.collision.height / 2 };
			Rectangle cursorCollision = { cursorCollisionPos.x, cursorCollisionPos.y, player.collision.width, player.collision.height };


			Hit hit;
			map.IterateMap([map, camera, player, tileCountY, tileHeight, cursorCollision, cursorPos, &hit, &closerHitPos](int x, int y)
				{
					Tile tile = map.map[y][x];
					if (tile.type == AIR) return;

					Rectangle collisionAABB = { tile.collision.x, tile.collision.y, tile.collision.width, tile.collision.height };
					Vector2 collisionPos = { collisionAABB.x + collisionAABB.width / 2, collisionAABB.y + collisionAABB.height / 2 };

					Vector2 sumPos = collisionPos;
					Rectangle sum = { sumPos.x, sumPos.y, (collisionAABB.width / 2 + cursorCollision.width / 2) * 2, (collisionAABB.height / 2 + cursorCollision.width / 2) * 2 };
					sum.x -= sum.width / 2;
					sum.y -= sum.height / 2;

					hit = ShapecastAABB(player.position, cursorPos, sum);
					if (hit.isHit)
					{
						//std::cout << "shapecast" << std::endl;
						DrawLine(player.position.x, player.position.y, hit.position.x, hit.position.y, RED);
						DrawRectangle(hit.position.x - player.collision.width / 2, hit.position.y - player.collision.height / 2, player.collision.width, player.collision.height, RED);
					}

					//DrawRectangle(sum.x, sum.y, sum.width, sum.height, GRAY);
					//DrawRectangle(collisionAABB.x, collisionAABB.y, collisionAABB.width, collisionAABB.height, DARKGRAY);
					//DrawCircle(collisionPos.x, collisionPos.y, 10, BLUE);
				});
			DrawRectangle(cursorCollisionPos.x, cursorCollisionPos.y, player.collision.width, player.collision.height, GREEN);

		}
		
		if (wasHit)
			DrawRectangle(closerHitPos.x - player.collision.width / 2, closerHitPos.y - player.collision.height / 2, player.collision.width, player.collision.height, PURPLE);

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
