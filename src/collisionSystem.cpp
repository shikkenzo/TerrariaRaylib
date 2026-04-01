#include "collisionSystem.h"

bool CheckCollisionAABB(Rectangle a, Rectangle b, int& directionOut)
{
	bool result = false;

	float aLeft = a.x;
	float aRight = a.x + a.width;
	float aTop = a.y;
	float aBottom = a.y + a.height;

	float bLeft = b.x;
	float bRight = b.x + b.width;
	float bTop = b.y;
	float bBottom = b.y + b.height;

	if (aRight > bLeft &&
		aLeft < bRight &&
		aBottom > bTop &&
		aTop < bBottom)
	{
		float overlapLeft = aRight - bLeft;
		float overlapRight = bRight - aLeft;
		float overlapTop = aBottom - bTop;
		float overlapBottom = bBottom - aTop;

		float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

		if (minOverlap == overlapLeft)
			directionOut = 0;
		else if (minOverlap == overlapRight)
			directionOut = 1;
		else if (minOverlap == overlapTop)
			directionOut = 2;
		else
			directionOut = 3;

		return true;
	}

	return false;
}

void ResolvePlayerCollision(Player& p, Rectangle b, int collisionDirection)
{
	Rectangle a = p.collision;

	float aLeft = a.x;
	float aRight = a.x + a.width;
	float aTop = a.y;
	float aBottom = a.y + a.height;

	float bLeft = b.x;
	float bRight = b.x + b.width;
	float bTop = b.y;
	float bBottom = b.y + b.height;
	
	//0 LEFT, 
	// 1 RIGHT, 
	// 2 UP, 
	// 3 DOWN
	switch (collisionDirection)
	{
	case(0):
		p.position.x = bRight + a.width / 2;
		p.velocity.x = 0;
		break;
	case(1):
		p.position.x = bLeft - a.width / 2;
		p.velocity.x = 0;
		break;
	case(2):
		p.position.y = bBottom + a.height / 2;
		p.velocity.y = 0;
		break;
	case(3):
		p.position.y = bTop - a.height / 2;
		p.velocity.y = 0;
		break;

	default:
		break;
	}

	p.AdjustCollider();
}
