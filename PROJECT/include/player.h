#pragma once
#include "raylib.h"
#include "raymath.h"
#include <iostream>

struct Player
{
	Rectangle collision = { 0, 0, 25.f, 50.f };
	Vector2 position = { 0.f, 0.f };
	float baseSpeed = 500.f;
	float jumpSpeed = 4.5f;
	bool canJump = false;
	bool isGrounded = false;
	float baseFallingAccel = 9.8f;
	float currentFallingAccel = baseFallingAccel;
	float fallingAccelMultiplier = 2.f;
	Vector2 velocity = {};

	void CalculateVelocity(Vector2 input, float deltaTime);
	void Jump();
	void MoveTo(Vector2 newPosition);
	void AdjustCollider();
	void Draw();
	void AdjustFallingSpeed();
};