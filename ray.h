#pragma once

#include "stdafx.h"
#include "rayhit.h"
#include <memory>

using namespace sf;

class Ray
{
public:
	static bool hit;
	static float hit_distance;
	static float hit_textureX;
	static Vector2f hit_point;

	static Vector2f origin;
	static Vector2f direction;

	static void cast();
};

