#pragma once
#include "raylib.h"
#include "player.h"

bool CheckCollisionAABB(Rectangle a, Rectangle b,  int& directionOut);
void ResolvePlayerCollision(Player& p, Rectangle c, int collisionDirection);
