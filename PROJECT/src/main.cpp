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
	Texture background = LoadTexture("Map_Background_Sky.png");


	SendTextures({ dirtAtlas, grassAtlas });

	int tileCountX = 350;
	int tileCountY = 300;
	int horizonBase = tileCountY / 2;
	int tileWidth = 16;
	int tileHeight = 16;
	Vector2 startPos = { -(tileWidth * tileCountX / 2.f), -(tileHeight * tileCountY / 2.f) };
	float perlinStrength = 150.f;

	Map map = Map(tileCountX, tileCountY, horizonBase, tileWidth, tileHeight, startPos, perlinStrength);

	map.GenerateMapDebug();

	Player player;
	player.MoveTo({ 0.f, -((float)tileCountY / 3 * (float)tileHeight) });

	Camera2D camera = { 0 };
	camera.offset = { (screenWidth / 2.f), (screenHeight / 2.f) };
	camera.rotation = 0.0f;
	camera.zoom = 1.f;
	camera.target = player.position;

	constexpr float maxDeltaTime = 0.033f;

	SetTargetFPS(300);
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float deltaTime = std::min(GetFrameTime(), maxDeltaTime);
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 p = GetScreenToWorld2D(GetMousePosition(), camera);

			int x = map.GetTileFromPos(p.x, p.y).x;
			int y = map.GetTileFromPos(p.x, p.y).y;

			if (map.CheckTile(x, y))
			{
				std::cout << x << " + " << y << ", " << map.map[y][x].type << ", " << map.map[y][x].collidable << std::endl;
				//std::cout << map.horizon[i++] << std::endl;
				map.map[y][x].type = AIR;
				map.AutoTile();
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


		// Horizontal Movement
		{
			Vector2 start = player.position;
			Vector2 end = player.position + Vector2{ player.velocity.x, 0 };

			Rectangle sweepBounds;
			sweepBounds.x = std::min(start.x, end.x) - player.collision.width / 2;
			sweepBounds.y = std::min(start.y, end.y) - player.collision.height / 2;
			sweepBounds.width = std::abs(end.x - start.x) + player.collision.width;
			sweepBounds.height = player.collision.height;

			float closestT = 1.0f;
			Vector2 hitPos = end;
			bool hit = false;

			map.IterateMap([&](int x, int y)
				{
					Tile& tile = map.map[y][x];
					if (tile.type == AIR) return;

					tile.collision = { tile.drawingPosX, tile.drawingPosY, map.tileWidth, map.tileHeight };

					// distance culling
					float dist = Vector2Length(player.position - Vector2{
						tile.collision.x + map.tileWidth / 2,
						tile.collision.y + map.tileHeight / 2
						});

					if (dist > player.collision.width * 4.0f) return;

					int outDir;
					if (!CheckCollisionAABB(tile.collision, sweepBounds, outDir)) return;

					// Minkowski sum
					Rectangle sum;
					sum.width = tile.collision.width + player.collision.width;
					sum.height = tile.collision.height + player.collision.height;
					sum.x = tile.collision.x - player.collision.width / 2;
					sum.y = tile.collision.y - player.collision.height / 2;

					Hit h = ShapecastAABB(start, end, sum);

					if (h.isHit && h.time < closestT)
					{
						closestT = h.time;
						hitPos = h.position;
						hit = true;
					}
				});

			if (hit)
			{
				player.position.x = hitPos.x;
				player.velocity.x = 0;
			}
			else
			{
				player.position.x = end.x;
			}
		}


		// Vertical Movement
		{
			Vector2 start = player.position;
			Vector2 end = player.position + Vector2{ 0, player.velocity.y };

			Rectangle sweepBounds;
			sweepBounds.x = player.position.x - player.collision.width / 2;
			sweepBounds.y = std::min(start.y, end.y) - player.collision.height / 2;
			sweepBounds.width = player.collision.width;
			sweepBounds.height = std::abs(end.y - start.y) + player.collision.height;

			float closestT = 1.0f;
			Vector2 hitPos = end;
			bool hit = false;

			player.isGrounded = false;

			map.IterateMap([&](int x, int y)
				{
					Tile& tile = map.map[y][x];
					if (tile.type == AIR) return;

					tile.collision = { tile.drawingPosX, tile.drawingPosY, map.tileWidth, map.tileHeight };

					float dist = Vector2Length(player.position - Vector2{
						tile.collision.x + map.tileWidth / 2,
						tile.collision.y + map.tileHeight / 2
						});

					if (dist > player.collision.height * 4.0f) return;

					int outDir;
					if (!CheckCollisionAABB(tile.collision, sweepBounds, outDir)) return;

					Rectangle sum;
					sum.width = tile.collision.width + player.collision.width;
					sum.height = tile.collision.height + player.collision.height;
					sum.x = tile.collision.x - player.collision.width / 2;
					sum.y = tile.collision.y - player.collision.height / 2;

					Hit h = ShapecastAABB(start, end, sum);

					if (h.isHit && h.time < closestT)
					{
						closestT = h.time;
						hitPos = h.position;
						hit = true;
					}
				});

			if (hit)
			{
				player.position.y = hitPos.y;

				if (player.velocity.y > 0)
					player.isGrounded = true;

				player.velocity.y = 0;
			}
			else
			{
				player.position.y = end.y;
			}
		}

		player.AdjustCollider();

		if (player.isGrounded) player.canJump = true;

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

		//Camera World Rectangle
		float currentZoom = std::min(camera.zoom, 1.f);
		camera.zoom = 1.f;

		Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, camera);
		Vector2 bottomRight = GetScreenToWorld2D({
			(float)GetScreenWidth(),
			(float)GetScreenHeight()
			}, camera);

		Rectangle rect;
		rect.x = topLeft.x;
		rect.y = topLeft.y;
		rect.width = bottomRight.x - topLeft.x;
		rect.height = bottomRight.y - topLeft.y;

		camera.zoom = currentZoom;

		DrawTexturePro(background, { 0, 0, (float)background.width, (float)background.height }, { topLeft.x - 10, topLeft.y - 10, (bottomRight.x - topLeft.x) + 10,  (bottomRight.y - topLeft.y) + 10 }, { 0, 0 }, 0, WHITE);

		map.DrawMap(rect);
		//map.DrawGrid();
		player.Draw();

		//DEBUG //!
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			/*Rectangle collisionAABB = { 0.f, -((float)tileCountY / 3 * (float)tileHeight), 100.f, 100.f };
			Vector2 collisionPos = { collisionAABB.x + collisionAABB.width / 2, collisionAABB.y + collisionAABB.height / 2 };*/

			Vector2 cursorPos = GetScreenToWorld2D(GetMousePosition(), camera);
			Vector2 cursorCollisionPos = cursorPos - Vector2{ player.collision.width / 2,  player.collision.height / 2 };
			Rectangle cursorCollision = { cursorCollisionPos.x, cursorCollisionPos.y, player.collision.width, player.collision.height };


			Hit hit;
			map.IterateMap([map, camera, player, tileCountY, tileHeight, cursorCollision, cursorPos, &hit](int x, int y)
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
		//DEBUG END //!


		//if (wasHit)
			//DrawRectangle(closerHitPosX.x - player.collision.width / 2, closerHitPos.y - player.collision.height / 2, player.collision.width, player.collision.height, PURPLE);

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
