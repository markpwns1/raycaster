#pragma once

#include "stdafx.h"
#include "Wall.h"

using namespace sf;

class RayHit
{
public:
	RayHit();
	//RayHit(Wall *w);

	bool exists;
	float distance;
	float textureX;
	Vector2f point;
	//Vector2f normal;
	//Wall* wall;
};

