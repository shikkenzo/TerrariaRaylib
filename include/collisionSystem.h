#pragma once
#include "raylib.h"
#include "player.h"

bool CheckCollisionAABB(Rectangle a, Rectangle b, int& directionOut);
void ResolvePlayerCollision(Player& p, Rectangle c, int collisionDirection);

struct Hit
{
	bool isHit = false;
	float time = 0.f;
	Vector2 position = {};
};

constexpr int DIMENSIONS = 2;
Hit ShapecastAABB(Vector2 startingPos, Vector2 targetPos, Rectangle collision);
