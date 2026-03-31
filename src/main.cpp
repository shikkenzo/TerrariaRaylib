/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir


#include "map.h"

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	const int screenWidth = 1280;
	const int screenHeight = 800;
	InitWindow(1280, 800, "Terraria");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Texture dirtAtlas = LoadTexture("PC _ Computer - Terraria - Tiles - Dirt.png");

	Map map;
	map.startPos =  {-(screenWidth / 2.f), (screenWidth / 4.f) };
	map.tileCountX = 150;
	map.tileCountY = 40;
	map.tileWidth = (dirtAtlas.width / 16) - 2;
	map.tileHeight = (dirtAtlas.height / 15) - 2;
	map.GenerateMap(dirtAtlas);

	Camera2D camera = { 0 };
	camera.offset = Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;


	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(SKYBLUE);
		
		BeginMode2D(camera);
		map.DrawMap();
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
