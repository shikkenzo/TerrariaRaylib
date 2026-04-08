#include "collisionSystem.h"

enum Direction
{
	LEFT = 0,
	RIGHT,
	UP,
	DOWN
};

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
	case(LEFT):
		if (p.velocity.x < 0.f) //if going left
		{
			p.position.x = bRight + a.width / 2;
			p.velocity.x = 0.f;
		}
		break;
	case(RIGHT):
		if (p.velocity.x > 0.f) //if going right
		{
			p.velocity.x = 0.f;
			p.position.x = bLeft - a.width / 2;
		}
		break;
	case(UP):
		if (p.velocity.y < 0.f) //if jumping
		{
			p.position.y = bBottom + a.height / 2;
			p.velocity.y = 0.f;
		}
		break;
	case(DOWN):
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

Hit ShapecastAABB(Vector2 startingPos, Vector2 targetPos, Rectangle collision)
{
	Vector2 magnitudeVector = targetPos - startingPos;

	float startPos[DIMENSIONS] = { startingPos.x, startingPos.y };
	float magnitude[DIMENSIONS] = { magnitudeVector.x, magnitudeVector.y };
	float collisionSum[DIMENSIONS * 2] = { collision.x, collision.y, collision.width, collision.height };

	Hit hit = {};
	
	float lastEntry = -INFINITY;
	float firstExit = INFINITY;

	float min[DIMENSIONS] = { collisionSum[0], collisionSum[1] };
	float max[DIMENSIONS] = { collisionSum[0] + collisionSum[0 + DIMENSIONS], collisionSum[1] + collisionSum[1 + DIMENSIONS] };

	for (int i = 0; i < DIMENSIONS; i++)
	{
		if (magnitude[i] != 0)
		{
			float t1 = (min[i] - startPos[i]) / magnitude[i];
			float t2 = (max[i] - startPos[i]) / magnitude[i];

			lastEntry = std::max(lastEntry, std::min(t1, t2));
			firstExit = std::min(firstExit, std::max(t1, t2));
		}
		else if (startPos[i] <= min[i] || startPos[i] >= max[i])
		{
			return hit;
		}
	}

	if (firstExit > lastEntry && firstExit > 0.f && lastEntry < 1.f) //if hit
	{
		hit.position.x = startPos[0] + magnitude[0] * lastEntry;
		hit.position.y = startPos[1] + magnitude[1] * lastEntry;

		hit.isHit = true;
		hit.time = lastEntry;
	}

	return hit;
}
