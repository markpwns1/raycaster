#include "stdafx.h"
#include "Ray.h"
#include "Wall.h"
#include "World.h"
#include "rayhit.h"
#include <memory>
#include "graphics.h"

using namespace maths;

Vector2f Ray::origin;
Vector2f Ray::direction;

Vector2f Ray::hit_point;

bool Ray::hit;

float Ray::hit_distance;
float Ray::hit_textureX;

inline float ray_line_intersect(Vector2f rayOrigin, Vector2f rayDirection, Vector2f point1, Vector2f point2)
{
	Vector2f v1 = rayOrigin - point1;
	Vector2f v2 = point2 - point1;
	Vector2f v3 = vec_perp(rayDirection);

	float d = vec_dot(v2, v3);

	float t2 = vec_dot(v1, v3) / d;

	if (t2 <= 0.0f || t2 > 1.0f) {
		return 0.0f;
	}

	float t1 = vec_cross(v2, v1) / d;

	return (t1 > 0) * t1;
}

void Ray::cast()
{
	for (int i = 0; i < World::workedWallCount; ++i) {
		Wall* wall = World::workedWalls[i];

		float distance = ray_line_intersect(origin, direction, wall->pointA, wall->pointB);

		if (distance == 0.0f) continue;

		hit = true;
		//hit.wall = wall;
		hit_distance = distance;
		hit_point = origin + direction * distance;
		hit_textureX = fmodf(vec_length(hit_point - wall->pointA), 1.0f);
		//hit.normal = vec_perp(wall->pointB - wall->pointA);
		return;
	}

	hit = false;
}
