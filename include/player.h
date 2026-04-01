#pragma once
#include "raylib.h"
#include "raymath.h"
#include <iostream>

struct Player
{
	Rectangle collision = { 0, 0, 20.f, 20.f };
	Vector2 position = { 0.f, 0.f };
	float baseSpeed = 500.f;
	float jumpSpeed = 5.f;
	bool canJump = false;
	float baseFallingAccel = 9.8f;
	float fallingAccelMultiplier = 2.f;
	Vector2 velocity = {};

	void CalculateVelocity(Vector2 input, float deltaTime);
	void Player::Jump();
	void AdjustCollider();
	void Draw();
};