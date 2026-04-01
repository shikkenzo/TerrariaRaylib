#include "player.h"
#include "map.h"

void Player::CalculateVelocity(Vector2 input, float deltaTime)
{
	Vector2 l_addedMovement = { 0, 0 };
	
	l_addedMovement.y += baseFallingAccel * deltaTime;
	l_addedMovement.x += Vector2Normalize(input).x * baseSpeed * deltaTime;

	velocity.y += l_addedMovement.y;
	velocity.x = l_addedMovement.x;

	//std::cout << collision.x << ", " << collision.y << std::endl;
}

void Player::Jump()
{
	if (canJump)
	{
		velocity.y = -jumpSpeed;
		canJump = false;
	}
}

void Player::AdjustCollider()
{
	collision.x = position.x - collision.width / 2;
	collision.y = position.y - collision.height / 2;
}

void Player::Draw()
{
	DrawRectangle(collision.x, collision.y, collision.width, collision.height, DARKBLUE);
}