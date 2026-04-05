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
		if (p.velocity.x < 0.f) //if going left
		{
			p.position.x = bRight + a.width / 2;
			p.velocity.x = 0.f;
		}
		break;
	case(1):
		if (p.velocity.x > 0.f) //if going right
		{
			p.velocity.x = 0.f;
			p.position.x = bLeft - a.width / 2;
		}
		break;
	case(2):
		if (p.velocity.y < 0.f) //if jumping
		{
			p.position.y = bBottom + a.height / 2;
			p.velocity.y = 0.f;
		}
		break;
	case(3):
		if (p.velocity.y > 0.f) //if falling
		{
			p.position.y = bTop - a.height / 2;
			p.velocity.y = 0.f;
		}
		break;

	default:
		break;
	}

	p.AdjustCollider();
}

Hit ShapecastAABB(float startingPos[DIMENSIONS], float magnitude[DIMENSIONS], float collision[DIMENSIONS * 2])
{
	Hit hit = {};
	
	float lastEntry = -INFINITY;
	float firstExit = INFINITY;

	float min[DIMENSIONS] = { collision[0], collision[1] };
	float max[DIMENSIONS] = { collision[0] + collision[0 + DIMENSIONS], collision[1] + collision[1 + DIMENSIONS] };

	for (int i = 0; i < DIMENSIONS; i++)
	{
		if (magnitude[i] != 0)
		{
			float t1 = (min[i] - startingPos[i]) / magnitude[i];
			float t2 = (max[i] - startingPos[i]) / magnitude[i];

			lastEntry = std::max(lastEntry, std::min(t1, t2));
			firstExit = std::min(firstExit, std::max(t1, t2));
		}
		else if (startingPos[i] <= min[i] || startingPos[i] >= max[i])
		{
			return hit;
		}
	}

	if (firstExit > lastEntry && firstExit > 0.f && lastEntry < 1.f) //if hit
	{
		hit.position.x = startingPos[0] + magnitude[0] * lastEntry;
		hit.position.y = startingPos[1] + magnitude[1] * lastEntry;

		hit.isHit = true;
		hit.time = lastEntry;
	}

	return hit;
}
